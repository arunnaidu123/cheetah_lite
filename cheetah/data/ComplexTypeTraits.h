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
#ifndef SKA_CHEETAH_DATA_COMPLEXTYPETRAITS_H
#define SKA_CHEETAH_DATA_COMPLEXTYPETRAITS_H

#include "cheetah/Configuration.h"
#include "cheetah/utils/Architectures.h"
#include "cheetah/cuda_utils/cuda_thrust.h"
#include <type_traits>

namespace ska {
namespace cheetah {
namespace data {

/**
 * @brief     A helper class to determine the type of complex data for different architectures
 */
template <typename Arch, typename T>
struct ComplexTypeTraits
{
};

/**
 * Set the complex type for Cpu to std::complex
 */
template <typename T>
struct ComplexTypeTraits<cheetah::Cpu, T>
{
    typedef std::complex<T> type;
};

#ifdef SKA_CHEETAH_ENABLE_CUDA
/**
 * Set the complex type for Cuda to thrust::complex
 */
template <typename T>
struct ComplexTypeTraits<cheetah::Cuda, T>
{
    typedef thrust::complex<T> type;
};
#endif //SKA_CHEETAH_ENABLE_CUDA

template <typename T>
struct ComplexTypeTraits<cheetah::Fpga, T>
{
    typedef std::complex<T> type;
};

template<class Arch, typename T>
using ComplexTypeTraits_t = typename ComplexTypeTraits<Arch, T>::type;

/**
 * @brief type trait to deduce if a type represents a complex number
 * @tparam T the type to test and extract type information from
 * @details
 *    the following typedefs are set:
 *      those or std::true_type if it is complex number otherwise std::false_type
 *      value_type is set to the numerical value type (e.g. float, int) of the complex
 *        class or, in the case T is not a complex number type, to T.
 */
template<typename T>
struct IsComplexNumber : public std::false_type
{
    typedef T value_type;
};

template<typename T>
struct IsComplexNumber<std::complex<T>> : public std::true_type
{
    typedef T value_type;
};

#ifdef SKA_CHEETAH_ENABLE_CUDA
template<typename T>
struct IsComplexNumber<thrust::complex<T>> : public std::true_type
{
    typedef T value_type;
};
#endif // SKA_CHEETAH_ENABLE_CUDA

/**
 * @brief EnableIf helper to match complex number types
 * @details
 * Use to specialise a function:
 * @code
 * template<typename T
 *         , EnableIfIsComplexNumber<T, bool> = true
 *  void my_function(T const& number);
 * @endcode
 *
 * Use to specialise a class/struct:
 * @code
 * template<typename T, typename Enable=void>
 * struct MyStruct;
 *
 * template<typename T>
 * struct MyStruct<T, EnableIfIsComplexNumber<T>>;
 */
template<typename T, typename EnableT = void>
using EnableIfIsComplexNumber = typename std::enable_if<IsComplexNumber<T>::value, EnableT>::type;

/**
 * @brief EnableIf helper to match non-complex number types
 */
template<typename T, typename EnableT = void>
using EnableIfIsNotComplexNumber = typename std::enable_if<!IsComplexNumber<T>::value, EnableT>::type;

/**
 * @brief type traits helper to convert between different complex number reps by Architecture
 */
template<typename ArchT, typename NumberT, typename Enable=void>
struct ConvertComplexNumber
{
    typedef NumberT type; // default not a complex number
    typedef NumberT value_type;
};

template<typename ArchT, typename NumberT>
struct ConvertComplexNumber<ArchT, NumberT, EnableIfIsComplexNumber<NumberT>>
{
    typedef typename IsComplexNumber<NumberT>::value_type value_type;
    typedef typename ComplexTypeTraits<ArchT, value_type>::type type;
};

template<typename ArchT, typename NumberT>
using ConvertComplexNumber_t = typename ConvertComplexNumber<ArchT, NumberT>::type;

} // namespace data
} // namespace cheetah
} // namespace ska

#endif // SKA_CHEETAH_DATA_COMPLEXTYPETRAITS_H
