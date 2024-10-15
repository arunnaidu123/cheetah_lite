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
#ifndef SKA_CHEETAH_UTILS_SINGLETHREAD_H
#define SKA_CHEETAH_UTILS_SINGLETHREAD_H

#include <thread>
#include <vector>
#include <mutex>
#include <functional>
#include <deque>
#include <condition_variable>

namespace ska {
namespace cheetah {
namespace utils {

/**
 * @brief
 *    MultiThread - extension of std::thread to permit NUMA optimisation when available
 *
 * @details
 *
 */
class MultiThread
{
    public:

        MultiThread();
        MultiThread(MultiThread const&) = delete;
        MultiThread(MultiThread&&) = delete;

        /**
         * @brief construct the thread and bind to the core specified by 'affinity'. If affinity == -1 the thread is not bound to any particular core
         */
        ~MultiThread();

        template<typename Function, typename... Args>
        void add_job(Function&& fn, Args... args);

        /**
         * @brief instruct the thread to run on a specific core - forwards to pthread_setaffinity_np on LINUX otherwise has no effect
         */
        void set_core_affinity(unsigned const& affinity);

        void terminate();
        void ready(unsigned id);
        void finish(unsigned id);
        unsigned number_of_jobs() const;

    protected:
        void wait(unsigned id);
        void reset(unsigned id);
        bool status(unsigned id);


    private:
        unsigned _njobs;
        bool _terminate;
        std::vector<bool> _ready;
        std::vector<std::mutex> _mutex;
        std::mutex _mt;
        std::condition_variable _cv;
        std::vector<std::thread> _threads;
};

} // namespace utils
} // namespace cheetah
} // namespace ska
#include "detail/MultiThread.cpp"

#endif // SKA_CHEETAH_UTILS_MULTITHREAD_H