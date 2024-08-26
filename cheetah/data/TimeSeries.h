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
#ifndef SKA_CHEETAH_DATA_TIMESERIES_H
#define SKA_CHEETAH_DATA_TIMESERIES_H

#include "cheetah/data/Series.h"
#include "cheetah/data/Units.h"
#include "cheetah/data/DefaultAllocator.h"
#include "cheetah/utils/Architectures.h"

#include "panda/DataChunk.h"

namespace ska {
namespace cheetah {
namespace data {


/**
 * @brief      Class for time series data.
 *
 * @tparam     Arch       The architecture on which the time series will be stored
 * @tparam     ValueType  The underlying value type of the time series
 * @tparam     Alloc      The allocator type of the time series
 */
template <typename Arch, typename ValueType, typename Alloc=typename DefaultAllocator<Arch,ValueType>::type>
class TimeSeries
    : public Series<Arch, ValueType, Alloc>
    , public panda::DataChunk<TimeSeries<Arch, ValueType, Alloc>>
{
        typedef Series<Arch, ValueType, Alloc> BaseT;

    public:
        typedef Alloc Allocator;

    public:
        /**
         * @brief      Construct a regularly sampled time series
         *
         * @details    The time series is initialised to have zero size
         *             and a default sampling interval of 1 second is set.
         */
        explicit TimeSeries(Allocator const& alloc=Allocator());

        /**
         * @brief      Construct a regularly sampled time series
         *
         * @details    The time series is initialised to have the specified size
         *             and a default sampling interval of 1 second is set.
         */
        explicit TimeSeries(std::size_t size, Allocator const& alloc=Allocator());

        /**
         * @brief      Construct a regularly sampled time series copying data from device memory
         */
        template<typename OtherArch, typename OtherValueT, typename OtherAlloc>
        explicit TimeSeries(TimeSeries<OtherArch, OtherValueT, OtherAlloc> const& copy, Alloc const& allocator = Allocator());

        /**
         * @brief      Construct a time series with a sampling interval
         *
         * @details    The time series is initialised to have zero size
         *
         * @param      dt  The sampling interval of the time series.
         */
        TimeSeries(TimeType const& dt, std::size_t size=0, Allocator const& alloc=Allocator());
        ~TimeSeries();

        /**
         * @brief copy assignment operator
         */
        template<typename OtherArch, typename OtherValueT, typename OtherAlloc>
        TimeSeries& operator=(TimeSeries<OtherArch, OtherValueT, OtherAlloc> const& copy);

        /**
         * @brief      Retrive the sampling interval
         *
         * @return     The sampling interval of the time series
         */
        TimeType const& sampling_interval() const;

        /**
         * @brief      Set the sampling interval
         *
         * @param      dt  The new sampling interval
         */
        void sampling_interval(TimeType const& dt);

    private:
        TimeType _sampling_interval;
};

} // namespace data
} // namespace cheetah
} // namespace ska

#include "cheetah/data/detail/TimeSeries.cpp"

#endif // SKA_CHEETAH_DATA_TIMESERIES_H
