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
#ifndef SKA_CHEETAH_DATA_FREQUENCYSERIES_H
#define SKA_CHEETAH_DATA_FREQUENCYSERIES_H

#include "cheetah/data/Series.h"
#include "cheetah/data/Units.h"
#include "cheetah/data/DefaultAllocator.h"
#include "cheetah/utils/Architectures.h"

#include "panda/DataChunk.h"

namespace ska {
namespace cheetah {
namespace data {

/**
 * @brief A container of Fourier series data
 *
 * @details A contiguous block of data ordered by Fourier frequency.
 *
 * The FrequencySeries class wraps a generic Series class providing
 * methods and members that describe a series of data (powers, amplitudes, etc.)
 * ordered by Fourier frequency.
 *
 * Note that by convention the frequency value of each bin is referenced to the
 * centre of that bin. This means that the zero'th bin of the series spans the range
 * (-frequecy_step/2,frequecy_step/2). This is important to account for when performing
 * operations which require exact knowledge of the frequencies of each bin.
 */

template <typename Arch, typename ValueType, typename Alloc=typename DefaultAllocator<Arch,ValueType>::type>
class FrequencySeries
    : public Series<Arch, ValueType, Alloc>
    , public panda::DataChunk<FrequencySeries<Arch, ValueType, Alloc>>
{
        typedef Series<Arch, ValueType, Alloc> BaseT;

    public:
        typedef Alloc Allocator;
        typedef Arch Architecture;

    public:
        /**
         * @brief      Construct a default frequency series (of size 0)
         */
        FrequencySeries(Allocator const& allocator = Allocator());

        /**
         * @brief      Construct a default frequency series
         */
        FrequencySeries(std::size_t size, Allocator const& allocator = Allocator());

        /**
         * @brief      Construct a regularly sampled frequency series copying data from device memory
         */
        template<typename OtherArch, typename OtherValueT, typename OtherAlloc>
        explicit FrequencySeries(FrequencySeries<OtherArch, OtherValueT, OtherAlloc> const& copy, Alloc const& allocator = Allocator());

        /**
         * @brief      Construct a frequency series with a given sampling interval
         *
         * @param      df  The frequency step between neighbouring bins in the series
         */
        FrequencySeries(FourierFrequencyType const& df, std::size_t size, Allocator const& allocator = Allocator());

        /**
         * @brief      Construct a frequency series with a given sampling interval
         *
         * @param      df  The frequency step between neighbouring bins in the series
         */
        FrequencySeries(FourierFrequencyType const& df, Allocator const& allocator = Allocator());
        ~FrequencySeries();

        /**
         * @brief copy assignment operator
         */
        template<typename OtherArch, typename OtherValueT, typename OtherAlloc>
        FrequencySeries& operator=(FrequencySeries<OtherArch, OtherValueT, OtherAlloc> const& copy);

        /**
         * @brief      Retrieve the frequency step of the series
         *
         * @return     The frequency step between neighbouring bins in the series
         */
        FourierFrequencyType const& frequency_step() const;

        /**
         * @brief      Set the frequency step of the series
         *
         * @param      df    The new frequency step for the series
         */
        void frequency_step(FourierFrequencyType const& df);


        data::FourierFrequencyType const bin_to_frequency(std::size_t idx) const;
        data::TimeType const bin_to_period(std::size_t idx) const;
        std::size_t frequency_to_bin(data::FourierFrequencyType const& frequency) const;

    private:
        FourierFrequencyType _frequency_step;
};


} // namespace data
} // namespace cheetah
} // namespace ska

#include "cheetah/data/detail/FrequencySeries.cpp"

#endif // SKA_CHEETAH_DATA_FREQUENCYSERIES_H

