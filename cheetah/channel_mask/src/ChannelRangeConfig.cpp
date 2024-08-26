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
#include "cheetah/channel_mask/ChannelRangeConfig.h"
#include "cheetah/data/Units.h"


namespace ska {
namespace cheetah {
namespace channel_mask {


ChannelRangeConfig::ChannelRangeConfig(std::string const& tag_name)
    : utils::Config(tag_name)
{
}

ChannelRangeConfig::~ChannelRangeConfig()
{
}

void ChannelRangeConfig::add_options(OptionsDescriptionEasyInit& add_options)
{
    add_options
    ("start_number", boost::program_options::value<std::size_t>(&_start)
                   , "the first channel numberin the range (inclusive)")
    ("end_number", boost::program_options::value<std::size_t>(&_end)
                 , "the last channel number in the range (inclusive)")
    ;
}

typename ChannelRangeConfig::Channel const& ChannelRangeConfig::start() const
{
    return _start;
}

void ChannelRangeConfig::start(Channel const& start)
{
    _start = start;;
}

typename ChannelRangeConfig::Channel const& ChannelRangeConfig::end() const
{
    return _end;
}

void ChannelRangeConfig::end(Channel const& end)
{
    _end = end;;
}

} // namespace channel_mask
} // namespace cheetah
} // namespace ska
