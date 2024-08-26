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
#ifndef SKA_CHEETAH_DATA_TIMEFREQUENCYMETADATA_H
#define SKA_CHEETAH_DATA_TIMEFREQUENCYMETADATA_H

#include "cheetah/data/Units.h"
#include "cheetah/utils/chrono.h"
#include "pss/astrotypes/multiarray/DimensionSize.h"
#include <vector>
#include <utility>

namespace ska {
namespace cheetah {
namespace data {

template<typename T> using DimensionSize = pss::astrotypes::DimensionSize<T>;
using pss::astrotypes::units::Frequency;
using pss::astrotypes::units::Time;

/**
 * @brief Metadata for TimeFrequency and FrequencyTime classes
 * @details
 */

class TimeFrequencyMetadata
{
    public:
        typedef boost::units::quantity<MegaHertz, double> FrequencyType;
        typedef boost::units::quantity<boost::units::si::time, double> TimeIntervalType;
        typedef cheetah::utils::ModifiedJulianClock::time_point TimePointType;

    public:
        TimeFrequencyMetadata();
        TimeFrequencyMetadata(TimeFrequencyMetadata const& copy);
        TimeFrequencyMetadata& operator=(TimeFrequencyMetadata const& copy);

        /**
         * @returns the sample interval in seconds
         */
        TimeIntervalType const& sample_interval() const;

        /**
         * @set the sample interval in seconds
         */
        void sample_interval(TimeIntervalType dt);

        /**
         * @returns a vector of frequencies that are represented in each time sample
         */
        std::vector<FrequencyType> const& channel_frequencies() const;

        /**
         * @returns returns the lowest and highest frequencies represented in channel_frequenceis
         */
        std::pair<FrequencyType, FrequencyType> low_high_frequencies() const;

        /**
         * Set the frequency index based on arbitary channels
         */
        template<class InputIterator>
        void channel_frequencies(InputIterator begin, InputIterator const end);

        /**
         * @brief Set the frequency index based on evenly spaced channels
         * @details
         *  Assumes that the number_of_channels has already been set
         * @Param pss::astrotypes::DimensionSize<Frequency> is the number of frequency channels
         */
        void channel_frequencies_const_width(FrequencyType const& start
                                            , FrequencyType const& delta
                                            , pss::astrotypes::DimensionSize<Frequency>);

        /**
         * @returns the absolute time the first time sample corresponds to.
         */
        TimePointType const& start_time() const;

        /**
         * @returns the absolute time the Nth time sample corresponds to.
         */
        TimePointType start_time(std::size_t offset) const;

        /**
         * @brief      Set the start time of the instance
         *
         * @param      start_time  The start time (MJD) of the instance
         */
        void start_time(TimePointType const& start_time);

    private:
        std::vector<FrequencyType> _frequency_index;
        TimeIntervalType _sample_interval;
        TimePointType _start_time;
};


} // namespace data
} // namespace cheetah
} // namespace ska
#include "detail/TimeFrequencyMetadata.cpp"

#endif // SKA_CHEETAH_DATA_TIMEFREQUENCYMETADATA_H
