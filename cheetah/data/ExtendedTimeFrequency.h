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
#ifndef SKA_CHEETAH_DATA_EXTENDEDTIMEFREQUENCY_H
#define SKA_CHEETAH_DATA_EXTENDEDTIMEFREQUENCY_H

#include "cheetah/data/TimeFrequencyCommon.h"
#include <pss/astrotypes/types/ExtendedTimeFrequency.h>

namespace ska {
namespace cheetah {
namespace data {


struct ExtendedTimeFrequencyTag {}; // empty base class for std::is_base_of comparisons

/**
 * @brief Wrapper around a TimeFrequency type object (or shared_ptr to such) mainiting the TimeFrequency interface
 * @details Allows for easy composition.
 */

template<typename TimeFrequencyT>
class ExtendedTimeFrequency : public pss::astrotypes::types::ExtendedTimeFrequency<std::shared_ptr<TimeFrequencyT>>
                            , public ExtendedTimeFrequencyTag
{
        typedef pss::astrotypes::types::ExtendedTimeFrequency<std::shared_ptr<TimeFrequencyT>> BaseT;

    protected:
        typedef typename BaseT::Type Type;

    public:
        typedef Type TimeFrequencyType;
        typedef typename Type::TimeType TimeType;
        typedef typename Type::TimePointType TimePointType;
        typedef typename Type::FrequencyType FrequencyType;
        typedef typename Type::Architecture Architecture;
        typedef typename Type::NumericalRep NumericalRep;

    public:
        ExtendedTimeFrequency(std::shared_ptr<TimeFrequencyT> const&);
        ~ExtendedTimeFrequency();

        /**
         * @brief conversion operator to the underlying TimeFrequency type
         * @details static casting to the tf block and using the reference
         *          may prove more efficient way of accessing the TF interface
         *          if you are calling more than one method.
         */
        operator Type const&() const;

        /**
         * @brief      get the start time of first spectrum
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
         * @returns the sample interval in seconds
         */
        TimeType sample_interval() const;

        /**
         * @set the sample interval in seconds
         */
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
        void set_channel_frequencies_const_width(FrequencyType const& start, FrequencyType const& delta);

        /**
         * @brief sets the value of the specified channel to the provided
         * value across all time samples
         */
        void set_channel(unsigned channel_number, NumericalRep const& value);
};

} // namespace data
} // namespace cheetah
} // namespace ska

#include "detail/ExtendedTimeFrequency.cpp"

#endif // SKA_CHEETAH_DATA_EXTENDEDTIMEFREQUENCY_H
