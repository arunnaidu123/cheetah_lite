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
#include "cheetah/channel_mask/Config.h"
#include "cheetah/channel_mask/ChannelRangeConfig.h"


namespace ska {
namespace cheetah {
namespace channel_mask {


Config::Config(std::string const& tag_name)
    : BaseT(tag_name)
    , _ranges_processed(false)
{
    add_factory("range", []() { return new ChannelRangeConfig(); });
}

Config::~Config()
{
}

void Config::add_options(OptionsDescriptionEasyInit& add_options)
{
    add_options
    ("channel_numbers", boost::program_options::value<panda::CommaSeparatedOptions<std::size_t>>()->notifier(
                [&](panda::CommaSeparatedOptions<std::size_t> const& channels) {
                    for(auto const& channel : static_cast<const std::vector<std::size_t>>(channels) )
                    {
                        _flagged_channels.insert(pss::astrotypes::DimensionSpan<data::Frequency>(
                                                          pss::astrotypes::DimensionIndex<data::Frequency>(channel)
                                                        , pss::astrotypes::DimensionSize<data::Frequency>(1)
                                                  ));
                    }
                })
              , "flag the channels specified (e.g. 1,2,4,7,8)"
    )
    ;
}

void Config::process_ranges() const
{
    if(!_ranges_processed)
    {
        panda::ConfigModuleIteratorWrapper<ChannelRangeConfig> it(this->subsection("range"));
        panda::ConfigModuleIteratorWrapper<ChannelRangeConfig> end_it=this->subsection_end();
        while(it!=end_it) {
            _flagged_channels.insert(pss::astrotypes::DimensionSpan<data::Frequency>(
                                    pss::astrotypes::DimensionIndex<data::Frequency>(it->start())
                                   ,pss::astrotypes::DimensionIndex<data::Frequency>(it->end())));
            ++it;
        }
        _ranges_processed=true;
    }
}

FlaggedChannels const& Config::flagged_channels() const
{
    process_ranges();
    return _flagged_channels;
}

FlaggedChannels& Config::flagged_channels()
{
    process_ranges();
    return _flagged_channels;
}

} // namespace channel_mask
} // namespace cheetah
} // namespace ska
