/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 The SKA organisation
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
#include "cheetah/modules/rfim/iqrmcpu/Config.h"


namespace ska {
namespace cheetah {
namespace modules {
namespace rfim {
namespace iqrmcpu {

Config::Config()
    : utils::Config("rfim_iqrmcpu")
    , _active(false)
    , _radius(100)
    , _threshold(3.0)
    , _edge_channels(0)
{
}

Config::~Config()
{
}

void Config::add_options(OptionsDescriptionEasyInit& add_options)
{
    namespace bpo = boost::program_options;
    add_options
    ("active", bpo::value<bool>(&_active)->default_value(_active), "Whether to enable the IQRM adaptive channel masking algorithm")
    ("radius", bpo::value<std::size_t>(&_radius)->default_value(_radius), "Radius parameter of IQRM, in number of frequency channels")
    ("threshold", bpo::value<float>(&_threshold)->default_value(_threshold), "Threshold parameter of IQRM, in number of Gaussian sigmas")
    ("edge_channels", bpo::value<std::size_t>(&_edge_channels)->default_value(_edge_channels), "Number of channels to forcibly flag on both band edges");
}

bool Config::active() const
{
    return _active;
}

void Config::active(bool b)
{
    _active = b;
}

std::size_t Config::radius() const
{
    return _radius;
}

void Config::radius(std::size_t r)
{
    _radius = r;
}

float Config::threshold() const
{
    return _threshold;
}

void Config::threshold(float t)
{
    _threshold = t;
}

std::size_t Config::edge_channels() const
{
    return _edge_channels;
}

void Config::edge_channels(std::size_t ec)
{
    _edge_channels = ec;
}

} // namespace iqrmcpu
} // namespace rfim
} // namespace modules
} // namespace cheetah
} // namespace ska
