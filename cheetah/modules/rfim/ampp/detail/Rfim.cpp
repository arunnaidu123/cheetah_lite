/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 The SKA organisation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "cheetah/modules/rfim/ampp/Rfim.h"
#include "cheetah/data/Units.h"
#include "panda/Error.h"
#include "panda/Log.h"
#include <cmath>
#include <numeric>
#include <type_traits>

namespace ska {
namespace cheetah {
namespace modules {
namespace rfim {
namespace ampp {


template<typename RfimTraits>
typename Rfim<RfimTraits>::ReturnType Rfim<RfimTraits>::operator()(panda::PoolResource<Cpu>&, std::shared_ptr<cheetah::data::TimeFrequency<Cpu, NumericalRep>> data)
{
    return run(*data);
}

template<typename RfimTraits>
Rfim<RfimTraits>::Rfim(Config const& c, typename RfimTraits::BandPassHandler const& bp_handler)
    : _config(c)
    , _zero_dm(c.zero_dm()?0:1)
    , _bad_spectra(0)
    , _fraction_bad_channels(0.0)
    , _sample_counter(0)
    , _max_history(c.max_history())
    , _use_mean_over_rms(0)
    , _bandpass_data_ptr(std::make_shared<Spectrum<NumericalRep>>())
    , _bandpass_data(*_bandpass_data_ptr)
    , _bandpass_handler(bp_handler)
{
    assert(_max_history != 0);
    _mean_buffer.set_capacity(_max_history);
    _rms_buffer.set_capacity(_max_history);
}

template<typename RfimTraits>
Rfim<RfimTraits>::~Rfim()
{
}

template<typename RfimTraits>
Rfim<RfimTraits>::Rfim(Rfim&& c)
    : _config(c._config)
    , _zero_dm(c._zero_dm)
    , _bad_spectra(c._bad_spectra)
    , _mean_buffer(std::move(c._mean_buffer))
    , _rms_buffer(std::move(c._rms_buffer))
    , _fraction_bad_channels(c._fraction_bad_channels)
    , _sample_counter(c._sample_counter)
    , _max_history(c._max_history)
    , _use_mean_over_rms(c._use_mean_over_rms)
    , _bandpass_data_ptr(c._bandpass_data_ptr)
    , _bandpass_data(*_bandpass_data_ptr)
    , _bandpass_handler(c._bandpass_handler)
{
}

namespace {
struct MiniBand {
    public:
        MiniBand(std::size_t number_of_chans, data::DimensionSize<data::Frequency> next_band_index, BandPassConfig const& config)
            : band_sigma(0)
            , band_mean(config.mean())
            , band_mean_sq(0.0)
            , band_min(0)
            , number_of_channels(number_of_chans)
            , next_band_index(next_band_index)
            , _config(config)
        {}

        void reset() {
            band_sigma = 0.0;
            band_mean = 0.0;
            band_mean_sq = 0.0;
            band_min = std::numeric_limits<float>::max();
        }

