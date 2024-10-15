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
#include "cheetah/modules/ddtr/klotski/DedispersionPlan.h"

namespace ska {
namespace cheetah {
namespace modules {
namespace ddtr {
namespace klotski {

template <typename DdtrTraits>
DedispersionPlan<DdtrTraits>::DedispersionPlan(BeamConfigType const& beam_config, ConfigType const& config, std::size_t memory)
    : _beam_config(beam_config)
    , _config(config)
    , _memory(memory)
    , _max_delay(0)
    , _dedispersion_samples(0)
    , _number_of_spectra(0)
    , _current_dm_range(0)
{
}

template <typename DdtrTraits>
data::DimensionSize<data::Time> DedispersionPlan<DdtrTraits>::reset(TimeFrequencyType const& data)
{
    _strategy = std::make_shared<DedispersionStrategy<NumericalT>>(data, _config, _memory);

    _max_delay = _strategy->maxshift();
    FrequencyListType const& channel_freqs = data.channel_frequencies();
    auto freq_pair = data.low_high_frequencies();
    FrequencyType freq_top = freq_pair.second;

    auto const one_over_top_freq_squared = 1.0/(freq_top*freq_top);
    for (auto freq: channel_freqs)
    {
        double factor = (this->_config.dm_constant().value() * (1.0/(freq*freq) -  one_over_top_freq_squared) / data.sample_interval()).value();
        PANDA_LOG_DEBUG << "Frequency: " << freq << "  Reference: " << freq_top << "  DM constant: "
                        << this->_config.dm_constant() << "  Sampling interval: " << data.sample_interval() <<  "  DM factor: " << factor;
        _dm_factors.push_back(factor);
    }
    _number_of_spectra =  _strategy->dedispersed_samples() + _strategy->maxshift();

    if (_number_of_spectra < 2 * _max_delay)
    {
        PANDA_LOG_WARN << "Requested number of samples to dedisperse ("
                        << this->_config.dedispersion_samples()
                        << ") is less than twice the max dispersion delay ("
                        << 2 * _max_delay << "): Setting number of samples to dedisperse to "
                        << 2 * _max_delay;
        _number_of_spectra = 2 * _max_delay;
    }
    _dedispersion_samples = _number_of_spectra-_max_delay;
    _dm_trial_metadata = this->generate_dmtrials_metadata(data.sample_interval(), _dedispersion_samples);

    _dm_trials_ptr = DmTrialsType::make_shared(_dm_trial_metadata, data.start_time());
    _spdt_dm_trials_ptr = DmTrialsType::make_shared(_dm_trial_metadata, data.start_time());

    return data::DimensionSize<data::Time>(_number_of_spectra);
}

template <typename DdtrTraits>
std::shared_ptr<data::DmTrialsMetadata> DedispersionPlan<DdtrTraits>::generate_dmtrials_metadata(TimeType sample_interval, std::size_t nspectra) const
{
    if (nspectra < _strategy->maxshift()) {
        panda::Error e("Overlap exceeds number of spectra: ");
        e << _strategy->maxshift() << " > " << nspectra;
        throw e;
    }
    std::shared_ptr<data::DmTrialsMetadata> meta_data(new data::DmTrialsMetadata(sample_interval, nspectra));
    for( unsigned index=0; index<_strategy->number_of_dm_ranges(); ++index)
    {
        for (unsigned int dmindx=0; dmindx < _strategy->ndms()[index]; ++dmindx)
        {
            auto dm = _strategy->dm_low()[index].value() + dmindx*_strategy->dm_step()[index].value();
            meta_data->emplace_back(Dm(dm* data::parsecs_per_cube_cm), std::pow(2,index));
        }
    }
    return meta_data;
}

template <typename DdtrTraits>
data::DimensionSize<data::Time> DedispersionPlan<DdtrTraits>::buffer_overlap() const
{
    return data::DimensionSize<data::Time>(_max_delay);
}

template <typename DdtrTraits>
void DedispersionPlan<DdtrTraits>::reset(data::DimensionSize<data::Time> const& spectra)
{
    _number_of_spectra = spectra;
}

template <typename DdtrTraits>
data::DimensionSize<data::Time> DedispersionPlan<DdtrTraits>::number_of_spectra() const
{
    return data::DimensionSize<data::Time>(_number_of_spectra);
}

template <typename DdtrTraits>
std::shared_ptr<data::DmTrialsMetadata> DedispersionPlan<DdtrTraits>::dm_trial_metadata() const
{
    return _dm_trial_metadata;
}

template <typename DdtrTraits>
std::shared_ptr<DedispersionStrategy<typename DedispersionPlan<DdtrTraits>::NumericalT>> const& DedispersionPlan<DdtrTraits>::dedispersion_strategy() const
{
    return _strategy;
}

template <typename DdtrTraits>
std::shared_ptr<typename DdtrTraits::DmTrialsType> const& DedispersionPlan<DdtrTraits>::dm_trials()
{
    return _dm_trials_ptr;
}

template <typename DdtrTraits>
std::shared_ptr<typename DdtrTraits::DmTrialsType> const& DedispersionPlan<DdtrTraits>::spdt_dm_trials()
{
    return _spdt_dm_trials_ptr;
}

template <typename DdtrTraits>
std::vector<unsigned> const& DedispersionPlan<DdtrTraits>::affinities()
{
    return _beam_config.affinities();
}

template <typename DdtrTraits>
unsigned DedispersionPlan<DdtrTraits>::current_dm_range() const
{
    return _current_dm_range;
}

template <typename DdtrTraits>
void DedispersionPlan<DdtrTraits>::current_dm_range(unsigned val)
{
    _current_dm_range = val;
}

template <typename DdtrTraits>
std::string const& DedispersionPlan<DdtrTraits>::beam_id() const
{
    return _beam_config.id();
}

} // namespace klotski
} // namespace ddtr
} // namespace modules
} // namespace cheetah
} // namespace ska
