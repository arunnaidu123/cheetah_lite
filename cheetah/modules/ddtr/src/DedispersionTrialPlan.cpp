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
#include "cheetah/modules/ddtr/DedispersionTrialPlan.h"


namespace ska {
namespace cheetah {
namespace modules {
namespace ddtr {

static std::string const& dedispersion_tag()
{
	static std::string var("dedispersion");
	return var;
}

DedispersionTrialPlan::DedispersionTrialPlan(std::string const& tag_name)
    : BaseT(tag_name)
    , _dm_constant(data::dm_constant::s_mhz::dm_constant)
    , _max_dm(0.0 * data::parsecs_per_cube_cm)
    , _dedispersion_samples(0) // should be set in the config
{
    add_factory(dedispersion_tag(), []()
    {
        return new ddtr::DedispersionConfig();
    });
}

DedispersionTrialPlan::~DedispersionTrialPlan()
{
}

void DedispersionTrialPlan::add_options(OptionsDescriptionEasyInit& add_options)
{
    add_options
    ("dedispersion_samples", boost::program_options::value<std::size_t>(&_dedispersion_samples)->
        default_value(_dedispersion_samples), "the maximum number of samples to process in each call to the dedisperser (may be less depending on chosen algorithm constraints)")
    ("dm_constant", boost::program_options::value<double>()->default_value(_dm_constant.value())->notifier([this](double v) { _dm_constant = v * data::dm_constant::s_mhz_squared_cm_cubed_per_pc; }), "the dedispersion constant to use (in MHz^2 sec cm^3 per parsec");
}

std::vector<std::pair<DedispersionTrialPlan::Dm, unsigned>> const& DedispersionTrialPlan::dm_trials() const
{
    if(_dm_trials.empty()) {
        auto it = subsection(dedispersion_tag());
        unsigned df = 1;
        while (it != subsection_end())
        {
            ddtr::DedispersionConfig const& c = static_cast<ddtr::DedispersionConfig const&>(*it);
            unsigned count=0;
            for (Dm dm = c.dm_start(); dm < c.dm_end(); dm += c.dm_step())
            {
                if(dm > _max_dm) _max_dm = dm;
               _dm_trials.push_back(std::make_pair(dm, df));
               count++;
            }
            _number_of_dms.push_back(count);
            df = 1<<df;
            ++it;
        }
    }
    return _dm_trials;
}

void DedispersionTrialPlan::add_dm_range(Dm start, Dm end, Dm step)
{
    DedispersionConfig& dm_config = static_cast<DedispersionConfig&>(*generate_section(dedispersion_tag()));
    dm_config.dm_start(start);
    dm_config.dm_end(end);
    dm_config.dm_step(step);
    _dm_trials.clear();
}

void DedispersionTrialPlan::dedispersion_config(DedispersionConfig& config)
{
    add(dedispersion_tag(), config);
    _dm_trials.clear();
}

std::shared_ptr<data::DmTrialsMetadata> DedispersionTrialPlan::generate_dmtrials_metadata(TimeType sample_interval, std::size_t nspectra, std::size_t overlap) const
{
    if (nspectra < overlap) {
        panda::Error e("Overlap exceeds number of spectra: ");
        e << overlap << " > " << nspectra;
        throw e;
    }
    std::shared_ptr<data::DmTrialsMetadata> meta_data(new data::DmTrialsMetadata(sample_interval, nspectra - overlap));
    for (auto dm : dm_trials())
    {
        meta_data->emplace_back(dm.first, dm.second);
    }
    return meta_data;
}

DedispersionTrialPlan::DmConstantType DedispersionTrialPlan::dm_constant() const
{
    return _dm_constant;
}

DedispersionTrialPlan::Dm DedispersionTrialPlan::max_dm() const
{
    dm_trials();
    return _max_dm;
}

void DedispersionTrialPlan::dm_constant(DedispersionTrialPlan::DmConstantType dm_const)
{
    _dm_constant = dm_const;
}

typename DedispersionTrialPlan::TimeType DedispersionTrialPlan::maximum_delay(FrequencyType freq_low, FrequencyType freq_high) const
{
    dm_trials();
    return TimeType((this->dm_constant().value() * (1.0/(freq_low*freq_low) -  1.0/(freq_high*freq_high)) * _max_dm).value() * boost::units::si::second);
}

DedispersionTrialPlan::RangeIterator DedispersionTrialPlan::begin_range() const
{
    return this->subsection(dedispersion_tag());
}

DedispersionTrialPlan::RangeIterator DedispersionTrialPlan::end_range() const
{
    return this->subsection_end();
}

std::size_t DedispersionTrialPlan::dedispersion_samples() const
{
    return _dedispersion_samples;
}

void DedispersionTrialPlan::dedispersion_samples(std::size_t n)
{
    _dedispersion_samples = n;
}

std::vector<std::size_t> const& DedispersionTrialPlan::number_of_dms() const
{
    return _number_of_dms;
}

} // namespace ddtr
} // namespace modules
} // namespace cheetah
} // namespace ska
