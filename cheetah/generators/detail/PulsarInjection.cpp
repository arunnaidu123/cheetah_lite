#include "cheetah/generators/PulsarInjection.h"
#include "cheetah/generators/pulse_profile/ProfileManager.h"
#include "cheetah/utils/ConvolvePlan.h"
#include "panda/Log.h"
#include <boost/math/special_functions/sinc.hpp>
#include <exception>

#define M_PI_DBL 3.14159265358979323846264338327950288

namespace {

template<typename T>
class FftwAlloc {
    public:
        FftwAlloc(std::size_t size)
        {
            _ptr = static_cast<T*>( fftwf_malloc(size * sizeof(T)) );
        }
        operator T*() { return _ptr; }

        ~FftwAlloc()
        {
            fftwf_free(_ptr);
        }
    private:
        T* _ptr;
};

// constants for random number generator
static constexpr std::size_t IM1(2147483563);
static constexpr std::size_t IM2(2147483399);
static constexpr std::size_t IMM1(IM1 - 1);
static constexpr float AM(1.0/IM1);
static constexpr int IA1(40014);
static constexpr int IA2(40692);
static constexpr int IQ1(53668);
static constexpr int IQ2(52774);
static constexpr int IR1(12211);
static constexpr int IR2(3791);
static constexpr int NTAB(32);
static constexpr std::size_t NDIV(1 + IMM1/NTAB);
static constexpr float EPS(1.2e-7);
#define RNMX (1.0 - EPS)
#define MJK_RAND_R1279_SZ 2048
#define MJK_RAND_R1279_SZ1 2047
#define TWO_PI 6.2831853071795864769252866

typedef struct MjkRand {
    public:
        MjkRand(uint64_t init_seed)
            : gauss_buffer(nullptr)
        {
            long nseed = -(long) init_seed;
            next = -1;
            nthreadmax = 8;
            buffer_len = 1024 * 16 * nthreadmax;
            buffer = (double*) calloc(buffer_len, sizeof(double));
            rmax = INT32_MAX/2;
            gauss_next = -1;
            gauss_buffer = NULL;
            srand(init_seed);
            ir = (int*) calloc(nthreadmax, sizeof(int));
            seed = (uint64_t*) calloc(MJK_RAND_R1279_SZ * nthreadmax, sizeof(uint64_t));
            uint64_t j, k;
            for(std::size_t i = 0; i < (unsigned) (MJK_RAND_R1279_SZ * nthreadmax); ++i) {
                j = (uint64_t) (UINT32_MAX * (double) nrran2(&nseed));
                j = j << 8 * sizeof(uint32_t);
                k = (uint64_t) (UINT32_MAX * (double) nrran2(&nseed));
                seed[i] = j + k;
            }
            rand_fill();
        }

        ~MjkRand()
        {
            free(buffer);
            free(ir);
            free(seed);
            if (gauss_buffer != nullptr) free(gauss_buffer);
        }

        void rand_fill()
        {
            uint64_t ibuf, iblk, blksize = buffer_len/nthreadmax;
            for (iblk = 0; iblk < nthreadmax; iblk++) {
                for (ibuf = 0; ibuf < blksize; ibuf++) {
                    buffer[ibuf + iblk * blksize] = a2281(seed + iblk * MJK_RAND_R1279_SZ, &(ir)[iblk]);
                }
            }
            next = buffer_len - 1;
        }

        void fill_gauss() {
            rand_fill();
            double o1, o2, i1, i2;
            //  const uint32_t chunk = 64;
            if (gauss_buffer == NULL) {
                gauss_buffer = (float*) calloc(buffer_len, sizeof(float));
            }
            //#pragma omp parallel private(i,o1,o2,i1,i2)
            //#pragma omp for schedule(dynamic,chunk)
            for(std::size_t i = 0; i < buffer_len; i += 2) {
                i1 = buffer[i];
                i2 = buffer[i + 1];
                rand_gauss2(i1, i2, &o1, &o2);
                gauss_buffer[i] = o1;
                gauss_buffer[i + 1] = o2;
            }
            gauss_next = buffer_len - 1;
            next = -1;
        }

        double rand_double()
        {
            if (next < 0) {
                rand_fill();
            }
            return buffer[next--];
        }

        float rand_gauss()
        {
            if (gauss_next < 0) {
                fill_gauss();
            }
            return gauss_buffer[gauss_next--];
        }

    protected:
        float nrran2(long *idum)
        {
            int j;
            long k;
            static long idum2 = 123456789;
            static long iy = 0;
            static long iv[NTAB];
            float temp;

            if (*idum <= 0) {
                if (-(*idum) < 1) *idum = 1;
                else *idum = -(*idum);
                idum2 = (*idum);
                for (j = NTAB + 7; j >= 0; j--) {
                    k = (*idum)/IQ1;
                    *idum = IA1 * (*idum - k * IQ1) - k * IR1;
                    if (*idum < 0) *idum += IM1;
                    if (j < NTAB) iv[j] = *idum;
                }
                iy = iv[0];
            }
            k = (*idum)/IQ1;
            *idum = IA1 * (*idum - k * IQ1) - k * IR1;
            if (*idum < 0) *idum += IM1;
            k = idum2/IQ2;
            idum2 = IA2 * (idum2 - k * IQ2) - k * IR2;
            if (idum2 < 0) idum2 += IM2;
            j = iy/NDIV;
            iy = iv[j] - idum2;
            iv[j] = *idum;
            if (iy < 1) iy += IMM1;
            temp = AM * iy;
            if (temp > RNMX) return RNMX;
            else return temp;
        }

        double a2281(uint64_t seed[MJK_RAND_R1279_SZ], int *i)
        {
            const int i1 = (*i - 2281) & MJK_RAND_R1279_SZ1;
            const int i2 = (*i - 1252) & MJK_RAND_R1279_SZ1;
            seed[*i] = seed[i1] +seed[i2];
            double ret = seed[*i]/(double) UINT64_MAX;
            (*i) = (*i + 1)&MJK_RAND_R1279_SZ1;
            return ret;
        }

        void rand_gauss2(double rand1, double rand2, double *o1, double *o2)
        {
            if (rand1 < 1e-100) rand1 = 1e-100;
            rand1 = -2 * log(rand1);
            rand2 = rand2 * TWO_PI;
            *o1 = sqrt(rand1) * cos(rand2);
            *o2 = sqrt(rand1) * sin(rand2);
        }

    private:
        uint32_t nthreadmax;
        uint32_t buffer_len;
        double *buffer;
        int32_t next;
        uint32_t rmax;
        int32_t gauss_next;
        float* gauss_buffer;
        uint64_t* seed;
        int32_t* ir;
} mjk_rand_t;



inline void convolve(ska::cheetah::utils::ConvolvePlan& plan)
{
    plan.convolve();
}

struct IsmModels {
    IsmModels(std::size_t number_of_models, std::size_t model_size)
        : _size(number_of_models)
    {
        _models = (float**) malloc(sizeof(float*) * _size);
        if(! _models) throw std::bad_alloc();
        for(std::size_t i = 0; i < _size; ++i) {
            _models[i] = (float*) malloc(sizeof(float) * model_size);
            if(! _models[i]) {
                clean_models(i);
                throw std::bad_alloc();
            }
        }
    }

