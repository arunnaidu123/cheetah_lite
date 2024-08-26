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
#ifndef SKA_CHEETAH_MODULES_RFIM_POLICY_H
#define SKA_CHEETAH_MODULES_RFIM_POLICY_H

#include "cheetah/modules/rfim/PolicyInfo.h"

namespace ska {
namespace cheetah {
namespace modules {
namespace rfim {
namespace policy {

/**
 * @brief Policy executor. Performs the sequence of operations required to use the Policy object
 * @details Any policy conforming to the requirements of the rfim::PolicyInfo template can be used
 */

template<typename PolicyType>
class Policy
{
    public:
        typedef typename PolicyInfo<PolicyType>::ReturnType ReturnType;
        typedef typename PolicyInfo<PolicyType>::DataArgumentType ArgumentType;
        typedef typename PolicyInfo<PolicyType>::AdapterType AdapterType;

    public:
        Policy();

        /**
         * @brief start a single Policy sequence.
         * 1) a data adapter is requested from the policy
         * 2) the argument and the adapter are then passed to the provided handler, which should use the adpater interface to modify data.
         * 3) the policies return type is generatoed by the adpater after the handler returns
         * @tparam Handler: signature dhould be (ArgumentType const& data, AdapterType& data_adapter)
         *                  the data_adapter interface should be any data modifications inside the handler
         *                  and represents the same data as passed in ArgumentType
         */
        template<typename Handler>
        inline ReturnType exec(Handler, ArgumentType& data);

        /**
         * @brief Generates the appropriate ReturnType for the Policy but performs no actions in the data.
         */
        inline ReturnType null_op(ArgumentType& data);

    private:
        PolicyType _policy;
};


} // namespace policy
} // namespace rfim
} // namespace modules
} // namespace cheetah
} // namespace ska
#include "detail/Policy.cpp"

#endif // SKA_CHEETAH_MODULES_RFIM_POLICY_H