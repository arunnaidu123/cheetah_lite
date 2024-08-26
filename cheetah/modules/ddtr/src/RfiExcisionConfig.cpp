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
#include "cheetah/modules/ddtr/RfiExcisionConfig.h"


namespace ska {
namespace cheetah {
namespace modules {
namespace ddtr {


RfiExcisionConfig::RfiExcisionConfig()
    : utils::Config("rfiexcision")
    , _ideal_rms(7.0)
    , _threshold(5.0)
    , _active(true)
{
}

RfiExcisionConfig::~RfiExcisionConfig()
{
}

float RfiExcisionConfig::ideal_rms() const
{
    return _ideal_rms;
}

float RfiExcisionConfig::threshold() const
{
    return _threshold;
}

bool RfiExcisionConfig::active() const
{
    return _active;
}

void RfiExcisionConfig::add_options(OptionsDescriptionEasyInit& add_options)
{
    add_options
    ("active", boost::program_options::value<bool>(&_active), "flag to active thresholding during the copy.")
    ("ideal_rms", boost::program_options::value<float>(&_ideal_rms), "RMS to use for thresholding")
    ("threshold", boost::program_options::value<float>(&_threshold), "Threshold (in units of rms) to use for identifying/flagging RFI");
}

} // namespace ddtr
} // namespace modules
} // namespace cheetah
} // namespace ska
