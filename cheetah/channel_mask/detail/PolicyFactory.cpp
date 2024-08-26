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
#include "cheetah/channel_mask/PolicyFactory.h"
#include "cheetah/channel_mask/ReplaceConstantPolicy.h"
#include "cheetah/channel_mask/Policy.h"

namespace ska {
namespace cheetah {
namespace channel_mask {

template<typename NumRep, typename PolicyType>
class PolicyWrapper final : public Policy<NumRep>
{
        typedef Policy<NumRep> BaseT;
        typedef typename BaseT::SliceType SliceType;

    public:
        template<typename... Args>
        PolicyWrapper(Args&&... args) : _policy(std::forward<Args>(args)...) {}
        PolicyWrapper(PolicyWrapper const&) = delete;
        PolicyWrapper(PolicyWrapper&&) = delete;

        void operator()(SliceType& data) override
        {
            _policy(data);
        }

    private:
        PolicyType _policy;
};

template<typename T>
PolicyFactory<T>::PolicyFactory(PolicyConfig<T> const& config)
    : BaseT("channel_mask Policy Factory:")
    , _config(config)
{
    this->add_type("replace_constant", [this]() { return new PolicyWrapper<T, ReplaceConstantPolicy<T>>(_config.replace_constant_policy_config()); });
}

template<typename T>
PolicyFactory<T>::~PolicyFactory()
{
}

} // namespace channel_mask
} // namespace cheetah
} // namespace ska
