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

namespace ska {
namespace cheetah {
namespace data {

template <typename Type>
DataSequence2DPartialRangeImpl<Type>::DataSequence2DPartialRangeImpl(
        typename DataSequence2DPartialRangeTraits<Type>::DataType& data,
        std::size_t fbegin,
        std::size_t fend,
        std::size_t sbegin,
        std::size_t send,
        std::size_t flength)
    : _data(data)
    , _fbegin(fbegin)
    , _fend(fend)
    , _sbegin(sbegin)
    , _send(send)
    , _raw_flength(flength)
{}

template <typename Type>
typename DataSequence2DPartialRangeTraits<Type>::Iterator DataSequence2DPartialRangeImpl<Type>::begin() const
{
    return Iterator(
            _data.begin() + _raw_flength * _sbegin + _fbegin,
            _fend - _fbegin,
            _raw_flength - _fend + _fbegin);
}

template <typename Type>
typename DataSequence2DPartialRangeTraits<Type>::Iterator DataSequence2DPartialRangeImpl<Type>::end() const
{
    return Iterator(
            _data.begin() + _send * _raw_flength + _fbegin);
}

} // namespace data
} // namespace cheetah
} // namespace ska

