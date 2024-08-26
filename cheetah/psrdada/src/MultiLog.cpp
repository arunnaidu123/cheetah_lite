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
#include "cheetah/psrdada/detail/MultiLog.h"


namespace ska {
namespace cheetah {
namespace psrdada {
namespace detail {

MultiLog::MultiLog(std::string name)
    : _name(name)
    , _log(0)
    , _open(false)
{
    open();
}

MultiLog::~MultiLog()
{
    close();
}

void MultiLog::open()
{
    _log = multilog_open(_name.c_str(),0);
    _open = true;
}

void MultiLog::close()
{
    multilog_close(_log);
    _open = false;
}

multilog_t* MultiLog::native_handle()
{
    return _log;
}

std::string const& MultiLog::name() const
{
    return _name;
}

} // namespace detail
} // namespace psrdada
} // namespace cheetah
} // namespace ska
