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
#include "cheetah/psrdada/detail/RawBytesReader.h"
#include "panda/Log.h"
#include <algorithm>

namespace ska {
namespace cheetah {
namespace psrdada {
namespace detail {


template <typename DataType, typename Iterator>
Iterator& RawBytesReader::read(Iterator& begin, Iterator const& end)
{
    std::size_t span = std::distance(begin, end);
    std::size_t remaining_elements = remaining_bytes()/sizeof(DataType);
    if (span > remaining_elements)
    {
        PANDA_LOG_DEBUG << "Reading " << remaining_elements
                        << " elements of size " << sizeof(DataType) << " bytes";
        std::copy((DataType*)_read_ptr, ((DataType*)_read_ptr)+remaining_elements, begin);
        begin += remaining_elements;
        _read_ptr += remaining_elements * sizeof(DataType);
        _empty = true;
    }
    else
    {
        PANDA_LOG_DEBUG << "Reading " << span << " elements of size "
                        << sizeof(DataType) << " bytes";
        std::copy((DataType*)_read_ptr, ((DataType*)_read_ptr)+span, begin);
        begin += span;
        _read_ptr += span*sizeof(DataType);
        _empty = false;
    }
    return begin;
}

} // namespace detail
} // namespace psrdada
} // namespace cheetah
} // namespace ska
