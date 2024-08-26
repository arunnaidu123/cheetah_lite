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
#include "cheetah/modules/rfim/policy/Policy.h"


namespace ska {
namespace cheetah {
namespace modules {
namespace rfim {
namespace policy {

namespace {
    template<typename ReturnType, typename AdapterDataReturnType>
    struct DoReturn
    {
        template<typename Adapter, typename DataType>
        static inline
        ReturnType exec(Adapter& adapter, DataType const&) {
            return adapter.data();
        }
    };

    template<typename ReturnType>
    struct DoReturn<ReturnType, void>
    {
        template<typename Adapter>
        static inline
        ReturnType& exec(Adapter const&, ReturnType& arg) {
            return arg;
        }
    };

} // namespace

template<typename PolicyType>
Policy<PolicyType>::Policy()
{
}

template<typename PolicyType>
template<typename HandlerType>
typename Policy<PolicyType>::ReturnType Policy<PolicyType>::exec(HandlerType handler, ArgumentType& arg)
{
    AdapterType adapter( _policy.adapter(arg) );
    handler(arg, adapter);
    return DoReturn<ReturnType, typename PolicyInfo<PolicyType>::AdapterDataReturnType>::exec(adapter, arg);
}

template<typename PolicyType>
typename Policy<PolicyType>::ReturnType Policy<PolicyType>::null_op(ArgumentType& arg)
{
    AdapterType adapter( _policy.adapter(arg) );
    return DoReturn<ReturnType, typename PolicyInfo<PolicyType>::AdapterDataReturnType>::exec(adapter, arg);
}

} // namespace policy
} // namespace rfim
} // namespace modules
} // namespace cheetah
} // namespace ska
