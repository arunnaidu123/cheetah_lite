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
#include "cheetah/modules/spdt/klotski_common/Config.h"


namespace ska {
namespace cheetah {
namespace modules {
namespace spdt {
namespace klotski_common {


Config::Config(std::string module_name)
    : utils::Config(module_name)
    , _widths({1,2,4,5,6,7,8,9,10,11,12,15,16,17,22,23,24,25,28,29,30,31,32,33,34,100, 150, 300, 400, 512, 768, 1024, 1200, 1400, 1600, 2048, 4096}) // array of widths.
    , _number_of_dms_per_iteration(8)
    , _max_width_limit(16*1024) // limit on the width thah can be searched. Dont change it unless you what you are doing.
{

}

Config::~Config()
{
}

void Config::add_options(OptionsDescriptionEasyInit& add_options)
{
    add_options
    ( "widths"
    , boost::program_options::value<panda::CommaSeparatedOptions<unsigned>>(&_widths)->multitoken()
    , "widths serperated by commas")

    ("number_of_dms_per_iteration"
    , boost::program_options::value<unsigned>(&_number_of_dms_per_iteration)->default_value(_number_of_dms_per_iteration)
    , "number of dms per iteration")
    ( "max_width_limit"
    , boost::program_options::value<unsigned>(&_max_width_limit)->default_value(_max_width_limit)
    , "max width limit that can be searched (this value need not be in the width array but must be a multiple of 1024)");
}

std::vector<unsigned> const& Config::widths() const
{
    return static_cast<std::vector<unsigned> const&>(_widths);
}

void Config::widths(std::vector<unsigned> const& widths)
{
    _widths = widths;
}

unsigned int const& Config::number_of_dms_per_iteration() const
{
    return _number_of_dms_per_iteration;
}

void Config::number_of_dms_per_iteration(unsigned int& value)
{
    _number_of_dms_per_iteration = value;
}

unsigned int const& Config::max_width_limit() const
{
    return _max_width_limit;
}

void Config::max_width_limit(unsigned int& value)
{
    _max_width_limit = value;
}

} // namespace klotski_common
} // namespace spdt
} // namespace modules
} // namespace cheetah
} // namespace ska
