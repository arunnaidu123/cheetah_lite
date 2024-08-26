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


namespace ska {
namespace cheetah {
namespace psrdada {
namespace detail {

RawBytesWriter::RawBytesWriter(RawBytes& bytes)
    : _bytes(bytes)
    , _write_ptr(_bytes.ptr())
    , _end_ptr(_bytes.ptr() + _bytes.total_bytes())
    , _full(false)
{
}

RawBytesWriter::~RawBytesWriter()
{
}

bool RawBytesWriter::full() const
{
    return (_write_ptr >= _end_ptr) || _full;
}

std::size_t RawBytesWriter::remaining_bytes() const
{
    return _end_ptr - _write_ptr;
}

} // namespace detail
} // namespace psrdada
} // namespace cheetah
} // namespace ska
