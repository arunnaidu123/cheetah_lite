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
#ifndef SKA_CHEETAH_IO_PRODUCERS_RCPT_SKASELECTOR_H
#define SKA_CHEETAH_IO_PRODUCERS_RCPT_SKASELECTOR_H

#include <string>
#include <vector>

namespace ska {
namespace cheetah {
namespace io {
namespace producers {
namespace rcpt {

/**
 * @brief Utility to specialise by telescope type and provision if runtime tags
 *        for low and mid telescopes
 * @details
 */

class SkaSelector
{
    public:
        /**
         * @brief Use these tpes to specialise template to a specific stream
         *        Explicit specialisations can be chosen at runtime using the
         *        select method and the string id (ska_mid or ska_low).
         */
        enum class Stream { SkaLow, SkaMid };

    public:
        /**
         * @brief return the names of the available stream identifiers
         */
        static
        std::vector<std::string> available();

        /**
         * @brief  function to map stream names (as strings) to the underlying types
         * @tparam ReturnT - a common type returned by the Executor for any type
         * @tparam Executor a template that defines an exec(Args&&...) method for any
         *         Stream type passed
         * @tparam Args forwarded on to the Executor<SelecteType>::exec method
         */
        template<typename ReturnT, template <Stream> class Executor, typename... Args>
        static
        ReturnT select(std::string const& stream_id, Args&&... args);
};


} // namespace rcpt
} // namespace producers
} // namespace io
} // namespace cheetah
} // namespace ska
#include "cheetah/io/producers/rcpt/detail/SkaSelector.cpp"

#endif // SKA_CHEETAH_IO_PRODUCERS_RCPT_SKASELECTOR_H
