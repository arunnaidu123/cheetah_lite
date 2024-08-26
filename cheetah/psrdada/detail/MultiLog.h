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
#ifndef SKA_CHEETAH_PSRDADA_MULTILOG_H
#define SKA_CHEETAH_PSRDADA_MULTILOG_H

#include "dada_hdu.h" // n.b we include this to get the correct multilog.h, which is too generic a name
#include <string>

namespace ska {
namespace cheetah {
namespace psrdada {
namespace detail {

/**
 * @brief      A class for wrapping multilog_t instances
 *             required for logging with the underlying
 *             DADA API.
 */
class MultiLog
{
    public:
        /**
         * @brief      Create a new instance
         *
         * @param[in]  name  The name to give this logger
         */
        explicit MultiLog(std::string name);
        MultiLog(MultiLog const&) = delete;
        ~MultiLog();

        /**
         * @brief      Get a native handle to the wrapped multilog_t pointer
         */
        multilog_t* native_handle();

        /**
         * @brief      Write to the log
         *
         * @param[in]  priority   The priority (0, 1, 2...)
         * @param[in]  format     The format string
         * @param[in]  ...        Parameters for the format string
         *
         * @tparam     Args       The types of the parameters for the format string
         */
        template<class... Args>
        void write(int priority, const char* format, Args&&... args);

        /**
         * @brief      Return the name of the logger
         */
        std::string const& name() const;

    private:
        void open();
        void close();

    private:
        std::string _name;
        multilog_t* _log;
        bool _open;

};

} // namespace detail
} // namespace psrdada
} // namespace cheetah
} // namespace ska

#include "cheetah/psrdada/detail/MultiLog.cpp"

#endif // SKA_CHEETAH_PSRDADA_MULTILOG_H
