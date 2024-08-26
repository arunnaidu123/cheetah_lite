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
#ifndef SKA_CHEETAH_IO_EXPORTERS_SPCCLSIGPROC_H
#define SKA_CHEETAH_IO_EXPORTERS_SPCCLSIGPROC_H

#include "cheetah/io/exporters/SpCclSigProcConfig.h"
#include "cheetah/sigproc/SigProcWriter.h"
#include "cheetah/data/SpCcl.h"
#include "cheetah/data/Units.h"
#include "cheetah/utils/chrono.h"

namespace ska {
namespace cheetah {
namespace io {
namespace exporters {

/**
 * @brief
 * @details
 */

template<typename NumericalRep>
class SpCclSigProc
{
    typedef pss::astrotypes::sigproc::Header HeaderType;
    typedef cheetah::utils::ModifiedJulianClock::time_point TimePointType;
    public:
        SpCclSigProc(SpCclSigProcConfig const& config);
        SpCclSigProc(SpCclSigProcConfig& config);

        ~SpCclSigProc();

        std::vector<TimePointType> start_times();

        /**
         * @brief Operator for iterating through candidates to extract filterbank data
         */
        bool operator << (data::SpCcl<NumericalRep> const& cand_list);

    private:
        SpCclSigProcConfig const& _config;
        sigproc::SigProcWriter<HeaderType> _writer;
        std::vector<TimePointType> _start_times;
};


} // namespace exporters
} // namespace io
} // namespace cheetah
} // namespace ska
#include "cheetah/io/exporters/detail/SpCclSigProc.cpp"
#endif // SKA_CHEETAH_IO_EXPORTERS_SPCCLSIGPROC_H
