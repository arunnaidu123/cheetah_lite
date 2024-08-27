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
#include "cheetah/modules/spdt/cpu/Config.h"


namespace ska {
namespace cheetah {
namespace modules {
namespace spdt {
namespace cpu {


Config::Config()
    : utils::Config("cpu")
    , _active(false)
    , _samples_per_iteration(1024) // default is set to 1024 samples it is configurable
    , _number_of_widths(10) // default is set as 10 -> we are going to seach upto 1024 samples width.
{
}

Config::~Config()
{
}

void Config::add_options(OptionsDescriptionEasyInit& add_options)
{
    add_options
    ("active", boost::program_options::value<bool>(&_active)->default_value(_active) , "Enable cpu spdt mode")
    ("samples_per_iteration", boost::program_options::value<std::size_t>(&_samples_per_iteration)->default_value(_samples_per_iteration), "Enable emulation mode")
    ("number_of_widths", boost::program_options::value<std::size_t>(&_number_of_widths)->default_value(_number_of_widths) , "Enable emulation mode");
}

bool Config::active() const
{
    return _active;
}

void Config::activate()
{
    _active = true;
}

std::size_t Config::samples_per_iteration() const
{
    return _samples_per_iteration;
}

void Config::samples_per_iteration(std::size_t const& value)
{
    _samples_per_iteration = value;
}

std::size_t Config::number_of_widths() const
{
    return _number_of_widths;
}

void Config::number_of_widths(std::size_t const& value)
{
    _number_of_widths = value;
}

} // namespace cpu
} // namespace spdt
} // namespace modules
} // namespace cheetah
} // namespace ska
