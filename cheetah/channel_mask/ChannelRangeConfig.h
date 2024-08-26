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
#ifndef SKA_CHEETAH_CHANNEL_MASK_CHANNELRANGECONFIG_H
#define SKA_CHEETAH_CHANNEL_MASK_CHANNELRANGECONFIG_H

#include "cheetah/utils/Config.h"

namespace ska {
namespace cheetah {
namespace channel_mask {

/**
 * @brief Config Node to specfiy a range of channels
 */

class ChannelRangeConfig : public utils::Config
{
        typedef std::size_t Channel;

    public:
        ChannelRangeConfig(std::string const& tag_name="range");
        ~ChannelRangeConfig();

        /// get the start of the range
        Channel const& start() const;

        /// set the start of the range
        void start(Channel const&);

        /// get the end of the range
        Channel const& end() const;

        /// set the end of the range
        void end(Channel const&);

    protected:
        void add_options(OptionsDescriptionEasyInit& add_options) override;

    private:
        Channel _start;
        Channel _end;
};


} // namespace channel_mask
} // namespace cheetah
} // namespace ska

#endif // SKA_CHEETAH_CHANNEL_MASK_CHANNELRANGECONFIG_H
