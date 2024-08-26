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
#include "cheetah/io/exporters/detail/SpeadLoggingAdapter.h"
#include <panda/Log.h>
#include <spead2/common_logging.h>


namespace ska {
namespace cheetah {
namespace io {
namespace exporters {

class SpeadLoggingAdapterImpl
{
    private:
        SpeadLoggingAdapterImpl();
        SpeadLoggingAdapterImpl(SpeadLoggingAdapterImpl const&)=delete;

        friend SpeadLoggingAdapterImpl& redirect_spead_logging();
};


SpeadLoggingAdapterImpl::SpeadLoggingAdapterImpl()
{
    spead2::set_log_function([](spead2::log_level log_level, std::string const& msg)
                             {
                                static const std::string prefix("spead: ");
                                switch(log_level) {
                                    case spead2::log_level::debug:
                                        PANDA_LOG_DEBUG << prefix << msg;
                                        break;
                                    case spead2::log_level::info:
                                        PANDA_LOG << prefix << msg;
                                        break;
                                    case spead2::log_level::warning:
                                        PANDA_LOG_WARN << prefix << msg;
                                        break;
                                };
                             }
                            );
}

SpeadLoggingAdapter::SpeadLoggingAdapter()
{
    redirect_spead_logging();
}

SpeadLoggingAdapterImpl& redirect_spead_logging()
{
    static SpeadLoggingAdapterImpl adapter;
    return adapter;
}

} // namespace exporters
} // namespace io
} // namespace cheetah
} // namespace ska
