/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016-2022 The SKA organisation
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

#include <panda/Error.h>

namespace ska {
namespace cheetah {
namespace io {
namespace producers {
namespace rcpt {


template<typename ReturnType, template <SkaSelector::Stream> class Executor, typename... Args>
ReturnType SkaSelector::select(std::string const& stream_id, Args&&... args)
{
    if(stream_id == "udp_low") {
        return Executor<Stream::SkaLow>::exec(std::forward<Args>(args)...);
    }
    else if(stream_id == "udp_mid") {
        return Executor<Stream::SkaMid>::exec(std::forward<Args>(args)...);
    }
    panda::Error e("unknown stream: ");
    e << stream_id;
    throw e;
}

} // namespace rcpt
} // namespace producers
} // namespace io
} // namespace cheetah
} // namespace ska
