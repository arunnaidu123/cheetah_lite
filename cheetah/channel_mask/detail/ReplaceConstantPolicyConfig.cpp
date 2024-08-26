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
#include "cheetah/channel_mask/ReplaceConstantPolicyConfig.h"
#include <limits>


namespace ska {
namespace cheetah {
namespace channel_mask {

template<typename T>
ReplaceConstantPolicyConfig<T>::ReplaceConstantPolicyConfig(std::string const& tag_name)
    : BaseT(tag_name)
    , _value(std::numeric_limits<T>::max()/2)
{
}

template<typename T>
ReplaceConstantPolicyConfig<T>::~ReplaceConstantPolicyConfig()
{
}

template<typename T>
T const& ReplaceConstantPolicyConfig<T>::value() const
{
    return _value;
}

template <typename T>
void ReplaceConstantPolicyConfig<T>::value(T const& value)
{
    _value = value;
}

namespace {
    template<typename T>
    struct CharHandler {
        typedef T type;
    };

    template<>
    struct CharHandler<char> {
        typedef int type;
    };
    template<>
    struct CharHandler<unsigned char> {
        typedef unsigned type;
    };
}

template <typename T>
void ReplaceConstantPolicyConfig<T>::add_options(OptionsDescriptionEasyInit& add_options)
{
    typedef typename CharHandler<T>::type TT;
    add_options
    ("value", boost::program_options::value<TT>()->default_value((TT)_value)->notifier(
                                  [this](TT const& val)
                                  {
                                       _value = (T)val;
                                  })
                                  , "value to set flagged channels");
}

} // namespace channel_mask
} // namespace cheetah
} // namespace ska
