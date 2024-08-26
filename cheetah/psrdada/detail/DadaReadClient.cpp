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

#include "cheetah/psrdada/DadaReadClient.h"
#include "panda/Error.h"
#include "panda/Log.h"
#include <streambuf>
#include <functional>

namespace ska {
namespace cheetah {
namespace psrdada {

template <typename Iterator, typename DataType>
Iterator& DadaReadClient::read(Iterator& begin, Iterator const& end)
{
    std::shared_ptr<bool> destructor_flag(_destructor_flag);
    while (begin != end)
    {
        if(*destructor_flag)
        {
            return begin;
        }
        if (!_current_reader || _current_reader->empty())
        {
            if (!acquire_data_block())
            {
                return begin;
            }
        }
        begin = _current_reader->read<DataType>(begin, end);
    }
    return begin;
}

template <typename CallBackFunctor>
void DadaReadClient::next_sequence(CallBackFunctor callback)
{
    flush();
    _engine.post(std::bind(&DadaReadClient::do_next_sequence<CallBackFunctor>,this,_destructor_flag, callback));
}

template <typename CallBack>
void DadaReadClient::do_next_sequence(std::shared_ptr<bool> destructor_flag, CallBack callback)
{
    if (*destructor_flag)
    {
        return;
    }

    PANDA_LOG_DEBUG << id() << "Acquiring next header block";
    uint64_t nbytes = 0;
    try
    {
        char* tmp;
        do
        {
            tmp = ipcbuf_get_next_read(_hdu->header_block, &nbytes);
            if (!tmp)
            {
                if (*destructor_flag)
                {
                    return;
                }
                _log.write(LOG_ERR, "Could not acquire next header block\n");
            }
        } while(!tmp);
        auto deleter = [&](std::istream* i)
        {
            delete i;
            if (*destructor_flag)
            {
                return;
            }
            PANDA_LOG_DEBUG << id() << "Releasing header block";
            if (ipcbuf_mark_cleared(_hdu->header_block) < 0)
            {
                _log.write(LOG_ERR, "Could not mark cleared header block\n");
                PANDA_LOG_ERROR << "Could not mark cleared header block";
            }
        };
        detail::membuf sbuf(tmp, tmp + nbytes);
        std::unique_ptr<std::istream, decltype(deleter)> header(new std::istream(&sbuf), deleter);
        callback(*header,std::current_exception());
    }
    catch (...)
    {
        std::stringstream  sbuf(" ");
        callback(sbuf, std::current_exception());
    }
}

std::shared_ptr<bool> DadaReadClient::stopped() const
{
    return _destructor_flag;
}

} // namespace psrdada
} // namespace cheetah
} // namespace ska
