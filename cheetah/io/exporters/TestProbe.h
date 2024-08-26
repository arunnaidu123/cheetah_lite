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
#ifndef SKA_CHEETAH_IO_EXPORTERS_TESTPROBE_H
#define SKA_CHEETAH_IO_EXPORTERS_TESTPROBE_H

#include "panda/ChannelId.h"
#include "panda/Log.h"
#include <deque>
#include <memory>

namespace ska {
namespace cheetah {
namespace io {
namespace exporters {

/**
 * @brief Attach to a stream to record the data sent
 * @details
 */

template<typename T>
class TestProbe
{
    public:
        TestProbe(panda::ChannelId const& id);
        TestProbe(TestProbe&& probe);
        TestProbe(TestProbe const&) = default;

        /// operator to be called by the DataExport stream manager
        void operator()(T const& data);

        /**
         * @brief return the data data received
         * @details each subsequent call will return the next data value or a null ptr
         */
        std::shared_ptr<const T> data();

    private:
        typedef std::deque<std::shared_ptr<const T>> ContainerType;
        panda::ChannelId _id;
        std::shared_ptr<ContainerType> _data;
};

} // namespace exporters
} // namespace io
} // namespace cheetah
} // namespace ska
#include "cheetah/io/exporters/detail/TestProbe.cpp"

#endif // SKA_CHEETAH_IO_EXPORTERS_TESTPROBE_H
