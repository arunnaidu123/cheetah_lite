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

#include "cheetah/psrdada/detail/RawBytesWriter.h"
#include "panda/Log.h"
#include <algorithm>

namespace ska {
namespace cheetah {
namespace psrdada {
namespace detail {

template <typename DataType, typename Iterator>
Iterator& RawBytesWriter::write(Iterator& begin, Iterator const& end)
{
    std::size_t span = std::distance(begin, end);

    //Calculate the remaining number of elements that can be written
    //in the remaining buffer size.
    std::size_t remaining_elements = remaining_bytes() / sizeof(DataType);

    if (span > remaining_elements)
    {
        PANDA_LOG_DEBUG << "Writing "<< remaining_elements << " elements of size "
                        << sizeof(DataType) << " bytes";
        std::copy(begin, begin + remaining_elements, (DataType*) _write_ptr);
        _write_ptr += remaining_elements*sizeof(DataType);
        begin += remaining_elements;
        _full = true;
        _bytes.used_bytes(_bytes.total_bytes());
    }
    else
    {
        PANDA_LOG_DEBUG << "Writing "<< span << " elements of size " << sizeof(DataType) << " bytes";
        std::copy(begin, end, (DataType*)_write_ptr);
        _write_ptr += span * sizeof(DataType);
        begin += span;
        _full = false;
        _bytes.used_bytes(std::distance(_bytes.ptr(), _write_ptr));
    }
    return begin;
}

} // namespace detail
} // namespace psrdada
} // namespace cheetah
} // namespace ska
