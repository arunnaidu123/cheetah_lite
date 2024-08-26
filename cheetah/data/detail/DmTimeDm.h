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
#ifndef SKA_CHEETAH_DATA_DMTIMEBUFFERDM_H
#define SKA_CHEETAH_DATA_DMTIMEBUFFERDM_H

#include "cheetah/data/DedispersionMeasure.h"
#include "cheetah/data/Units.h"
#include <memory>

namespace ska {
namespace cheetah {
namespace data {
namespace detail {

/**
 * @brief      Class that wraps a single DM trial from a DmTime object
 *
 * @details    This class provides an interface that allows for single DM trials
 *             to be extracted from a DmTime object and sent to an arbitrary
 *             memory location on an arbitrary device.
 *
 *             This class is not intended to be used directly, but should instead
 *             only be returned from a dereferenced DmTimeSliceIterator.
 */
template <typename DmTimeSliceType>
class DmTimeDm
{
    private:
        typedef DmTimeSliceType SliceType;
        typedef SecondsType<double> TimeType;

    public:
        /**
         * @brief      Create a new instance
         *
         * @param[in]  dm_idx  The dm index
         * @param[in]  parent  The parent DmTimeSlice object
         */
        DmTimeDm(std::size_t const dm_idx, std::shared_ptr<SliceType const> parent);
        ~DmTimeDm();

        /**
         * @brief      Copy the given DM trial to an arbitrary timeseries
         *
         * @details    The copy_to method will copy only up to the size of the timeseries
         *             provided. For objects using this method, it is necessary to check
         *             that the iterator returned by copy_to() matches the end() iterator
         *             of the timeseries. When they do not match, the timeseries should
         *             likely be resized or padded to ensure that all of the data it
         *             contains remains valid.
         *
         * @param      timeseries      The timeseries to be copied to
         *
         * @tparam     TimeSeriesType  The type of the timeseries
         *
         * @return     An iterator pointing to the last copied element in the time series
         *
         */
        template <typename TimeSeriesType>
        typename TimeSeriesType::Iterator copy_to(TimeSeriesType& timeseries) const;

        /**
         * @brief      Get the number of time samples in the associated DM trial
         *
         * @return     The number of samples in the DM trial
         */
        std::size_t number_of_samples() const;

        data::DedispersionMeasureType<float> dm() const;

        TimeType sampling_interval() const;

    private:
        std::size_t _dm_idx;
        std::shared_ptr<SliceType const> _parent;
};

} // namespace detail
} // namespace data
} // namespace cheetah
} // namespace ska

#include "cheetah/data/detail/DmTimeDm.cpp"

#endif // SKA_CHEETAH_DATA_DMTIMEDM_H
