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
#include "cheetah/modules/ddtr/cpu/DedispersionPlan.h"

namespace ska {
namespace cheetah {
namespace modules {
namespace ddtr {
namespace cpu {

template <typename DdtrTraits>
DedispersionPlan<DdtrTraits>::DedispersionPlan(ConfigType const& config, std::size_t memory)
    : _algo_config(config)
    , _memory(memory)
{
    if(config.dm_trials().size()==0)
    {
        throw panda::Error("ddtr::cpu Please specify DM ranges to dedisperse");
    }
}

template <typename DdtrTraits>
DedispersionPlan<DdtrTraits>::~DedispersionPlan()
{
}

template <typename DdtrTraits>
data::DimensionSize<data::Time> DedispersionPlan<DdtrTraits>::reset(TimeFrequencyType const& data)
{
    _strategy = std::make_shared<DedispersionStrategy<DdtrTraits>>(data, _algo_config, _memory);
    _dm_trial_metadata = this->generate_dmtrials_metadata(data.sample_interval(), _strategy->number_of_spectra(), _strategy->buffer_overlap());

    return _strategy->number_of_spectra();
}

template <typename DdtrTraits>
std::vector<std::pair<typename DedispersionPlan<DdtrTraits>::Dm, unsigned>> const& DedispersionPlan<DdtrTraits>::dm_trials() const
{
    return _algo_config.dm_trials();
}

template <typename DdtrTraits>
std::vector<double> const& DedispersionPlan<DdtrTraits>::dm_factors() const
{
    return _strategy->dm_factors();
}

template <typename DdtrTraits>
data::DimensionSize<data::Time> DedispersionPlan<DdtrTraits>::buffer_overlap() const
{
    return _strategy->buffer_overlap();
}

template <typename DdtrTraits>
void DedispersionPlan<DdtrTraits>::reset(data::DimensionSize<data::Time> const& spectra)
{
    _strategy->number_of_spectra(spectra);
}

template <typename DdtrTraits>
data::DimensionSize<data::Time> DedispersionPlan<DdtrTraits>::number_of_spectra() const
{
    return _strategy->number_of_spectra();
}

template <typename DdtrTraits>
std::shared_ptr<data::DmTrialsMetadata> DedispersionPlan<DdtrTraits>::generate_dmtrials_metadata(typename DedispersionPlan<DdtrTraits>::TimeType sample_interval, data::DimensionSize<data::Time> nspectra, std::size_t nsamples) const
{
    return _algo_config.generate_dmtrials_metadata(sample_interval, nspectra, nsamples);
}

template <typename DdtrTraits>
std::shared_ptr<data::DmTrialsMetadata> DedispersionPlan<DdtrTraits>::dm_trials_metadata(data::TimeFrequencyMetadata const& tf_info, data::DimensionSize<data::Time> number_of_spectra)
{
    if (number_of_spectra != _strategy->number_of_spectra()) {
        _dm_trial_metadata = this->generate_dmtrials_metadata(tf_info.sample_interval(), number_of_spectra, _strategy->buffer_overlap());
        _strategy->number_of_spectra(number_of_spectra);
    }
    return _dm_trial_metadata;

}

template <typename DdtrTraits>
typename DedispersionPlan<DdtrTraits>::ConfigType const& DedispersionPlan<DdtrTraits>::algo_config() const
{
    return _algo_config;
}

} // namespace cpu
} // namespace ddtr
} // namespace modules
} // namespace cheetah
} // namespace ska
