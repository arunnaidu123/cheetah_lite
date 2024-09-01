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
#ifndef SKA_CHEETAH_MODULES_DDTR_AGGREGATIONBUFFER_H
#define SKA_CHEETAH_MODULES_DDTR_AGGREGATIONBUFFER_H

#include "panda/Buffer.h"
#include "cheetah/data/FrequencyTime.h"
#include "cheetah/data/TimeFrequencyCommon.h"
#include <vector>
#include <type_traits>

namespace ska {
namespace cheetah {
namespace modules {
namespace ddtr {

/**
 * @brief
 *    Aggregate together data chunks into a larger Buffer.
 *
 * @details
 *    Aggregates the data from several different DataType objects whilst maintaining references to the
 *    original DataType objects.
 *    The Aggregated buffer is contiguous in memory and will be equal or a subset of the data blocks in the
 *    composition obects.
 *
 * @verbatim
   Inserted Data(composition())  [ DataType #0 ][ DataType #1 ][ DataType #2 ][ DataType #3 ] ....
   Buffer                             [              BufferType                  ]
                                      ^
                                      |
                                    data()

                                 |----|------------------------------------------|
                                   ^                       ^
                                   |                       |
                                   |                    data_size()
                              offset_first_block()
  @endverbatim
 *
 *
 * @param BufferType the underlying type of the data in the Buffer e.g. float, double etc
 * \ingroup config
 * @{
 */

template<typename NumericalRep>
class AggregationBuffer
{
    public:
        typedef typename std::vector<NumericalRep>::iterator iterator;
        typedef typename std::vector<NumericalRep>::const_iterator const_iterator;
        typedef typename data::FrequencyTime<cheetah::Cpu, NumericalRep> FrequencyTimeType;
        typedef typename data::TimeFrequency<cheetah::Cpu, NumericalRep> TimeFrequencyType;
        typedef typename FrequencyTimeType::FrequencyType FrequencyType;
        typedef typename std::unique_ptr<std::vector<NumericalRep>> BufferType;
        typedef boost::units::quantity<boost::units::si::time, double> TimeType;
        typedef cheetah::utils::ModifiedJulianClock::time_point TimePointType;

    public:
        AggregationBuffer(std::size_t number_of_spectra, std::size_t number_of_channels);
        AggregationBuffer();
        AggregationBuffer(AggregationBuffer&&);
        AggregationBuffer(AggregationBuffer const&) = delete;
        ~AggregationBuffer();

        AggregationBuffer& operator=(AggregationBuffer&&) = default;

        std::size_t time_stride() const;

        void time_stride(std::size_t const& stride);

        std::size_t frequency_stride() const;

        void frequency_stride(std::size_t const& stride);

        /**
         * @brief transfers size bytes of data, and assosiated composition information from the end of the
         *        buffer to the destination_buffer
         */
        void transfer(std::size_t size, AggregationBuffer<NumericalRep>& destination_buffer) const;

        /**
         * @brief return a pointer to the  position of the underlying data buffer
         * @param offset from the position
         */
        iterator data(unsigned offset=0);
        const_iterator data(unsigned offset=0) const;

        /**
         * @brief iterators for beginning to end of the data
         */
        iterator begin();
        const_iterator begin() const;
        const_iterator cbegin() const;
        iterator begin(unsigned frequency_channel);
        const_iterator begin(unsigned frequency_channel) const;
        const_iterator cbegin(unsigned frequency_channel) const;
        iterator end();
        const_iterator end() const;
        const_iterator cend() const;
        iterator end(unsigned frequency_channel);
        const_iterator end(unsigned frequency_channel) const;
        const_iterator cend(unsigned frequency_channel) const;

        /**
         * @brief return a pointer to the  buffer object
         */
        BufferType& buffer();
        BufferType const& buffer() const;
        void buffer(BufferType const& buffer);

        /**
         * @brief copy the data into the current buffer at the current insertion location
         *        The insertion pointer is then updated to point to the end of this data.
         */
        void insert(std::shared_ptr<TimeFrequencyType> const& object );
        void insert(std::shared_ptr<FrequencyTimeType> const& object );
        void insert(TimeFrequencyType const& object);
        void insert(FrequencyTimeType const& object );

        /**
         * @brief return the unused number of time samples in the buffer
         */
        std::size_t remaining_capacity() const;

        /**
         * @brief return the current number of time samples inserted in the buffer
         */
        std::size_t data_size() const;

        /**
         * @brief return the total number of time samples that can be stored in the buffer.
        */
        std::size_t capacity() const;

        std::size_t number_of_spectra() const;

        std::size_t number_of_channels() const;

        void swap(AggregationBuffer& b);

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
        typename data::TimeFrequencyMetadata const& metadata() const;

        /**
         * @brief set the metadata
         */
        void metadata(typename data::TimeFrequencyMetadata const& metadata);

        void resize(std::size_t number_of_spectra, std::size_t number_of_channels);



    private:
        BufferType _buffer;
        std::size_t _time_stride;
        std::size_t _frequency_stride;
        std::size_t _number_of_spectra;
        std::size_t _number_of_channels;
        std::size_t _current_time;
        typename data::TimeFrequencyMetadata _metadata;
};

} // namespace ddtr
} // namespace modules
} // namespace cheetah
} // namespace ska

#include "AggregationBuffer.cpp"

#endif // SKA_CHEETAH_MODULES_DDTR_AGGREGATIONBUFFER_H
