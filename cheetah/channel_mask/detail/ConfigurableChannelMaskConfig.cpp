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
#include "cheetah/channel_mask/ConfigurableChannelMaskConfig.h"


namespace ska {
namespace cheetah {
namespace channel_mask {


template<typename NumericalRep>
ConfigurableChannelMaskConfig<NumericalRep>::ConfigurableChannelMaskConfig(std::string const& tag_name)
    : BaseT(tag_name)
    , _policy_type("replace_constant")
    , _factory(_policy_configs)
{
    this->add(_policy_configs);
}

template<typename NumericalRep>
ConfigurableChannelMaskConfig<NumericalRep>::~ConfigurableChannelMaskConfig()
{
}

template<typename NumericalRep>
std::string const& ConfigurableChannelMaskConfig<NumericalRep>::policy_name() const
{
    return _policy_type;
}

template<typename NumericalRep>
std::string& ConfigurableChannelMaskConfig<NumericalRep>::policy_name()
{
    return _policy_type;
}

template<typename NumericalRep>
void ConfigurableChannelMaskConfig<NumericalRep>::add_options(OptionsDescriptionEasyInit& add_options)
{
    add_options
        ("policy", boost::program_options::value<std::string>(&_policy_type)->default_value(_policy_type),
                   "the policy to apply to flagged channels. Leave blank to deactivate.");
    BaseT::add_options(add_options);
}

template<typename NumericalRep>
typename ConfigurableChannelMaskConfig<NumericalRep>::PolicyType& ConfigurableChannelMaskConfig<NumericalRep>::policy() const
{
    if(!_policy) {
        _policy.reset(_policy_type==""?(new Policy<NumericalRep>()):_factory.create(_policy_type));
    }
    return *_policy;
}

} // namespace channel_mask
} // namespace cheetah
} // namespace ska
