/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016-2023 The SKA organisation
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

#include "cheetah/modules/ddtr/klotski_bruteforce/detail/KlotskiBruteforceConstraints.h"
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
namespace klotski_bruteforce{

template<typename NumericalRep>
DedispersionStrategy<NumericalRep>::DedispersionStrategy(const data::TimeFrequency<Cpu,NumericalRep>& chunk
                                                              , const ddtr::Config& config
                                                              , std::size_t cpu_memory)
{
    _nsamps = config.dedispersion_samples();
    _tsamp = chunk.sample_interval();
    _nchans = chunk.number_of_channels();
    auto t = chunk.low_high_frequencies();
    _fch1 = t.second;
    _foff = (t.second-t.first)/((double)(_nchans-1));
    _dm_constant = config.dm_constant();
    _kchans = config.klotski_bruteforce_algo_config().max_channels_per_klotski_bruteforce();
    _cache_size = config.klotski_bruteforce_algo_config().cache_size();
    unsigned int bin=0;
    for(auto it = config.begin_range(); it!=config.end_range(); ++it)
    {
        _dm_low.push_back(it->dm_start());
        _dm_high.push_back(it->dm_end());
        _dm_step.push_back(it->dm_step());
        _in_bin.push_back(bin++);
    }
    _number_of_dm_ranges = _dm_low.size();
    make_strategy(cpu_memory);
}

template<typename NumericalRep>
DedispersionStrategy<NumericalRep>::~DedispersionStrategy()
{
}

template<typename NumericalRep>
unsigned int DedispersionStrategy<NumericalRep>::number_of_dm_ranges() const
{
    return _number_of_dm_ranges;
}

template<typename NumericalRep>
std::vector<unsigned int> const& DedispersionStrategy<NumericalRep>::in_bin() const
{
    return _in_bin;
}

template<typename NumericalRep>
unsigned int DedispersionStrategy<NumericalRep>::maxshift() const
{
    return _maxshift;
}

template<typename NumericalRep>
std::vector<typename DedispersionStrategy<NumericalRep>::Dm> const& DedispersionStrategy<NumericalRep>::dm_low() const
{
    return _dm_low;
}

template<typename NumericalRep>
std::vector<typename DedispersionStrategy<NumericalRep>::Dm> const& DedispersionStrategy<NumericalRep>::dm_high() const
{
    return _dm_high;
}

template<typename NumericalRep>
std::vector<typename DedispersionStrategy<NumericalRep>::Dm> const& DedispersionStrategy<NumericalRep>::dm_step() const
{
    return _dm_step;
}

template<typename NumericalRep>
std::vector<float> const& DedispersionStrategy<NumericalRep>::dmshifts() const
{
    return _dmshifts;
}

template<typename NumericalRep>
std::vector<std::vector<unsigned int>> const& DedispersionStrategy<NumericalRep>::start_dmshifts() const
{
    return _start_dmshifts;
}

template<typename NumericalRep>
std::vector<unsigned int> const& DedispersionStrategy<NumericalRep>::ndms() const
{
    return _ndms;
}

template<typename NumericalRep>
unsigned int DedispersionStrategy<NumericalRep>::total_ndms() const
{
    return _total_ndms;
}

template<typename NumericalRep>
typename DedispersionStrategy<NumericalRep>::Dm DedispersionStrategy<NumericalRep>::max_dm() const
{
    return _max_dm;
}

template<typename NumericalRep>
typename DedispersionStrategy<NumericalRep>::TimeType DedispersionStrategy<NumericalRep>::tsamp() const
{
    return _tsamp;
}

template<typename NumericalRep>
unsigned int DedispersionStrategy<NumericalRep>::nsamps() const
{
    return _nsamps;
}

template<typename NumericalRep>
typename DedispersionStrategy<NumericalRep>::ArrayType const& DedispersionStrategy<NumericalRep>::kloskis_per_band() const
{
    return _kloskis_per_band;
}

template<typename NumericalRep>
unsigned int DedispersionStrategy<NumericalRep>::nchans() const
{
    return _nchans;
}

template<typename NumericalRep>
unsigned int DedispersionStrategy<NumericalRep>::kchans() const
{
    return _kchans;
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
    _maxshift = 0;
    _ndms.resize(_number_of_dm_ranges,0);
    _dmshifts.resize(_nchans,0);
    _start_dmshifts.resize(_number_of_dm_ranges);
    _kloskis_per_band.resize(_number_of_dm_ranges);

    for(unsigned int i=0; i<_number_of_dm_ranges; ++i)
    {
        _ndms[i] = (_dm_high[i]-_dm_low[i])/_dm_step[i];
        _start_dmshifts[i].resize(_nchans);
    }

    for (unsigned int c = 0; c < _nchans; ++c)
    {
        _dmshifts[c] = std::abs((float)(_dm_constant.value() * ((1.0 / pow(_fch1.value()-_foff.value()*c, 2.0f )) - (1.0 / pow(_fch1.value(), 2.0f))))/_tsamp.value());
    }

    for(unsigned int i=0; i<_number_of_dm_ranges; ++i)
    {
        for(unsigned int channel=0; channel<_nchans; ++channel)
        {
            _start_dmshifts[i][channel] = (unsigned int)(_dmshifts[channel]*_dm_low[i].value()/((float)std::pow(2,i)));
        }
    }

    std::size_t total_bands = std::ceil((float)_nchans/(float)_kchans);

    for(std::size_t range=0; range<_number_of_dm_ranges; ++range)
    {
        for(std::size_t band=0; band<total_bands; ++band)
        {
            std::vector<unsigned> nbands;
            unsigned max_channels_per_subband =_kchans;
            if(_nchans%_kchans != 0 && band==total_bands-1)
            {
                max_channels_per_subband = _nchans%_kchans;
            }

            unsigned i=0;
            while(i!=max_channels_per_subband)
            {
                unsigned tnchans =max_channels_per_subband;
                unsigned flag=0;

                while(flag==0)
                {
                    if(tnchans<4 || tnchans%4!=0) // limit of klotski_bruteforce algorithm we need to have atleast 4 channels for this to work
                    {
                        throw panda::Error("number of channels should be a multiple of 4");
                    }

                    if(_dmshifts[band*_kchans+tnchans-1]*_ndms[range]*_dm_step[range].value()*tnchans<_cache_size)
                    {
                        nbands.push_back(tnchans);
                        flag=1;
                        i += tnchans;
                    }
                    else
                    {
                        tnchans = pow(2,std::ceil(log2(tnchans)))/2;
                    }
                }
            }
            _kloskis_per_band[range].push_back(nbands);
        }
    }

    _maxshift = _dmshifts[_nchans-1]*_dm_high[_number_of_dm_ranges-1].value();
    _maxshift = std::ceil(((double)_maxshift/(double)(klotskiBruteforceConstraints::minimum_overlap))+1)*klotskiBruteforceConstraints::minimum_overlap;

    _nsamps = std::ceil((double)_nsamps/(double)(klotskiBruteforceConstraints::minimum_overlap))*klotskiBruteforceConstraints::minimum_overlap;
    _dedispersed_time_samples = _nsamps - _maxshift;

    _total_ndms = 0;
    for(unsigned int i=0; i<_number_of_dm_ranges; ++i)
    {
        _ndms[i] = std::floor((_dm_high[i]-_dm_low[i])/_dm_step[i]);
        _total_ndms += _ndms[i];
    }

    std::size_t total_size = _total_ndms*_dedispersed_time_samples*sizeof(float) + sizeof(NumericalRep)*_nsamps;

    if(total_size > 0.75*_cpu_memory)
    {
        PANDA_LOG_ERROR << "Cpu memory is insufficient please check the search parameters. required: "
                        << total_size << " available: " << _cpu_memory;
        throw panda::Error("Cpu is not compatible for the selected dedispersion plan");
    }

    _temp_work_area = std::make_shared<std::vector<unsigned short>>(_nsamps*_nchans);

    _subanded_dm_trials = std::make_shared<std::vector<std::vector<int>>>(_kloskis_per_band[0].size());
    for(unsigned int i=0; i<_kloskis_per_band[0].size(); ++i) (*_subanded_dm_trials)[i].resize(_dedispersed_time_samples*(*std::max_element(_ndms.begin(), _ndms.end())));
}

template<typename NumericalRep>
data::DimensionSize<data::Time> DedispersionStrategy<NumericalRep>::dedispersed_samples()
{
    return data::DimensionSize<data::Time>(_dedispersed_time_samples);
}

template <typename NumericalRep>
std::shared_ptr<std::vector<std::vector<int>>> DedispersionStrategy<NumericalRep>::subanded_dm_trials()
{
    return _subanded_dm_trials;
}

template <typename NumericalRep>
std::shared_ptr<std::vector<unsigned short>> DedispersionStrategy<NumericalRep>::temp_work_area()
{
    return _temp_work_area;
}

} // namespace klotski_bruteforce
} // namespace ddtr
} // namespace modules
} // namespace cheetah
} // namespace ska
