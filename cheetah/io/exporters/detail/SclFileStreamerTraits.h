/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2024 The SKA organisation
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
#ifndef SKA_CHEETAH_IO_EXPORTERS_SCLFILESTREAMERTRAITS_H
#define SKA_CHEETAH_IO_EXPORTERS_SCLFILESTREAMERTRAITS_H

#include "cheetah/data/Scl.h"

namespace ska {
namespace cheetah {
namespace io {
namespace exporters {

/**
 * @brief
 *    class to describe the format of data::Scl output files
 *    as a panda::OutputFileStreamConcept
 * @details
 */

class SclFileStreamerTraits
{
    public:
        SclFileStreamerTraits();
        ~SclFileStreamerTraits();

        static inline
        bool consistent_with_existing_file(data::Scl const&) { return true; }

        static inline
        void headers(std::ostream& os, data::Scl const&);

        static inline
        void footers(std::ostream&) {};

        static
        void write(std::ostream& os, data::Scl const&);

};

void SclFileStreamerTraits::headers(std::ostream& os, data::Scl const&)
{
    os << "p(" << typename data::Scl::CandidateType::TimeType::unit_type()
       << ")\tp_dot(" << typename data::Scl::CandidateType::SecPerSecType::unit_type()
       << ")\tdm\t"
       << "harmonic\t"
       << "width(" << typename data::Scl::CandidateType::TimeType::unit_type() << ")\tsigma\n";
}

} // namespace exporters
} // namespace io
} // namespace cheetah
} // namespace ska

#endif // SKA_CHEETAH_IO_EXPORTERS_SCLFILESTREAMERTRAITS_H
