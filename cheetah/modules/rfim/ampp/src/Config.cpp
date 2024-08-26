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
#include "cheetah/modules/rfim/ampp/Config.h"


namespace ska {
namespace cheetah {
namespace modules {
namespace rfim {
namespace ampp {


Config::Config()
    : utils::Config("rfim_ampp")
    , _active(false)
    , _max_history(8192)
    , _cr_factor(8.0f)
    , _sr_factor(8.0f)
    , _zero_dm(false)
{
    add(_bandpass_config);
}

Config::~Config()
{
}

void Config::add_options(OptionsDescriptionEasyInit& add_options)
{
    add_options
    ("active", boost::program_options::value<bool>(&_active)->default_value(false), "use this algorithm for rfi clipping")
    ("channel_rejection_rms", boost::program_options::value<float>(&_cr_factor), "specify the channel rejection factor for the algorithm")
    ("spectrum_rejection_rms", boost::program_options::value<float>(&_sr_factor), "specify the spectrum rejection factor for the algorithm")
    ("max_history", boost::program_options::value<unsigned>(&_max_history)->default_value(_max_history), "maximum number of samples to keep stats")
    ("zero_dm", boost::program_options::value<bool>(&_zero_dm)->default_value(_zero_dm), "flatten the channel if RFI detected");
}

bool Config::active() const
{
    return _active;
}

unsigned Config::max_history() const
{
    return _max_history;
}

float Config::channel_rejection_rms() const
{
    return _cr_factor;
}

float Config::spectrum_rejection_rms() const
{
    return _sr_factor;
}


bool Config::zero_dm() const
{
    return _zero_dm;
}

BandPassConfig const& Config::bandpass() const
{
    return _bandpass_config;
}


BandPassConfig& Config::bandpass()
{
    return _bandpass_config;
}

} // namespace ampp
} // namespace rfim
} // namespace modules
} // namespace cheetah
} // namespace ska
