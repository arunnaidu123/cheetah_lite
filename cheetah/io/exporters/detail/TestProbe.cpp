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
#include "cheetah/io/exporters/TestProbe.h"


namespace ska {
namespace cheetah {
namespace io {
namespace exporters {


template<typename T>
TestProbe<T>::TestProbe(panda::ChannelId const& id)
    : _id(id)
    , _data(new ContainerType)
{
}

template<typename T>
TestProbe<T>::TestProbe(TestProbe&& p)
    : _id(std::move(p._id))
    , _data(std::move(p._data))
{
}

template<typename T>
void TestProbe<T>::operator()(T const& data)
{
    PANDA_LOG_DEBUG << "TestProbe(" << _id << ")) called";
    _data->push_back(data.shared_from_this());
}

template<typename T>
std::shared_ptr<const T> TestProbe<T>::data()
{
    std::shared_ptr<const T> r;
    if(_data->empty()) return r;
    r =  _data->front();
    _data->pop_front();
    return r;
}

} // namespace exporters
} // namespace io
} // namespace cheetah
} // namespace ska
