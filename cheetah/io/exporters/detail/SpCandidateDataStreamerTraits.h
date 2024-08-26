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
#ifndef SKA_CHEETAH_IO_EXPORTERS_SPCANDIDATEDATAFILESTREAMERTRAITS_H
#define SKA_CHEETAH_IO_EXPORTERS_SPCANDIDATEDATAFILESTREAMERTRAITS_H

#include "cheetah/data/SpCandidateData.h"
#include <iomanip>

namespace ska {
namespace cheetah {
namespace io {
namespace exporters {

/**
 * @brief
 *      Traits class to describe SpCandidateData output file format
 * @details
 */

template<typename TimeFrequencyT>
class SpCandidateDataStreamerTraits
{
    public:
        SpCandidateDataStreamerTraits();
        ~SpCandidateDataStreamerTraits();

        static inline
        bool consistent_with_existing_file(data::SpCandidateData<TimeFrequencyT> const&) { return true; }

        static inline
        void headers(std::ostream& os, data::SpCandidateData<TimeFrequencyT> const&);

        static inline
        void footers(std::ostream&) {};

        static
        void write(std::ostream& os, data::SpCandidateData<TimeFrequencyT> const&);

    protected:
        static constexpr unsigned _column_width=14;
};


template<typename TimeFrequencyT>
void SpCandidateDataStreamerTraits<TimeFrequencyT>::headers(std::ostream& os, data::SpCandidateData<TimeFrequencyT> const&)
{
    os << "MJD(" << "decimal days"
       << ")" << std::setw(_column_width) << "dm(" << typename data::SpCandidateData<TimeFrequencyT>::CandidateType::Dm::unit_type()
       << ")" << std::setw(_column_width)
       << "width(" << typename data::SpCandidateData<TimeFrequencyT>::CandidateType::MsecTimeType::unit_type()
       << ")" << std::setw(_column_width)
       << "sigma\n";
}

} // namespace exporters
} // namespace io
} // namespace cheetah
} // namespace ska

#include "SpCandidateDataStreamerTraits.cpp"

#endif // SKA_CHEETAH_IO_EXPORTERS_SPCANDIDATEDATAFILESTREAMERTRAITS_H
