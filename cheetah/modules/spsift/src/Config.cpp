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
#include "cheetah/modules/spsift/Config.h"

namespace ska {
namespace cheetah {
namespace modules {
namespace spsift {


Config::Config()
    : utils::Config("spsift")
    , _pulse_width_threshold(MsecTimeType(10.0 * cheetah::data::milliseconds))
    , _sigma_threshold(0.0)
    , _dm_threshold(0.0 * pss::astrotypes::units::parsecs_per_cube_cm)
    , _maximum_candidates(0)
    , _active(true)
{
}

Config::~Config()
{
}

void Config::add_options(OptionsDescriptionEasyInit& add_options )
{
    add_options
    ("active", boost::program_options::value<bool>(&_active)->default_value(_active),
        "perform SpSift if true"
    )
    ("dm_thresh", boost::program_options::value<float>()->default_value(_dm_threshold.value())->notifier(
        [&](float f)
        {
            _dm_threshold = f * pss::astrotypes::units::parsecs_per_cube_cm;
        }), "DM threshold in cm^-3 pc"
    )
    ("sigma_thresh", boost::program_options::value<float>(&_sigma_threshold)->default_value(_sigma_threshold),
        "S/N threshold for candidates to sift,"
    )
    ("pulse_width_threshold", boost::program_options::value<float>()->default_value(_pulse_width_threshold.value())->notifier(
        [&](float f)
        {
            _pulse_width_threshold = MsecTimeType(f * cheetah::data::milliseconds);
        }), "Pulse width threshold in ms"
    )
    ("maximum_candidates", boost::program_options::value<std::size_t>(&_maximum_candidates)->default_value(_maximum_candidates),
        "Threshold for maximum number of candidates. If the number of candidates exceeds the threshold then we resize the list to the threshold. A value of 0 sets the threshold to unlimited."
    );
}

typename Config::Dm Config::dm_threshold() const
{
    return _dm_threshold;
}

void Config::dm_threshold( Dm const& dm_threshold)
{
    _dm_threshold = dm_threshold;
}

typename Config::MsecTimeType Config::pulse_width_threshold() const
{
    return _pulse_width_threshold;
}

void Config::pulse_width_threshold(MsecTimeType const& pulse_width_threshold)
{
    _pulse_width_threshold = pulse_width_threshold;
}

float Config::sigma_threshold() const
{
    return _sigma_threshold;
}

void Config::sigma_threshold(float const& sigma_threshold)
{
    _sigma_threshold = sigma_threshold;
}

std::size_t Config::maximum_candidates() const
{
    return _maximum_candidates;
}

void Config::maximum_candidates(std::size_t const& maximum_candidates)
{
    _maximum_candidates = maximum_candidates;
}

bool Config::active() const
{
    return _active;
}

} // namespace spsift
} // namespace modules
} // namespace cheetah
} // namespace ska
