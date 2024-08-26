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
#ifndef SKA_CHEETAH_DATA_CANDIDATEWINDOW_H
#define SKA_CHEETAH_DATA_CANDIDATEWINDOW_H

#include "cheetah/data/Units.h"
#include <cstddef>

namespace ska {
namespace cheetah {
namespace data {

/**
 * @brief define a window of data surrounding a single pulse candidate
 */

class CandidateWindow
{
    private:
        typedef boost::units::quantity<MilliSeconds,double> MsecTimeType;

    public:
        CandidateWindow();
        CandidateWindow(MsecTimeType ms_before, MsecTimeType ms_after);
        ~CandidateWindow();

        /**
         * @brief the milliseconds required before the cnadidate start time
         */
        MsecTimeType ms_before() const;
        void ms_before(MsecTimeType);

        /**
         * @brief the milliseconds required after the candidate end time
         */
        MsecTimeType ms_after() const;
        void ms_after(MsecTimeType);

    private:
        MsecTimeType _pre;
        MsecTimeType _post;
};


} // namespace data
} // namespace cheetah
} // namespace ska

#endif // SKA_CHEETAH_DATA_CANDIDATEWINDOW_H
