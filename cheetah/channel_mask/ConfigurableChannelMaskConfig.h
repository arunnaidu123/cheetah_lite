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
#ifndef SKA_CHEETAH_CHANNEL_MASK_CONFIGURABLECHANNELMASKCONFIG_H
#define SKA_CHEETAH_CHANNEL_MASK_CONFIGURABLECHANNELMASKCONFIG_H


#include "cheetah/channel_mask/Config.h"
#include "cheetah/channel_mask/PolicyConfig.h"
#include "cheetah/channel_mask/PolicyFactory.h"
#include "cheetah/channel_mask/Policy.h"
#include <string>

namespace ska {
namespace cheetah {
namespace channel_mask {

/**
 * @brief Configuration for the Configurable ChannelMask module
 * @details
 */

template<typename NumericalRep>
class ConfigurableChannelMaskConfig : public Config
{
        typedef Config BaseT;
        typedef Policy<NumericalRep> PolicyType;

    public:
        ConfigurableChannelMaskConfig(std::string const& tag_name="channel_mask");
        ~ConfigurableChannelMaskConfig();

        /**
         *  @brief the policy to apply to flagged channels
         */
        std::string const& policy_name() const;
        std::string& policy_name();

        /**
         * @brief return the Policy object matching the configuration
         */
        PolicyType& policy() const;

    protected:
        void add_options(OptionsDescriptionEasyInit& add_options) override;

    private:
        std::string _policy_type;
        PolicyConfig<NumericalRep> _policy_configs;
        PolicyFactory<NumericalRep> _factory;
        mutable std::unique_ptr<PolicyType> _policy;
};


} // namespace channel_mask
} // namespace cheetah
} // namespace ska

#include "detail/ConfigurableChannelMaskConfig.cpp"

#endif // SKA_CHEETAH_CHANNEL_MASK_CONFIGURABLECHANNELMASKCONFIG_H