    public:
        float band_sigma;
        float band_mean;
        float band_mean_sq;
        float band_min;
        std::size_t number_of_channels;
        data::DimensionSize<data::Frequency> next_band_index;
        BandPassConfig const& _config;
};

}

template<typename RfimTraits>
void Rfim<RfimTraits>::run_impl(data::TimeFrequency<Cpu, NumericalRep> const& data, DataAdapter& adapter)
{
    typedef typename data::TimeFrequency<Cpu, NumericalRep>::ConstSpectra Spectra;
    data::DimensionSize<data::Frequency> nchannels(data.number_of_channels());
    if((std::size_t)nchannels == 0) return;

    double sqr_nchannels = std::sqrt((unsigned)nchannels);
    unsigned nsamples = data.number_of_spectra();

    float spectrum_rms = 0.;

    // Split the spectrum into nbands bands for the purpose of matching
    // the spectrum to the model
    unsigned nbands = 8;
    unsigned const channels_per_band = nchannels / nbands;
    std::vector<MiniBand> mini_bands;
    {
        // TODO we could store this setup once for each value of nchannels
        mini_bands.reserve(nbands);
        data::DimensionSize<data::Frequency> next_index(0);
        for(unsigned i = 0; i < nbands; ++i) {
           next_index += data::DimensionSize<data::Frequency>(channels_per_band);
           mini_bands.emplace_back(channels_per_band, next_index, _config.bandpass());
        }
        unsigned channels_last_band = nchannels - channels_per_band * nbands;
        if(channels_last_band != 0) {
           mini_bands.emplace_back(channels_last_band, next_index + channels_last_band, _config.bandpass());
           ++nbands; // add a band for any left over channels
        }
    }

    std::unique_lock<std::mutex> bandpass_lock(_mutex);
    // setuup _replacement_sample the first time round
    if(_replacement_sample.size() != nchannels) {
        _replacement_sample.resize(nchannels, 0);
        _bandpass_data.resize(nchannels, _config.bandpass().mean());
    }

    for(unsigned sample_number = 0U; sample_number < nsamples; ++sample_number) {
        //auto const& bandpass = _bandpass.current_set();
        float spectrum_sum = 0.0;
        float spectrum_sum_sq = 0.0;
        std::vector<float> good_channels(nbands, 0.0);

        // reset mini bands for each new spectrum
        for(auto& band : mini_bands) {
            band.reset();
        }

        // Training phase, perform the following tasks:
        //
        Spectra sample = data.spectrum(sample_number);
        if (!_rms_buffer.full())
        {
            adapter.mark_good(sample); // assume its good until we know better
            data::DimensionIndex<data::Frequency> current_channel(0);
            for(auto& mini_band : mini_bands) {
                do {
                    auto const& channel_value = sample[current_channel];
                    // determine the miniband minimum as a good proxy for the level
                    if (channel_value < mini_band.band_min) mini_band.band_min = channel_value;
                    // compute the mean and rms per mini band
                    mini_band.band_mean += channel_value;
                    mini_band.band_mean_sq += std::pow(channel_value,2);
                }
                while(++current_channel < mini_band.next_band_index);
                // rewind to begining of miniband and update _bandpass_data
                current_channel -= mini_band.number_of_channels;
                do {
                    // bandpass is taken to be the weighted average value of the
                    // current minimum and the existing bandpass value for this channel
                    _bandpass_data[current_channel] = (_bandpass_data[current_channel] * _rms_buffer.size() + mini_band.band_min) / (_rms_buffer.size()+1);
                    _replacement_sample[current_channel] = _bandpass_data[current_channel];
                }
                while(++current_channel < mini_band.next_band_index);
            }

            // Now find the distances between data and model and the RMSs in
            // each band COULD PROBABLY BE TUCKED INTO THE FOR LOOP ABOVE
            spectrum_rms=std::numeric_limits<float>::max();
            float mini_band_sum = 0.0;
            for (MiniBand& mini_band : mini_bands) {
                //odata_minus_model[b] = mini_data[b] - mini_model[b];
                mini_band.band_mean /= mini_band.number_of_channels;
                mini_band.band_sigma = sqrt(mini_band.band_mean_sq/mini_band.number_of_channels
                                     - std::pow(mini_band.band_mean,2)); // TODO adjust for final band size
                // Assume the minimum bandSigma to be the best estimate of this
                // spectrum RMS
                if(mini_band.band_sigma < spectrum_rms) spectrum_rms = mini_band.band_sigma;
                // Get the bandpass average across the minibands
                mini_band_sum += mini_band.band_mean;
            }
            // Push back the spectrum rms into a ring buffer and work out the average
            _rms_buffer.push_back(spectrum_rms);
            _rms_run_ave = std::accumulate(_rms_buffer.begin(), _rms_buffer.end(),
                                         0.0)/_rms_buffer.size();
            //std::cout << "RFIstats: training rms buffer average=" << _rms_run_ave
            //    << " " << _rms_buffer.size() << " " << spectrum_rms << std::endl;
            // The very last time this is done, store a reference value of
            // the mean over the rms; this works as I have just added on the
            // last value two lines above
            if (_rms_buffer.full()) {
                _mean_over_rms = 96.0 / _rms_run_ave;
/*
                // smooth out the bandpass model within 3 sigma
                data::DimensionIndex<data::Frequency> i(0);
                auto last_value = mini_bands[0].band_mean;
                auto three_sigma = spectrum_rms * 3.0;
                for(auto& value : _bandpass_data)
                {
                    if( value > last_value) {
                       if( value - last_value > three_sigma )
                       {
                           value = last_value + spectrum_rms;
                       }
                    }
                    else {
                       if( last_value - value > three_sigma )
                       {
                           value = last_value - spectrum_rms;
                       }
                    }
                    last_value = value;
                    _replacement_sample[i] = value;
                    ++i;
                }
*/
                _bandpass_data.start_time(data.start_time());
                _bandpass_data.calculate_mean();
                _bandpass_rms=_rms_run_ave;
                _bandpass_handler(_bandpass_data);
            }

            // update replacement sample stats
            _replacement_sample_rms = spectrum_rms;
            //_replacement_sample_mean = _mean_run_ave;
            _replacement_sample_mean = mini_band_sum / nbands;

            //std::cout << "RFIstats: replacement_sample_rms=" << _replacement_sample_rms
              //  << " replacement_sample_mean=" << _replacement_sample_mean << std::endl;
            continue;
        }
        else {
            // Now out of the training phase
            // Update the running average with the current last value,
            // and take the oldest off the end. Then add the new
            // value onto the buffer. The idea here is that the new
            // value is not used for the current spectrum, but rather
            // for the one after. The current spectrum is evaluated
            // based on the rms values in the buffer up to that
            // point.
            _rms_run_ave -= (_rms_buffer.front()/_rms_buffer.size());
            _rms_run_ave += (_rms_buffer.back()/_rms_buffer.capacity());

            // In extreme RFI cases, the measured RMS may start growing
            // due to particular RFI signals. The mean over rms ratio
            // should remain approximately constant over the course of the
            // observation. Use this as a safety check, after the mean has
            // been reasonably determined, to set the RMS to a more
            // reliable value :
            if(_use_mean_over_rms) {
                //recover the rms running average
                spectrum_rms = std::abs(_mean_run_ave / _mean_over_rms);
                _rms_buffer.push_back(spectrum_rms);
            }
            else {
                _rms_buffer.push_back(_replacement_sample_rms);
            }
        }
        // now use this rms to define a margin of tolerance for bright
        // channels
        float margin = _config.channel_rejection_rms() * _rms_run_ave;

        // Now loop around all the channels: if you find a channel where
        // (I - bandpass) > margin, then mark as bad
        std::size_t total_good_channels = 0;
        data::DimensionIndex<data::Frequency> current_channel(0);
        for(auto & channel_value : sample) {
            if (channel_value - _bandpass_data[current_channel] > margin) {
                // clipping this channel to values from the last good
                // spectrum
                adapter.mark_bad(channel_value, current_channel);
                //std::cout << "The rms_run_ave at clipping: " << _rms_run_ave << std::endl;
            }
            else{
                unsigned band;
                if(channels_per_band) {
                    band = (int)current_channel / channels_per_band;
                }
                else {
                    band = 0;
                }
                ++good_channels[band];
                ++total_good_channels;
                spectrum_sum += channel_value;
            }
            ++current_channel;
        }
        // So now we have the mean of the incoming data, in a reliable
        // form after channel clipping
        _fraction_bad_channels += (float)(nchannels - total_good_channels)/nchannels;
        spectrum_sum /= total_good_channels;

        // Check if more than 20% of the channels in each band were
        // bad. If so in more than half of the bands, 4 in this case,
        // keep record. Also, if one band is completely gone, or less
        // than 80% of the total survive, keep record.
        unsigned bad_bands = 0;
        float good_channel_fraction = 0.8;
        for (unsigned b = 0; b < nbands; ++b){
            if (good_channels[b] < good_channel_fraction * mini_bands[b].number_of_channels) {
                ++bad_bands;
            }
            /*
            if (good_channels[b] == 0) {
                bad_bands += nbands / 2;
            }
            */
        }
        if (total_good_channels < good_channel_fraction * nchannels)
            bad_bands += nbands/2;

        // Let us now build up the running average of spectrumSum values
        // (_maxHistory of them) if the buffer is not full, compute the
        // new meanRunAve like this


        if (!_mean_buffer.full()) {
            _mean_buffer.push_back(spectrum_sum);
            _mean_run_ave = std::accumulate(_mean_buffer.begin(), _mean_buffer.end(),
                                            0.0)/_mean_buffer.size();
        }
        else {
            //   just update the running average with the new value, and
            //   take the oldest off the end, using the same principle as
            //   with the rms buffer, i.e. do not use the current
            //   measurement for the current spectrum.

            // Note there is a tiny descrepance at the point when the
            // buffer is first full

            _mean_run_ave -= _mean_buffer.front()/_mean_buffer.size();
            _mean_run_ave += _mean_buffer.back()/_mean_buffer.size();
            _mean_buffer.push_back(spectrum_sum);
        }

        // Now we can check if this spectrum has an abnormally high mean
        // compared to the running average

        // Let us define the tolerance first, and remember, we are
        // summing across nBins, hence sqrt(nchannels), strictly only valid
        // for Gaussian stats
        float spectrum_rms_tolerance = _config.spectrum_rejection_rms() *
            _bandpass_rms/sqr_nchannels;

        //Now check, if spectrum_sum - model > tolerance, declare this
        //time sample useless, replace its data and take care of the
        //running averages, also cut the first_spectra worth of the
        //first spectra, also cut spectra where badBands >= 4, see
        //above

        /*
        unsigned const first_spectra = 1000;
        if (_mean_buffer.size() < first_spectra) {
            // clip the sample, but continue to build the stats; this
            // helps the stats converge
            RfiPolicy::replace_sample(sample, _replacement_sample.begin());
        }
        else
        */
        if (spectrum_sum - _mean_run_ave > spectrum_rms_tolerance ||
                 bad_bands >= nbands / 2 ) {
            // we need to remove this entire spectrum
            adapter.mark_bad(sample);
            //std::cout << " rms_run_ave at bad spectrum: " << _rms_run_ave << std::endl;
            // keep a record of bad spectra
            ++_bad_spectra;

            // now remove the last samples from the running average
            // buffers and replace them with the values of the
            // lastgoodspectrum
            _mean_buffer.pop_back();
            _rms_buffer.pop_back();
            _mean_buffer.push_back(_replacement_sample_mean);
            _rms_buffer.push_back(_replacement_sample_rms);
        }
        // else keep a copy of the original spectrum, as it is good, but
        // clip everything down to 3 sigma

        else {
            spectrum_sum = 0.0; //
            spectrum_sum_sq = 0.0; //
            data::DimensionIndex<data::Frequency> current_channel(0);
            for(auto const& channel_value : sample) {
                if (channel_value - _bandpass_data[current_channel]
                    < 3.0 * _rms_run_ave) {
                    _replacement_sample[current_channel] = channel_value;
                    adapter.mark_good(channel_value, current_channel);
                }
                spectrum_sum += _replacement_sample[current_channel];
                spectrum_sum_sq += std::pow(_replacement_sample[current_channel],2);
                ++current_channel;
            }
            // and keep the mean and rms values as computed
            _replacement_sample_mean = spectrum_sum / nchannels;
            _replacement_sample_rms = sqrt(spectrum_sum_sq/nchannels -
                                           std::pow(_replacement_sample_mean,2));
        }
        // Now we have a valid spectrum, either the original or
        // replaced; this spectrum is good, so let us do the final
        // bits of post processing reset the spectrumSum, and SumSq,
        // and flatten subtract the bandpass from the data.
        //
        /*
        spectrum_sum = 0.0;
        spectrum_sum_sq = 0.0;
        current_channel = data::DimensionIndex<data::Frequency>(0);
        for(auto & channel_value : sample) {
            if (_bandpass_data[current_channel] > channel_value)
            {
                channel_value=0;
                ++current_channel;
                continue;
            }
            channel_value -= _bandpass_data[current_channel];
            spectrum_sum += channel_value;
            spectrum_sum_sq += std::pow(channel_value,2);
            ++current_channel;
        }

        // and normalize: bring to zero mean if zerodm is specified or
        // use the running mean if not
        spectrum_sum /= nchannels; // New meaning of these two variables
        spectrum_rms = sqrt(spectrum_sum_sq/nchannels - std::pow(spectrum_sum,2));

        // Avoid nastiness in those first spectra by avoiding divisions
        // by zero
        if (spectrum_rms == 0.0) spectrum_rms = 1.0;
        for(auto & channel_value : sample) {
            channel_value -= (float)_zero_dm * spectrum_sum;
            // it may be better to normalize by the running average RMS,
            // given this is a sensitive operation. For example, an
            // artificially low rms may scale things up
            channel_value /= spectrum_rms;
            // make sure this division is not introducing signals that
            // you would have clipped
            if (channel_value > _config.channel_rejection_rms())
                channel_value = 0.0;
        }
        */
        // The bandpass is flat and the spectrum clean and normalized,
        // so move to the next spectrum and write out some stats:
        unsigned const report_stats_every = 1 * _max_history;
        if (_sample_counter == 0) {
            // calculate fractions
            float fraction_bad_spectra = 100.0 *
                (float)_bad_spectra / (float)report_stats_every;

            // if the fraction of bad spectra becomes >99%, then empty the
            // circular buffers and go into learning mode again
            if (fraction_bad_spectra > 99.0) {
                _rms_buffer.resize(0);
                _mean_buffer.resize(0);
                PANDA_LOG << "Lost track of the RFI model, retraining.";
            }

            float fraction_bad_channels = 100.0 *
                _fraction_bad_channels / (float)report_stats_every ;
            PANDA_LOG_DEBUG <<  "RFIstats: mean=" << _mean_run_ave << " rms=" << _rms_run_ave << " bad_spec="
                      << fraction_bad_spectra << " bad_chan"
                      << fraction_bad_channels ;
            // Reset _bad
            _bad_spectra = 0;
            _fraction_bad_channels = 0.0;

        }
        // and update the model
        //_bandpass.set_mean(_mean_run_ave);
        //std::cout << margin << " " << spectrum_rms_tolerance << " " << _mean_run_ave << std::endl;
        _bandpass_data.adjust_mean(_mean_run_ave);
        _bandpass_rms=_rms_run_ave;
        ++_sample_counter;
        _sample_counter = _sample_counter % report_stats_every;
    }
}

template<typename RfimTraits>
typename Rfim<RfimTraits>::ReturnType Rfim<RfimTraits>::run(cheetah::data::TimeFrequency<Cpu, NumericalRep>& data)
{
    return _policy.template exec([this](cheetah::data::TimeFrequency<Cpu, NumericalRep>& data, DataAdapter& adapter) mutable
                        {
                             this->run_impl(data, adapter);
                        }, data);
}

} // namespace ampp
} // namespace rfim
} // namespace modules
} // namespace cheetah
} // namespace ska