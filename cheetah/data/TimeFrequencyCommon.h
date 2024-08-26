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
#ifndef SKA_CHEETAH_DATA_TIMEFREQUENCYCOMMON_H
#define SKA_CHEETAH_DATA_TIMEFREQUENCYCOMMON_H

#include "cheetah/data/TimeFrequencyMetadata.h"
#include "panda/DataChunk.h"
#include <pss/astrotypes/types/TimeFrequency.h>

namespace ska {
namespace cheetah {
namespace data {

/**
 * @brief
 * @details
 */
template <class DerivedType>
class TimeFrequencyCommon : public panda::DataChunk<DerivedType>
{
    public:
        typedef boost::units::quantity<MegaHertz, double> FrequencyType;
        typedef boost::units::quantity<boost::units::si::time, double> TimeType;
        typedef cheetah::utils::ModifiedJulianClock::time_point TimePointType;

    public:
        TimeFrequencyCommon();

        template <typename OtherDerivedType>
        TimeFrequencyCommon(TimeFrequencyCommon<OtherDerivedType> const& tfc);

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

        /**
         * @returns the absolute time of TF block end
         */

         TimePointType end_time() const;

        /**
         * @returns the sample interval in seconds
         */
        TimeType sample_interval() const;

        /**
         * @set the sample interval in seconds
         */
        inline
        void sample_interval(TimeType dt);

        /**
         * @returns a vector of frequencies that are represented in each time sample
         */
        std::vector<FrequencyType> const& channel_frequencies() const;

        /**
         * @returns returns the lowest and highest frequencies represented in channel_frequenceis
         */
        std::pair<FrequencyType, FrequencyType> low_high_frequencies() const;

        /**
         * @brief Set the frequency index based on evenly spaced channels
         * @details
         *  Assumes that the number_of_channels has already been set
         */
        void set_channel_frequencies_const_width(FrequencyType const& start,
                                            FrequencyType const& delta);

        /**
         * Set the frequency index based on arbitary channels
         */
        template<class InputIterator>
        void set_channel_frequencies(InputIterator begin, InputIterator const end);

        /**
         * @returns returns the metadata
         */
        TimeFrequencyMetadata const& metadata() const;

        /**
         * @brief set the metadata
         */
        void metadata(TimeFrequencyMetadata const& metadata);

    private:
        TimeFrequencyMetadata _metadata;
};

/**
 * @brief Becomes the type T if the DataType passed has <Time, Frequency> dimensions (in that exact order)
 */
template<typename DataType, typename T=bool>
using EnableIfIsTimeFrequency = typename std::enable_if<pss::astrotypes::has_exact_dimensions<typename std::remove_reference<DataType>::type
                                                                                            , pss::astrotypes::units::Time
                                                                                            , pss::astrotypes::units::Frequency>::value, T>::type;
/**
 * @brief Becomes the type T if the DataType passed has <Frequency, Time> dimensions (in that exact order)
 */
template<typename DataType, typename T=bool>
using EnableIfIsFrequencyTime = typename std::enable_if<pss::astrotypes::has_exact_dimensions<typename std::remove_reference<DataType>::type
                                                                                            , pss::astrotypes::units::Frequency
                                                                                            , pss::astrotypes::units::Time>::value, T>::type;



} // namespace data
} // namespace cheetah
} // namespace ska

#include "cheetah/data/detail/TimeFrequencyCommon.cpp"
#endif // SKA_CHEETAH_DATA_TIMEFREQUENCYCOMMON_H
