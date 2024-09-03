/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2023 The SKA organisation
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

#ifndef SKA_CHEETAH_MODULES_SPDT_KLOTSKICOMMON_MSDESTIMATOR_H
#define SKA_CHEETAH_MODULES_SPDT_KLOTSKICOMMON_MSDESTIMATOR_H

#include "cheetah/modules/spdt/klotski/Config.h"
#include "cheetah/modules/ddtr/klotski/Ddtr.h"
#include "cheetah/modules/ddtr/klotski/detail/DedispersionStrategy.h"
#include "cheetah/utils/Architectures.h"

namespace ska {
namespace cheetah {
namespace modules {
namespace spdt {
namespace klotski_common {

/**
 * @brief MSD (mean, standard deviation)
 * @details MsdEstimator estimates the mean and standard deviation of the data in the DMTrails object
 */
template <typename SpdtTraits>
class MsdEstimator
{

    public:
        typedef typename SpdtTraits::DmTrialsType DmTrialsType;
    public:
        MsdEstimator(DmTrialsType const& data, unsigned int dm_index);
        ~MsdEstimator();

    public:
        /**
        * @brief return the mean of the data
        */
        double const& mean() const;

        /**
        * @brief return the standard deviation of the data
        */
        double const& stdev() const;

    private:
        double _mean;
        double _stdev;
};

} // namespace klotski_common
} // namespace spdt
} // namespace modules
} // namespace cheetah
} // namespace ska

#include "cheetah/modules/spdt/klotski_common/detail/MsdEstimator.cpp"
#endif // SKA_CHEETAH_MODULES_SPDT_KLOTSKICOMMON_MSDESTIMATOR_H