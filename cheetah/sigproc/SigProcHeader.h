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
#ifndef CHEETAH_SIGPROC_SIGPROCHEADER_H
#define CHEETAH_SIGPROC_SIGPROCHEADER_H


#include "pss/astrotypes/sigproc/Header.h"
#include "cheetah/data/Units.h"
#include "cheetah/utils/chrono.h"
#include <boost/units/quantity.hpp>
#include <vector>

namespace ska {
namespace cheetah {
namespace sigproc {

/**
 * @brief
 *    struct for the header information of a SigProc file
 */

class SigProcHeader : public pss::astrotypes::sigproc::Header
{
        typedef pss::astrotypes::sigproc::Header BaseT;

    public:
        typedef boost::units::quantity<boost::units::si::time, double> TimeType;
        typedef data::FrequencyType FrequencyType;

    public:
        SigProcHeader();
        ~SigProcHeader();

       /**
        * @brief set the start_time (corresponding to the time of the first sample
        */
        void start_time(typename utils::ModifiedJulianClock::time_point const&);
        typename utils::ModifiedJulianClock::time_point start_time() const;

       /**
        * @brief returns the frequencies each channel represents
        */
        std::vector<FrequencyType> const& frequency_channels() const;

       /**
        * @brief set frequencies each channel represents
        */
        void frequency_channels(std::vector<FrequencyType> const&);

};

/// generate summary info in plain ascii text
std::ostream& operator<<(std::ostream& os, SigProcHeader const&);

} // namespace sigproc
} // namespace cheetah
} // namespace ska

#endif // CHEETAH_SIGPROC_SIGPROCHEADER_H 
