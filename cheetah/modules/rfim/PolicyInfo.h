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
#ifndef SKA_CHEETAH_MODULES_RFIM_POLICYINFO_H
#define SKA_CHEETAH_MODULES_RFIM_POLICYINFO_H
#include <boost/function_types/parameter_types.hpp>
#include <boost/function_types/function_type.hpp>
#include <boost/function_types/result_type.hpp>
#include <type_traits>


namespace ska {
namespace cheetah {
namespace modules {
namespace rfim {

/**
 * @brief Analyse RFIM policy to extract traits
 * @details
 */

template<typename PolicyType>
class PolicyInfo
{

    protected:
        // examine policy operator() to determine the constness of the argument and return types
        typedef decltype(&PolicyType::adapter) AdapterFuncType;
    public:
        typedef typename boost::function_types::result_type<AdapterFuncType>::type AdapterType;

    private:
        typedef decltype(&AdapterType::data) FuncType;

    public:
        typedef typename boost::function_types::result_type<FuncType>::type AdapterDataReturnType;

        /// The argument type that is to be expected to represent the TimeFrequencyData
        typedef typename std::conditional<
                        std::is_member_function_pointer<AdapterFuncType>::value,
                        typename boost::mpl::at_c<boost::function_types::parameter_types<AdapterFuncType>, 1>::type,
                        typename boost::mpl::at_c<boost::function_types::parameter_types<AdapterFuncType>, 0>::type
                        >::type DataArgumentType;

        /// the data type the policy will return
        typedef typename std::conditional<std::is_same<void, AdapterDataReturnType>::value,
                                          DataArgumentType,
                                          AdapterDataReturnType>::type ReturnType;

};


} // namespace rfim
} // namespace modules
} // namespace cheetah
} // namespace ska

#endif // SKA_CHEETAH_MODULES_RFIM_POLICYINFO_H
