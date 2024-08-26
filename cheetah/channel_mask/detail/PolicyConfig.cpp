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
#include "cheetah/channel_mask/PolicyConfig.h"


namespace ska {
namespace cheetah {
namespace channel_mask {

template<typename T>
PolicyConfig<T>::PolicyConfig(std::string const& tag_name)
    : BaseT(tag_name)
{
    add(_replace_constant_policy_config);
}

template<typename T>
PolicyConfig<T>::~PolicyConfig()
{
}

template<typename T>
ReplaceConstantPolicyConfig<T> const& PolicyConfig<T>::replace_constant_policy_config() const
{
    return _replace_constant_policy_config;
}

template<typename T>
void PolicyConfig<T>::add_options(OptionsDescriptionEasyInit&)
{
}

} // namespace channel_mask
} // namespace cheetah
} // namespace ska
