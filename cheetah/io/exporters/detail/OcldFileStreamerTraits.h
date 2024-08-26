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
#ifndef SKA_CHEETAH_IO_EXPORTERS_OCLDFILESTREAMERTRAITS_H
#define SKA_CHEETAH_IO_EXPORTERS_OCLDFILESTREAMERTRAITS_H

#include "cheetah/data/Ocld.h"

namespace ska {
namespace cheetah {
namespace io {
namespace exporters {

/**
 * @brief
 *    class to describe the format of data::Ocld output files
 *    as a panda::OutputFileStreamConcept
 * @details
 */

template<typename NumericalT>
class OcldFileStreamerTraits
{
    public:
        OcldFileStreamerTraits();
        ~OcldFileStreamerTraits();

        static inline
        bool consistent_with_existing_file(data::Ocld<NumericalT> const&);

        static inline
        void headers(std::ostream& os, data::Ocld<NumericalT> const&);

        static inline
        void footers(std::ostream&) {};

        static
        void write(std::ostream& os, data::Ocld<NumericalT> const&);
};

} // namespace exporters
} // namespace io
} // namespace cheetah
} // namespace ska

#include "cheetah/io/exporters/detail/OcldFileStreamerTraits.cpp"

#endif // SKA_CHEETAH_IO_EXPORTERS_OCLDFILESTREAMERTRAITS_H
