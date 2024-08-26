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
#include "cheetah/modules/sps_clustering/Config.h"

namespace ska {
namespace cheetah {
namespace modules {
namespace sps_clustering {


Config::Config()
    : utils::Config("sps_clustering")
    , _pulse_width_tolerance(5 * cheetah::data::milliseconds)
    , _time_tolerance(100 * cheetah::data::milliseconds)
    , _dm_tolerance(1 * pss::astrotypes::units::parsecs_per_cube_cm)
    , _linking_length(1.7)
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
        "perform SpsClustering if true"
    )
    ("number_of_threads", boost::program_options::value<std::size_t>(&_num_threads)->default_value(1),
        "number of threads to use for clustering"
    )
    ("time_tolerance", boost::program_options::value<float>()->notifier(
        [&](float f)
        {
            _time_tolerance = MsecTimeType(f * cheetah::data::milliseconds);
        }), "start time tolerance in ms to cluster candidates in time space."
    )
    ("dm_thresh", boost::program_options::value<float>()->notifier(
        [&](float f)
        {
            _dm_tolerance = f * pss::astrotypes::units::parsecs_per_cube_cm;
        }), "DM tolerance in cm^-3 pc to cluster candidates in DM space"
    )
    ("pulse_width_tolerance", boost::program_options::value<float>()->notifier(
        [&](float f)
        {
            _pulse_width_tolerance = MsecTimeType(f * cheetah::data::milliseconds);
        }), "Pulse width tolerance in ms to cluster candidates in pulse width space."
    )
    ("linking_length", boost::program_options::value<float>(&_linking_length)->default_value(1.7),
        "Linking length for clustering. This is the Euclidean distance in our parameter space inside which all candidates are considered part of the same cluster."
    );
}

typename Config::Dm Config::dm_tolerance() const
{
    return _dm_tolerance;
}

void Config::dm_tolerance( Dm const& dm_tolerance)
{
    _dm_tolerance = dm_tolerance;
}

typename Config::MsecTimeType Config::pulse_width_tolerance() const
{
    return _pulse_width_tolerance;
}

void Config::pulse_width_tolerance(MsecTimeType const& pulse_width_tolerance)
{
    _pulse_width_tolerance = pulse_width_tolerance;
}

typename Config::MsecTimeType Config::time_tolerance() const
{
    return _time_tolerance;
}

void Config::time_tolerance(MsecTimeType const& time_tolerance)
{
    _time_tolerance = time_tolerance;
}

float Config::linking_length() const
{
    return _linking_length;
}

void Config::linking_length(float const& l)
{
    _linking_length = l;
}

bool Config::active() const
{
    return _active;
}

std::size_t Config::num_threads() const
{
    return _num_threads;
}

void Config::num_threads(std::size_t const nt)
{
    _num_threads = nt;
}

} // namespace sps_clustering
} // namespace modules
} // namespace cheetah
} // namespace ska
