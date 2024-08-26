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

#include "cheetah/modules/rfim/Config.h"

namespace ska {
namespace cheetah {
namespace modules {
namespace rfim {


Config::Config()
    : cheetah::utils::Config("rfim")
    , _policy(true)
{
    add(_ampp_config);
    add(_cuda_config);
    add(_sum_threshold_config);
    add(_iqrmcpu_config);
}

Config::~Config()
{
}

void Config::add_options(OptionsDescriptionEasyInit& add_options)
{
    add_options
        ("flag-policy", boost::program_options::value<bool>(&_policy)->default_value(_policy), "if set true, detected RFI will be marked as a flag mask. Otherwise input data will be modified usinf the LastUnflagged replacement method");
}

ampp::Config const& Config::ampp_algo_config() const
{
    return _ampp_config;
}

cuda::Config const& Config::cuda_algo_config() const
{
    return _cuda_config;
}

sum_threshold::Config const& Config::sum_threshold_algo_config() const
{
    return _sum_threshold_config;
}

iqrmcpu::Config const& Config::iqrmcpu_algo_config() const
{
    return _iqrmcpu_config;
}

bool Config::algo_defined() const
{
    return _ampp_config.active() ||
           _cuda_config.active() ||
           _sum_threshold_config.active() ||
           _iqrmcpu_config.active();
}

bool Config::flag_policy() const
{
    return _policy;
}

} // namespace rfim
} // namespace modules
} // namespace cheetah
} // namespace ska
