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

#include "cheetah/psrdada/DadaWriteClient.h"
#include "panda/Log.h"
#include "panda/Error.h"
#include <cstddef>

namespace ska {
namespace cheetah {
namespace psrdada {

template <typename Iterator, typename DataType>
void DadaWriteClient::write(Iterator& begin, Iterator const& end)
{
    PANDA_LOG_DEBUG << "Number of elements to write " << std::distance(begin,end);
    while (begin != end)
    {
        if (!_current_writer)
        {
            acquire_data_block();
        }
        else if (_current_writer->full())
        {
            PANDA_LOG_DEBUG << "Current writer full, acquiring new writer instance";
            acquire_data_block();
        }
        begin = _current_writer->write<typename std::remove_cv<DataType>::type>(begin, end);
    }
}

} // namespace psrdada
} // namespace cheetah
} // namespace ska