    ~IsmModels()
    {
        clean_models(_size);
        delete _models;
    }

    inline
    float* operator[](std::size_t i) {
        return _models[i];
    }

    private:
        inline
        void clean_models(std::size_t model_num) {
            for(std::size_t i = 0; i < model_num; ++i) {
                free(_models[i]);
            }
        }

    private:
        std::size_t _size;
        float** _models;
};

} //namespace

namespace ska {
namespace cheetah {
namespace generators {


template<typename DataType>
PulsarInjection<DataType>::PulsarInjection(PulsarInjectionConfig const& config, ProfileManager const& profile_manager)
    : BaseT()
    , _config(config)
    , _profile_manager(profile_manager)
    , _phase_model(config.phase_model())
    , _last_number_of_channels(0)
    , _last_number_of_spectra(0)
{
    // these calls will throw if not found
    _profile = profile_manager.profile(config.profile());
}

template<typename DataType>
PulsarInjection<DataType>::~PulsarInjection()
{
}

template<typename DataType>
void PulsarInjection<DataType>::next(DataType& data)
{
    float spec_index = _config.spectral_index();
    const float in_snr = _config.signal_to_noise(); // S/N
    float pulse_energy_sigma = 0.2;
    float frac;
    uint32_t seed = time(NULL);
    uint_fast32_t nsubpulse = 5;
    uint_fast32_t n;

    //  fftwf_init_threads();
    //  fftwf_plan_with_nthreads(1);

    const uint_fast32_t nchan_const = data.number_of_channels();;
    const uint64_t nsamp = data.number_of_spectra();;
    long double tsamp = static_cast<boost::units::quantity<boost::units::si::time, double>>(data.sample_interval()).value(); // ensure we get value in seconds
    if( data.channel_frequencies().size() <=1 ) return;
    long double const fch1 = static_cast<boost::units::quantity<data::MegaHertz, double>>(data.channel_frequencies()[0]).value();
    utils::ModifiedJulianClock::time_point mjd(static_cast<cheetah::utils::ModifiedJulianClock::time_point>(data.start_time()));
    utils::ModifiedJulianClock::duration const tsamp_mjd(tsamp/86400.0L);

    // TODO scale to the RMS of the existing data
    const double noiseAmp = 24; // 8-bit only to match gaussian noise generated by the fake program

    // normalise the profile (if it hasn't been done already)
    if(nsamp != _last_number_of_spectra || nchan_const != _last_number_of_channels)
    {
        // restore profile from original template and rescale
        PANDA_LOG_DEBUG << "rescaling profile '" << static_cast<std::string>(_config.profile()) << "'";
        _profile = _profile_manager.profile(_config.profile());
        double sum = _profile.sum();
        sum /= (double) _profile.size();
        if(_profile.size() == 0) {
            panda::Error e("profile is empty:");
            e << static_cast<std::string>(_config.profile());
            throw e;
        }
        double scale = noiseAmp * in_snr/sqrt(nchan_const * nsamp)/sum;
        PANDA_LOG_DEBUG << "mean = " << sum << ", scale = " << scale;
        // normalise to pseudo-S/N in 1s
        std::for_each(_profile.begin(), _profile.end(), [scale](PulsarProfile::ProfileDataPoint& dp) { dp *= scale; });
        _last_number_of_spectra = nsamp;
        _last_number_of_channels = nchan_const;
    }
    const uint_fast32_t nprof = _profile.size();

    FftwAlloc<float> ism_convolution(nprof);
    FftwAlloc<float> subpulse_map(nprof); // mask for the number of pulses actually present (nsubpulse out of n randomly selected)
    FftwAlloc<float> subpulse_profile(nprof);
    FftwAlloc<float> unsmeared_profile(nprof);
    FftwAlloc<float> smeared_profile(nprof);

    utils::ConvolvePlan ism_convolution_plan(nprof, unsmeared_profile, ism_convolution, smeared_profile);
    utils::ConvolvePlan subpulse_convolution_plan(nprof, subpulse_profile, subpulse_map, unsmeared_profile);

    for(std::size_t i = 0; i < nprof; ++i){
        subpulse_profile[i] = 1.0;
    }

    double sum = 0.0;
    // normalise the subpulse profile
    for(std::size_t i = 0; i < nprof; ++i) {
        sum += subpulse_profile[i];
    }
    sum /= (double) nprof;
    // we also normalise by the number of subpulses so that the final subpulse profile will have an area of 1.
    double scale = 1.0/sum/(float) nsubpulse;
    for(std::size_t i = 0; i < nprof; ++i) {
        subpulse_profile[i] = subpulse_profile[i] * scale;
    }

    long double phase;
    int_fast32_t pbin;
    uint_fast32_t ch = 0;
    std::vector<boost::units::quantity<data::MegaHertz, double>> freq(nchan_const + 1);
    std::vector<float> sidx(nchan_const);
    std::vector<uint_fast32_t> ism_idx(nchan_const);
    IsmModels ism_models(nchan_const, nprof);
    uint_fast32_t nism = 0;
    const uint_fast32_t NCOPY = nprof * sizeof(float);
    std::vector<double> poff(nchan_const);

    const double pulse_energy_norm = exp(pow(pulse_energy_sigma, 2)/2.0);
    double p0 = _phase_model(mjd, data.channel_frequencies()[0]);

    std::vector<int_fast32_t> prevbin(nchan_const);
    int_fast32_t dbin;

    MjkRand rnd(seed);
    for (ch = 0; ch < nchan_const; ++ch) {
        freq[ch] = data.channel_frequencies()[ch];
        poff[ch] = _phase_model(mjd, freq[ch]) - p0;
        prevbin[ch] = -1;
        sidx[ch] = pow(freq[ch].value()/fch1, spec_index);
    }
    freq[nchan_const] = freq[nchan_const -1] + (freq[nchan_const - 1] - freq[nchan_const -2]); // assume a const bin width

    // set up the ISM model
    {
        int_fast32_t dm_bin=-1;
        int_fast32_t scatter_bin=-1;
        float* dm_conv = ism_convolution; // re-use an existing convolution function
        float* scatt_conv = unsmeared_profile;
        float* out_conv = smeared_profile;

        double dm = _config.dm().value();
        double const freq0 = boost::units::quantity<data::MegaHertz, double>(freq[0]).value();
        auto phase0 = _phase_model(mjd, freq[0]) + dm/(freq0*freq0);
        for (ch = 0; ch < nchan_const; ++ch) {
            //phase = T2Predictor_GetPhase(&pred,mjd,freq[ch]) - T2Predictor_GetPhase(&pred,mjd,freq[ch+1]);
            double const freq_ch = boost::units::quantity<data::MegaHertz, double>(freq[ch+1]).value();
            auto phase1 = _phase_model(mjd, freq[ch+1]) + dm/(freq_ch*freq_ch);
            phase = phase0 - phase1;
            phase0 = phase1;
            pbin = std::abs(phase) * nprof;
            if (pbin < 1) pbin = 1;
            else if (pbin%2 == 0) pbin -= 1;

            if ((pbin != dm_bin) != scatter_bin) {
                // make new ISM model
                sum = 0;
                for(std::size_t i = 0; i < nprof; ++i) {
                    // create a dispersed sinc function profile modelling the telescope response function
                    // to use in convolution.
                    // The function sinc(l) == sin(pi*l)/(pi*l) is the Fourier transform of the unit rectangle function
                    // and is the electric-field pattern of a uniformly illuminated unit aperture.
                    int_fast32_t x = (i - pbin/2 + nprof)%nprof;
                    double y = 2. * M_PI_DBL * (i - pbin/2.0 + 0.5)/pbin;
                    if (i < (std::size_t)pbin) {
                        dm_conv[x] = boost::math::sinc_pi(y);
                        sum += dm_conv[x];
                    }
                    else dm_conv[x] = 0;
                }
                for(std::size_t i = 0; i < nprof; ++i){
                    dm_conv[i] /= sum;
                }

                scatt_conv[0] = 1.0;
                for(std::size_t i = 1; i < nprof; ++i){
                    scatt_conv[i] = 0;
                }

                convolve(ism_convolution_plan); // dm_conv * scatt_conv => out_conv

                memcpy(ism_models[nism], out_conv, NCOPY);
                ++nism;
                dm_bin = pbin;
                scatter_bin = 0;
            }
            ism_idx[ch] = nism - 1;
        }
    }
    PANDA_LOG_DEBUG << "Generated " << nism << " ISM models";

    std::vector<std::vector<float>> grads(nism, std::vector<float>(nprof));
    std::vector<std::vector<float>> output_prof(nism, std::vector<float>(nprof));

    PANDA_LOG_DEBUG << "Starting simulation";
    uint64_t count = 0;
    int64_t ip0;
    int64_t prev_ip0 = INT64_MAX;
    std::vector<float>rands(nchan_const);
    for(std::size_t sample_index = 0; sample_index < nsamp; ++sample_index)
    {
        auto spectra = data.spectrum(sample_index);
        auto channel_it = spectra.begin();

        for (ch = 0; ch < nchan_const; ++ch) {
            rands[ch] = rnd.rand_double();
        }
        p0 = _phase_model(mjd, freq[0]);
        ip0 = (int64_t) floor(p0);
        //pragma omp paralell private(i,n,ch,phase,frac,pbin,A,dbin) shared(ip0,prev_ip0)
        {
            if (ip0 != prev_ip0) {
                for(std::size_t i = 0; i < nprof; i++) {
                    subpulse_map[i] = 0;
                }
                if(nsubpulse==0) {
                    // take the profile as given
                    std::copy(_profile.begin(), _profile.end(), &unsmeared_profile[0]);
                }
                else {
                    // generate subpulses
                    for (n = 0; n < nsubpulse; ++n) {
                        // subpulse_map - emulate that only parts of the pulse are actually seen(<=nsubpulse)
                        std::size_t i = floor(rnd.rand_double() * nprof);
                        subpulse_map[i] += exp(rnd.rand_gauss() * pulse_energy_sigma)/pulse_energy_norm;
                    }
                    // this convolution gives us a masked profile (i.e. a profile with random bits missing)
                    // with the result in unsmeared_profile
                    convolve(subpulse_convolution_plan);

                    // use this mask to modulate the profile and define our unsmeared_profile
                    std::transform(_profile.begin(), _profile.end(), &unsmeared_profile[0], &unsmeared_profile[0], std::multiplies<float>());
                }

                //pragma omp single
                {
                    for(std::size_t i = 0; i < nism; ++i) {
                        memcpy(static_cast<float*>(ism_convolution), ism_models[i], NCOPY);
                        convolve(ism_convolution_plan);
                        memcpy(static_cast<void*>(&output_prof[i][0]), static_cast<void*>(smeared_profile), NCOPY);
                        for (n = 0; n < nprof - 1; ++n) {
                            grads[i][n] = smeared_profile[n + 1] - smeared_profile[n];
                        }
                        grads[i][nprof - 1] = smeared_profile[0] - smeared_profile[nprof - 1];
                    }
                    prev_ip0 = ip0;
                }
            }
            //pragma omp  for  schedule(dynamic,128)
            for (ch = 0; ch < nchan_const; ++ch) {
                phase = p0 + poff[ch];
                phase = phase - floor(phase);
                pbin = floor(phase * nprof);
                frac = phase * nprof - pbin;
                float final_signal = output_prof[ism_idx[ch]][pbin] + frac * grads[ism_idx[ch]][pbin];
                if (prevbin[ch] < 0) prevbin[ch] = pbin;
                dbin = pbin - prevbin[ch];
                while (dbin < 0) dbin += nprof;
                while (dbin > 1) {
                    final_signal += output_prof[ism_idx[ch]][prevbin[ch]];
                    prevbin[ch] += 1;
                    dbin = pbin - prevbin[ch];
                    while (dbin < 0) dbin += nprof;
                }

                if (final_signal > 0) {
                    *channel_it += floor(sidx[ch] * final_signal + rands[ch]);
                }
                ++channel_it;
                prevbin[ch] = pbin;
            }
        }

        mjd += tsamp_mjd;
        count++;
    }
    PANDA_LOG_DEBUG << "Simulation ended.";
}

} // namespace generators
} // namespace cheetah
} // namespace ska
