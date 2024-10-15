/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016-2022 The SKA organisation
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
#include "cheetah/modules/ddtr/klotski/detail/KlotskiConstraints.h"
#include "cheetah/cuda_utils/cuda_errorhandling.h"
#include "cheetah/data/TimeFrequency.h"
#include "cheetah/data/Units.h"
#include "panda/Resource.h"
#include "panda/Log.h"
#include <memory>
#include <algorithm>
#include <limits>

namespace ska {
namespace cheetah {
namespace modules {
namespace ddtr {
namespace klotski{

template<typename NumericalRep>
void DedispersionStrategy<NumericalRep>::update_details( int ndms
                                                        , int& count
                                                        , std::vector<unsigned int>& base
                                                        , std::vector<unsigned int>& shifts
                                                        , std::vector<unsigned int>& index
                                                        , float dm_shift
                                                        , std::vector<float>& dm_shift_excess)
{
    std::vector<int> temp(count);

    for(int i=0; i<count; ++i)
    {
        temp[i] = index[i];
    }
    int current_shift = -1;
    int new_count = 0;
    int base_count = 0;
    for(int dm_index=0; dm_index<ndms; ++dm_index)
    {
        int shift = (int)(dm_shift*dm_index+dm_shift_excess[dm_index]);
        bool value = false;
        if((unsigned)base_count<temp.size()) value = (temp[base_count]==dm_index);

        if(shift!=current_shift || value)
        {
            for(int c=0; c<count; ++c)
            {
                if(temp[c]<=dm_index && (temp[c+1]>dm_index || c==count-1))
                {
                    base[new_count] = temp[c];
                    index[new_count] = dm_index;
                    shifts[new_count] = shift;
                    c = count+10;

                    if(temp[base_count]==dm_index) base_count++;
                    new_count++;
                    current_shift = shift;
                }
            }

        }
    }
    count=new_count;
}

template<typename NumericalRep>
void DedispersionStrategy<NumericalRep>::generate_details_stack(FloatArrayType const& shifts_per_dm
                                                                , int ndms
                                                                , IntArrayType& total_base
                                                                , IntArrayType& total_index
                                                                , IntArrayType& total_shift
                                                                , IntArrayType& counts_array
                                                                , FloatArrayType& excess_per_klotski
                                                                , unsigned int band
                                                                , unsigned int klotski
                                                                , unsigned int range
                                                                )
{
    int count=1;

    std::vector<unsigned int> base(ndms);
    std::vector<unsigned int> index(ndms);
    std::vector<unsigned int> shift(ndms);
    std::vector<float> excess(ndms);

    for(int dm_index=0; dm_index<ndms; ++dm_index)
    {
        excess[dm_index] = excess_per_klotski[range][band][klotski][dm_index];
    }

    for(std::size_t chan_idx=0; chan_idx<shifts_per_dm[range][band][klotski].size(); ++chan_idx)
    {
        update_details(ndms, count, base, shift, index, shifts_per_dm[range][band][klotski][chan_idx], excess);
        counts_array[range][band][klotski].push_back(count);

        for(int c=0; c<count; ++c)
        {
            total_base[range][band][klotski].push_back(base[c]);
            total_index[range][band][klotski].push_back(index[c]);
            total_shift[range][band][klotski].push_back(shift[c]);
        }
    }
    std::size_t total_size=0;
    for(unsigned i=0; i<counts_array[range][band][klotski].size(); ++i)
    {
        total_size+=counts_array[range][band][klotski][i];
    }
}

template<typename NumericalRep>
DedispersionStrategy<NumericalRep>::DedispersionStrategy(const data::TimeFrequency<Cpu,NumericalRep>& chunk
                                                              , const ddtr::Config& config
                                                              , std::size_t cpu_memory)
{
    _nsamps = config.dedispersion_samples();
    _max_channels_per_band = config.klotski_algo_config().max_channels_per_band();
    _precise = config.klotski_algo_config().precise();
    _nchans = chunk.number_of_channels();
    auto t = chunk.low_high_frequencies();
    _fch1 = t.second;
    _foff = (t.second-t.first)/((double)(_nchans-1));
    _dm_constant = config.dm_constant();
    _max_channels_per_klotski = config.klotski_algo_config().max_channels_per_klotski();
    _cache_size = config.klotski_algo_config().cache_size();
    _number_of_dmtrials_samples = _nsamps;

    for(auto it = config.begin_range(); it!=config.end_range(); ++it)
    {
        _dm_low.push_back(it->dm_start());
        _dm_high.push_back(it->dm_end());
        _dm_step.push_back(it->dm_step());
    }
    _number_of_dm_ranges = _dm_low.size();

    _start_dm_shifts.resize(_number_of_dm_ranges);


    _tsamp.resize(_number_of_dm_ranges);
    for(unsigned int range=0; range<_number_of_dm_ranges; ++range)
    {
        unsigned int downsampling_factor = std::pow(2,range);
        _tsamp[range] = (chunk.sample_interval().value()*downsampling_factor*data::seconds);
    }

    _total_base.resize(_number_of_dm_ranges);
    _total_index.resize(_number_of_dm_ranges);
    _total_shift.resize(_number_of_dm_ranges);
    _counts_array.resize(_number_of_dm_ranges);
    _dmshifts_per_channel.resize(_number_of_dm_ranges);
    _dmshifts_per_band.resize(_number_of_dm_ranges);
    _dmshifts_per_klotski.resize(_number_of_dm_ranges);
    _dsamps_per_klotski.resize(_number_of_dm_ranges);
    _dmshifts_per_klotski_excess.resize(_number_of_dm_ranges);
    make_strategy(cpu_memory);

}

template<typename NumericalRep>
DedispersionStrategy<NumericalRep>::~DedispersionStrategy()
{
}




template<typename NumericalRep>
void DedispersionStrategy<NumericalRep>::resize(size_t const number_of_samples, size_t const cpu_memory)
{
    if(number_of_samples == (std::size_t)_nsamps)
    {
        return;
    }
    _nsamps = number_of_samples;

    make_strategy(cpu_memory);

}

template<typename NumericalRep>
void DedispersionStrategy<NumericalRep>::make_strategy(size_t const cpu_memory)
{
    _cpu_memory = cpu_memory;
    _number_of_bands = _nchans/_max_channels_per_band;
    if(_nchans%_max_channels_per_band!=0) _number_of_bands+=1;

    _ndms.resize(_number_of_dm_ranges,0);
    _klotskis_per_band.resize(_number_of_bands);
    _channels_per_band.resize(_number_of_bands);

    for(unsigned int range=0; range<_number_of_dm_ranges; ++range)
    {
        _dmshifts_per_band[range].resize(_number_of_bands);
        _dmshifts_per_klotski[range].resize(_number_of_bands);
        _dmshifts_per_channel[range].resize(_number_of_bands);
        _dmshifts_per_klotski_excess[range].resize(_number_of_bands);
        _total_base[range].resize(_number_of_bands);
        _total_index[range].resize(_number_of_bands);
        _total_shift[range].resize(_number_of_bands);
        _counts_array[range].resize(_number_of_bands);
        _dsamps_per_klotski[range].resize(_number_of_bands);
    }

    for(unsigned int range=0; range<_number_of_dm_ranges; ++range)
    {
        _ndms[range] = (_dm_high[range]-_dm_low[range])/_dm_step[range];
    }

    for(unsigned int band=0; band<_number_of_bands; ++band)
    {
        unsigned int count= band*_max_channels_per_band;
        if(count<_nchans) _channels_per_band[band] = _max_channels_per_band;
        if(band==_number_of_bands-1 && _nchans%_max_channels_per_band!=0) _channels_per_band[band] =  _nchans%_max_channels_per_band;
    }

    std::vector<float> fch1_per_band(_number_of_bands);
    std::size_t temp_channels=0;
    for(unsigned int band=0; band<_number_of_bands; ++band)
    {
        fch1_per_band[band] = _fch1.value()-temp_channels*_foff.value();
        temp_channels += _channels_per_band[band];
    }

    for(unsigned int band=0; band<_number_of_bands; ++band)
    {
        _klotskis_per_band[band] = _channels_per_band[band]/_max_channels_per_klotski;
        if(_channels_per_band[band]%_max_channels_per_klotski!=0) _klotskis_per_band[band]+=1;
    }

    for(unsigned int range=0; range<_number_of_dm_ranges; ++range)
    {
        for(unsigned int band=0; band<_number_of_bands; ++band)
        {
            _dmshifts_per_band[range][band].resize(_channels_per_band[band]);
            _total_base[range][band].resize(_klotskis_per_band[band]);
            _total_index[range][band].resize(_klotskis_per_band[band]);
            _total_shift[range][band].resize(_klotskis_per_band[band]);
            _counts_array[range][band].resize(_klotskis_per_band[band]);
            _dsamps_per_klotski[range][band].resize(_klotskis_per_band[band]);
            _dmshifts_per_klotski[range][band].resize(_klotskis_per_band[band]);
            _dmshifts_per_channel[range][band].resize(_klotskis_per_band[band]);
            _dmshifts_per_klotski_excess[range][band].resize(_klotskis_per_band[band]);
            for(unsigned int klotski=0; klotski<_klotskis_per_band[band]; ++klotski) _dmshifts_per_klotski[range][band][klotski].resize(_ndms[range]);
        }
    }

    for(unsigned int range=0; range<_number_of_dm_ranges; ++range)
    {
        unsigned int start_channel_of_band = 0;
        for(unsigned int band=0; band<_number_of_bands; ++band)
        {

            float temp_shift = _dm_step[range].value()*_dm_constant.value()*((1.0/((_fch1.value() - start_channel_of_band*_foff.value())*(_fch1.value() - start_channel_of_band*_foff.value())))-(1.0/(_fch1.value()*_fch1.value())))/_tsamp[range].value();

            _dmshifts_per_band[range][band].resize(_ndms[range]);
            for(unsigned int dm_index=0; dm_index<_ndms[range]; ++dm_index)
            {
                _dmshifts_per_band[range][band][dm_index] = (unsigned int)(temp_shift*dm_index);
            }

            for(std::size_t klotski=0; klotski<_klotskis_per_band[band]; ++klotski)
            {
                unsigned int start_channel = start_channel_of_band+_max_channels_per_klotski*klotski;
                unsigned int channels_per_klotski = _max_channels_per_klotski;
                if(klotski==_klotskis_per_band[band]-1 && _channels_per_band[band]%_max_channels_per_klotski != 0) channels_per_klotski=_channels_per_band[band]%_max_channels_per_klotski;

                float fch1_klotski = _fch1.value() - start_channel*_foff.value();
                float fch1_value = _fch1.value();

                float temp_shift = _dm_step[range].value()*_dm_constant.value()*(1.0/((fch1_klotski)*(fch1_klotski))-1.0/(fch1_value*fch1_value))/_tsamp[range].value();
                _dmshifts_per_klotski[range][band][klotski].resize(_ndms[range]);
                _dmshifts_per_klotski_excess[range][band][klotski].resize(_ndms[range]);

                _dmshifts_per_channel[range][band][klotski].resize(channels_per_klotski);
                std::vector<float> temp_shift_channel(channels_per_klotski);
                for(unsigned int channel=0; channel<channels_per_klotski; ++channel)
                {
                    _dmshifts_per_channel[range][band][klotski][channel] = _dm_step[range].value()*_dm_constant.value()*(1.0/((fch1_klotski-_foff.value()*channel)*(fch1_klotski-_foff.value()*channel))-1.0/(fch1_klotski*fch1_klotski))/_tsamp[range].value();
                    temp_shift_channel[channel] = _dm_step[range].value()*_dm_constant.value()*(1.0/((fch1_klotski-_foff.value()*channel)*(fch1_klotski-_foff.value()*channel))-1.0/(_fch1.value()*_fch1.value()))/_tsamp[range].value();


                    if((_ndms[range]*_dmshifts_per_channel[range][band][klotski][channel])>64.0)
                    {
                        throw panda::Error("reduce the number of channels per klotski");
                    }
                }

                for(unsigned int dm_index=0; dm_index<_ndms[range]; ++dm_index)
                {
                    float excess = 0.0;
                    for(unsigned int channel=0; channel<channels_per_klotski; ++channel)
                    {
                        excess += std::floor(temp_shift_channel[channel]*dm_index)-std::floor(temp_shift*dm_index)-std::floor(_dmshifts_per_channel[range][band][klotski][channel]*dm_index);
                    }
                    excess /= (float)channels_per_klotski;
                    _dmshifts_per_klotski[range][band][klotski][dm_index] = std::floor(temp_shift*dm_index+excess);
                }


            }
            start_channel_of_band += _channels_per_band[band];
        }
    }

    for(unsigned int range=0; range<_number_of_dm_ranges; ++range)
    {
        for(unsigned int band=0; band<_number_of_bands; ++band)
        {
            for(unsigned int klotski=0; klotski<_klotskis_per_band[band]; ++klotski)
            {
                generate_details_stack( _dmshifts_per_channel
                              , (int)_ndms[range]
                              , _total_base
                              , _total_index
                              , _total_shift
                              , _counts_array
                              , _dmshifts_per_klotski_excess
                              , band
                              , klotski
                              , range
                              );
            }
        }
    }

    float temp_shift = _dm_constant.value()*(1.0/((_fch1.value()-_foff.value()*_nchans)*(_fch1.value()-_foff.value()*_nchans))-1.0/(_fch1.value()*_fch1.value()))/_tsamp[0].value();
    _maxshift = temp_shift*_dm_high[_number_of_dm_ranges-1].value();
    _maxshift = (std::ceil((double)_maxshift/(double)(KlotskiConstraints::minimum_overlap))+1)*KlotskiConstraints::minimum_overlap;
    if(_maxshift==0) _maxshift = KlotskiConstraints::minimum_overlap;
    if(_nsamps<2*_maxshift)
    throw panda::Error("Memory is less than required to perform DDTR");

    _nsamps = std::ceil((double)_nsamps/(double)(KlotskiConstraints::minimum_overlap))*KlotskiConstraints::minimum_overlap;
    _dedispersed_samples = _nsamps - _maxshift;

    for(unsigned int range=0; range<_number_of_dm_ranges; ++range)
    {
        for(unsigned int band=0; band<_number_of_bands; ++band)
        {
            int dshift = _dmshifts_per_band[range][band][_ndms[range]-1];
            dshift = KlotskiConstraints::minimum_overlap*(dshift/KlotskiConstraints::minimum_overlap+1);
            unsigned int temp_samps = _dedispersed_samples/std::pow(2,range) + dshift;
            if(band==0) temp_samps = _dedispersed_samples/std::pow(2,range);
            for(unsigned int klotski=0; klotski<_klotskis_per_band[band]; ++klotski)
            {
                int shift = _dmshifts_per_klotski[range][band][klotski][_ndms[range]-1];
                if(klotski>0) shift = KlotskiConstraints::minimum_overlap*(shift/KlotskiConstraints::minimum_overlap+1);
                else shift = 0;
                _dsamps_per_klotski[range][band][klotski] = temp_samps+shift;
            }
        }
    }

    for(unsigned int range=0; range<_number_of_dm_ranges; ++range)
    {
        _start_dm_shifts[range].resize(_nchans);
        for(unsigned int channel=0; channel<_nchans; ++channel)
        {
            _start_dm_shifts[range][channel] = (unsigned int)(_dm_low[range].value()*_dm_constant.value()*(1.0/((_fch1.value()-_foff.value()*channel)*(_fch1.value()-_foff.value()*channel))-1.0/(_fch1.value()*_fch1.value()))/_tsamp[range].value());
        }
    }

    _number_of_dmtrials_samples = _dsamps_per_klotski[0][_number_of_bands-1][_klotskis_per_band[_number_of_bands-1]-1];
    _temp_work_area = std::make_shared<std::vector<unsigned char>>(_nsamps*_nchans);

    _subanded_dm_trials = std::make_shared<std::vector<std::vector<int>>>(_number_of_bands, std::vector<int>(_number_of_dmtrials_samples*_ndms[0]));
}

template <typename NumericalRep>
std::vector<std::vector<unsigned int>> const& DedispersionStrategy<NumericalRep>::start_dm_shifts() const
{
    return _start_dm_shifts;
}

template <typename NumericalRep>
unsigned int DedispersionStrategy<NumericalRep>::max_channels_per_band() const
{
    return _max_channels_per_band;
}

template <typename NumericalRep>
unsigned int DedispersionStrategy<NumericalRep>::number_of_bands() const
{
    return _number_of_bands;
}

template <typename NumericalRep>
std::vector<unsigned int> const& DedispersionStrategy<NumericalRep>::channels_per_band() const
{
    return _channels_per_band;
}

template <typename NumericalRep>
std::vector<unsigned int> const& DedispersionStrategy<NumericalRep>::klotskis_per_band() const
{
    return _klotskis_per_band;
}

template <typename NumericalRep>
std::vector<std::vector<std::vector<unsigned int>>>& DedispersionStrategy<NumericalRep>::dmshifts_per_band()
{
    return _dmshifts_per_band;
}

template <typename NumericalRep>
typename DedispersionStrategy<NumericalRep>::IntArrayType
DedispersionStrategy<NumericalRep>::dmshifts_per_klotski() const
{
    return _dmshifts_per_klotski;
}

template <typename NumericalRep>
std::vector<std::vector<std::vector<unsigned int>>> const& DedispersionStrategy<NumericalRep>::dsamps_per_klotski() const
{
    return _dsamps_per_klotski;
}

template <typename NumericalRep>
std::shared_ptr<std::vector<unsigned char>> DedispersionStrategy<NumericalRep>::temp_work_area()
{
    return _temp_work_area;
}

template <typename NumericalRep>
std::shared_ptr<std::vector<std::vector<int>>> DedispersionStrategy<NumericalRep>::subanded_dm_trials()
{
    return _subanded_dm_trials;
}

template <typename NumericalRep>
typename DedispersionStrategy<NumericalRep>::IntArrayType DedispersionStrategy<NumericalRep>::total_base() const
{
    return _total_base;
}

template <typename NumericalRep>
typename DedispersionStrategy<NumericalRep>::IntArrayType DedispersionStrategy<NumericalRep>::total_index() const
{
    return _total_index;
}

template <typename NumericalRep>
typename DedispersionStrategy<NumericalRep>::IntArrayType DedispersionStrategy<NumericalRep>::total_shift() const
{
    return _total_shift;
}

template <typename NumericalRep>
typename DedispersionStrategy<NumericalRep>::IntArrayType DedispersionStrategy<NumericalRep>::counts_array() const
{
    return _counts_array;
}

template <typename NumericalRep>
typename DedispersionStrategy<NumericalRep>::FloatArrayType DedispersionStrategy<NumericalRep>::dmshifts_per_channel() const
{
    return _dmshifts_per_channel;
}

template <typename NumericalRep>
unsigned int DedispersionStrategy<NumericalRep>::number_of_dm_ranges() const
{
    return _number_of_dm_ranges;
}

template <typename NumericalRep>
unsigned int DedispersionStrategy<NumericalRep>::max_channels_per_klotski() const
{
    return _max_channels_per_klotski;
}

template <typename NumericalRep>
std::vector<typename DedispersionStrategy<NumericalRep>::Dm> const& DedispersionStrategy<NumericalRep>::dm_low() const
{
    return _dm_low;
}

template <typename NumericalRep>
std::vector<typename DedispersionStrategy<NumericalRep>::Dm> const& DedispersionStrategy<NumericalRep>::dm_high() const
{
    return _dm_high;
}

template <typename NumericalRep>
std::vector<typename DedispersionStrategy<NumericalRep>::Dm> const& DedispersionStrategy<NumericalRep>::dm_step() const
{
    return _dm_step;
}

template <typename NumericalRep>
unsigned int DedispersionStrategy<NumericalRep>::maxshift() const
{
    return _maxshift;
}

template <typename NumericalRep>
std::vector<unsigned int> const& DedispersionStrategy<NumericalRep>::ndms() const
{
    return _ndms;
}

template <typename NumericalRep>
unsigned int DedispersionStrategy<NumericalRep>::total_ndms() const
{
    return _total_ndms;
}

template <typename NumericalRep>
typename DedispersionStrategy<NumericalRep>::Dm const& DedispersionStrategy<NumericalRep>::max_dm() const
{
    return _max_dm;
}

template <typename NumericalRep>
std::vector<typename DedispersionStrategy<NumericalRep>::TimeType> DedispersionStrategy<NumericalRep>::tsamp() const
{
    return _tsamp;
}

template <typename NumericalRep>
std::size_t DedispersionStrategy<NumericalRep>::cache_size() const
{
    return _cache_size;
}

template <typename NumericalRep>
unsigned int DedispersionStrategy<NumericalRep>::nsamps() const
{
    return _nsamps;
}

template <typename NumericalRep>
unsigned int DedispersionStrategy<NumericalRep>::nchans() const
{
    return _nchans;
}

template <typename NumericalRep>
typename DedispersionStrategy<NumericalRep>::FrequencyType DedispersionStrategy<NumericalRep>::fch1() const
{
    return _fch1;
}

template <typename NumericalRep>
typename DedispersionStrategy<NumericalRep>::FrequencyType DedispersionStrategy<NumericalRep>::foff() const
{
    return _foff;
}

template <typename NumericalRep>
typename data::DimensionSize<data::Time> DedispersionStrategy<NumericalRep>::dedispersed_samples() const
{
    return data::DimensionSize<data::Time>(_dedispersed_samples);
}

template <typename NumericalRep>
unsigned int const& DedispersionStrategy<NumericalRep>::number_of_dmtrials_samples() const
{
    return _number_of_dmtrials_samples;
}

template <typename NumericalRep>
utils::MultiThread& DedispersionStrategy<NumericalRep>::ddtr_threads()
{
    return _ddtr_threads;
}

} // namespace klotski
} // namespace ddtr
} // namespace modules
} // namespace cheetah
} // namespace ska
