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
#include <type_traits>
#include <memory>

namespace ska {
namespace cheetah {
namespace data {
namespace {
// helper to implement the create_data_type() functino
// default is for non pointer types
template<typename DataT, typename Enable=void>
struct CreateDataTypeHelper
{
    template<class ArchT, typename... Args>
    static inline DataT exec(panda::PoolResource<ArchT>& device, Args&&... args)
    {
        return CreateDataType<DataT>::create(device, std::forward<Args>(args)...);
    }
};

// specialism for pointer types
template<typename DataT>
struct CreateDataTypeHelper<DataT, typename std::enable_if<std::is_pointer<DataT>::value>::type>
{
    template<class ArchT, typename... Args>
    static inline DataT exec(panda::PoolResource<ArchT>& device, Args&&... args)
    {
        return CreateDataType<typename std::remove_pointer<DataT>::type>::create_ptr(device, std::forward<Args>(args)...);
    }
};

// specialism for shared_ptr types
template<typename DataT>
struct CreateDataTypeHelper<std::shared_ptr<DataT>>
{
    template<class ArchT, typename... Args>
    static inline DataT exec(panda::PoolResource<ArchT>& device, Args&&... args)
    {
        return std::make_shared<DataT>(CreateDataType<DataT>::create_ptr(device, std::forward<Args>(args)...));
    }
};

} // namespace

template<typename DataT, class ArchT, typename... Args>
DataT create_data_type(panda::PoolResource<ArchT>& device, Args&&... args)
{
    return CreateDataTypeHelper<DataT>::exec(device, std::forward<Args>(args)...);
}

} // namespace data
} // namespace cheetah
} // namespace ska
