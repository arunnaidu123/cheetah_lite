/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2022 The SKA organisation
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
#ifndef SKA_CHEETAH_DATA_DATATYPETRAITS_H
#define SKA_CHEETAH_DATA_DATATYPETRAITS_H

#include "cheetah/utils/Architectures.h"
#include <type_traits>

namespace ska {
namespace cheetah {
namespace data {

/**
 * @brief Extract type information from the various data types
 * @tparam DataT the data type to inspect
 * @details
 *   Each data type should provide a specialisation to provide the correct information
 */
template<typename DataT>
struct DataTypeTraits
{
    /// The provided DataT
    typedef DataT type;

    /// The Architecture associated with the DataT
    typedef cheetah::Cpu Architecture;

    /// The type used to represent data for the data type
    typedef DataT value_type;

    /// The Allocator type
    typedef std::allocator<value_type> Allocator;
};

/**
 * @brief SFINAE helper to determine if a data type has a specific Architecture
 */
template<class ArchT, class DataT, typename T = void>
using EnableIfHasArch = typename std::enable_if<std::is_same<typename data::DataTypeTraits<DataT>::Architecture, ArchT>::value, T>::type;

/**
 * @brief SFINAE helper to determine if a data type does not have a specific Architecture
 */
template<class ArchT, class DataT, typename T = void>
using EnableIfNotHasArch = typename std::enable_if<!std::is_same<typename data::DataTypeTraits<DataT>::Architecture, ArchT>::value, T>::type;

/**
 * @brief Helper classes to specialise the creation of a data type on a specific device
 * @tparam DataT the type of data to generate
 * @tparam Enable Allow specialisations of theis helper for explicit circumstances
 *
 * @details Uses DataTypeTraits structure, so this should produce the correct results for provided DataT
 *          The presumption is that the type is some sort of container with an allocator.
 */
template<class DataT, typename Enable = void>
struct CreateDataType
{
        typedef typename DataTypeTraits<DataT>::Allocator Allocator;

        template<typename ArchT, typename... Args>
        static inline
        DataT* create_ptr(panda::PoolResource<ArchT>& device, Args&&... args)
        {
            return new DataT(std::forward<Args>(args)..., Allocator(device));
        }

        template<typename ArchT, typename... Args>
        static inline
        DataT create(panda::PoolResource<ArchT>& device, Args&&... args)
        {
            return DataT(std::forward<Args>(args)..., Allocator(device));
        }
};

template<class DataT>
struct CreateDataType<DataT, typename std::enable_if<std::is_default_constructible<typename DataTypeTraits<DataT>::Allocator>::value>::type>
{
        template<typename ArchT, typename... Args>
        static inline
        DataT* create_ptr(panda::PoolResource<ArchT>&, Args&&... args) {
            return new DataT(std::forward<Args>(args)...);
        }

        template<typename ArchT, typename... Args>
        static inline
        DataT create(panda::PoolResource<ArchT>&, Args&&... args) {
            return DataT(std::forward<Args>(args)...);
        }
};

/**
 * @brief create the specified data type given a device (and optional arguments)
 * @param device The device on which the data will be associated
 * @param args Arguments will be forwarded to the constructor of DataT
 * @tparam DataT must be specified by the user. If this is a pointer type then
 *         new will be called, else the object will be constructed on the stack.
 *         n.b. shared_ptr types are also supported and use std::make_shared.
 * @details convenience function to call the appropriate CreateDataType::create method
 *           Uses DataTypeTraits structure, so this should produce the correct results for provided DataT.
 */
template<typename DataT, class ArchT, typename... Args>
DataT create_data_type(panda::PoolResource<ArchT>& device, Args&&... args);

} // namespace data
} // namespace cheetah
} // namespace ska
#include "detail/DataTypeTraits.cpp"

#endif // SKA_CHEETAH_DATA_DATATYPETRAITS_H
