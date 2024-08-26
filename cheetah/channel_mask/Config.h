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
#ifndef SKA_CHEETAH_CHANNEL_MASK_CONFIG_H
#define SKA_CHEETAH_CHANNEL_MASK_CONFIG_H


#include "cheetah/channel_mask/FlaggedChannels.h"
#include "cheetah/utils/Config.h"

namespace ska {
namespace cheetah {
namespace channel_mask {

/**
 * @brief
 * @details
 */

class Config : public utils::Config
{
        typedef utils::Config BaseT;

    public:
        Config(std::string const& tag_name="channel_mask");
        ~Config();

        FlaggedChannels const& flagged_channels() const;
        FlaggedChannels& flagged_channels();


    protected:
        void process_ranges() const;
        void add_options(OptionsDescriptionEasyInit& add_options) override;

    private:
        mutable FlaggedChannels _flagged_channels;
        mutable bool _ranges_processed;
};


} // namespace channel_mask
} // namespace cheetah
} // namespace ska

#endif // SKA_CHEETAH_CHANNEL_MASK_CONFIG_H
