/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2023 The SKA organisation
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
#include "cheetah/modules/ddtr/cpu/DedispersionStrategy.h"

namespace ska {
namespace cheetah {
namespace modules {
namespace ddtr {
namespace cpu {

template <typename DdtrTraits>
DedispersionStrategy<DdtrTraits>::DedispersionStrategy(const TimeFrequencyType& chunk
                                                      , const ddtr::Config& config
                                                      , int const& memory)
    :    _memory(memory)
{
    for(auto it = config.begin_range(); it!=config.end_range(); ++it)
    {
        _user_dm_low.push_back(it->dm_start());
        _user_dm_high.push_back(it->dm_end());
        _user_dm_step.push_back(it->dm_step());
    }
    _number_of_dm_ranges = _user_dm_low.size();

    make_strategy(chunk, config);
    if(config.dm_trials().size()==0)
    {
        throw panda::Error("ddtr::cpu Please specify DM ranges to dedisperse");
    }
}

template <typename DdtrTraits>
DedispersionStrategy<DdtrTraits>::~DedispersionStrategy()
{
}

template <typename DdtrTraits>
data::DimensionSize<data::Time> DedispersionStrategy<DdtrTraits>::make_strategy(const TimeFrequencyType& chunk, const ddtr::Config& config)
{
    Dm max_dm = config.max_dm();

    FrequencyListType const& channel_freqs = chunk.channel_frequencies();
    auto freq_pair = chunk.low_high_frequencies();
    FrequencyType freq_top = freq_pair.second;
    FrequencyType freq_bottom = freq_pair.first;
    _max_delay = std::size_t((config.dm_constant().value()
                    * (1.0/(freq_bottom*freq_bottom) -  1.0/(freq_top*freq_top))
                    * max_dm / chunk.sample_interval()).value()) + 1;

    _max_delay = (_max_delay/1024 + 1)*1024;
    for (auto freq: channel_freqs)
    {
        double factor = (config.dm_constant().value() * (1.0/(freq*freq) -  1.0/(freq_top*freq_top)) / chunk.sample_interval()).value();
        _dm_factors.push_back(factor);
    }
    _number_of_spectra = std::min(_memory/chunk.number_of_channels(), config.dedispersion_samples());
    if (_number_of_spectra < _max_delay * 2)
    {
        PANDA_LOG_WARN << "Requested number of samples to dedisperse ("
                        << this->_number_of_spectra
                        << ") is less than twice the max dispersion delay ("
                        << 2 * _max_delay << ")";
    }

    return data::DimensionSize<data::Time>(_number_of_spectra);
}

template <typename DdtrTraits>
std::vector<double> const& DedispersionStrategy<DdtrTraits>::dm_factors() const
{
    return _dm_factors;
}

template <typename DdtrTraits>
data::DimensionSize<data::Time> DedispersionStrategy<DdtrTraits>::buffer_overlap() const
{
    return data::DimensionSize<data::Time>(_max_delay);
}

template <typename DdtrTraits>
data::DimensionSize<data::Time> DedispersionStrategy<DdtrTraits>::number_of_spectra() const
{
    return data::DimensionSize<data::Time>(_number_of_spectra);
}

template <typename DdtrTraits>
void DedispersionStrategy<DdtrTraits>::number_of_spectra(data::DimensionSize<data::Time> const& spectra)
{
    _number_of_spectra = spectra;
}

template <typename DdtrTraits>
std::size_t const& DedispersionStrategy<DdtrTraits>::number_of_dm_ranges() const
{
    return _number_of_dm_ranges;
}

} // namespace cpu
} // namespace ddtr
} // namespace modules
} // namespace cheetah
} // namespace ska