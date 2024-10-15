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

#ifndef SKA_CHEETAH_MODULES_DDTR_AGGREGATIONBUFFERFILLER_H
#define SKA_CHEETAH_MODULES_DDTR_AGGREGATIONBUFFERFILLER_H

#include "AggregationBuffer.h"
#include <functional>
#include <type_traits>
#include <chrono>
namespace ska {
namespace cheetah {
namespace modules {
namespace ddtr {

/**
 * @brief
 *    Fill up AggregateBuffers with objects, and call listeners when full
 * @tparam DataT The datatypes (chunks) of data that will be passed into the buffer
 *         and make up its composition
 * @tparam BufferFactory
 *         The object to generate the begin and end iterators when data is
 *         passed from the insert(DataType const&i data) function.
 *         data will be passed to the BufferFactory's begin(data) and end(data)
 *         methods which should return the required iterators for copying the
 *         data. The default BeginEndFactory will simply call data.begin() and
 *         data.end().
 *         In addition the BufferFactory can also have create_buffer()
 *         which returns the AgregationBuffer data type, size() and resize methods
 *         which return and take in the custom size type respectively and
 *         number_of_elements method converts the custom sizetype to
 *         size number of elements. If the create_buffer method is not present a default
 *         returning a panda::Buffer will be provided. If any of the other three methods are not defined
 *         methods using std::size_t as th size type will be generated.
 */

template<typename NumericalRep>
class AggregationBufferFiller
{
    public:
        typedef AggregationBuffer<NumericalRep> AggregationBufferType;
        typedef std::function<void(std::shared_ptr<AggregationBufferType>)> FullBufferHandlerT;

    public:
        /**
         * @param handler to be called whenever the buffer is full
         * @param args any other arguments will be forwarded to the BufferFactory constructor
         */
        template<typename... Args>
        AggregationBufferFiller(FullBufferHandlerT handler, Args&& ... args);
        ~AggregationBufferFiller();

    private:
        // disable the copy constructor
        AggregationBufferFiller(const AggregationBufferFiller<NumericalRep>&) = delete;
        AggregationBufferFiller<NumericalRep>& operator=(const AggregationBufferFiller<NumericalRep>&) = delete;

    public:
        /**
         * @brief copy the data into the current buffer at the current insertion location
         *        The insertion pointer is then updated to point to the end of this data.
         * @param InsertDataType must be iable to static_cast to DataType const&
         */
        template<typename InsertDataType>
        void insert( InsertDataType const& data );

        /**
         * @brief insert the passed Data
         */
        template<typename InsertDataType>
        AggregationBufferFiller<NumericalRep>& operator<<( InsertDataType const& data );

        /**
         * @brief set the number of elements consequtive buffers should overlap
         */
        void set_overlap(std::size_t const& overlap);

        /**
         * @brief return the current overlsp size between buffers
        */
        std::size_t overlap() const;

        /**
         * @brief send the buffer to the handler immediately, without waiting until
         *        it is full.
         * @return true if the handler was called, false otherwise
         */
        bool flush();

        /**
         * @brief return the maximium size of the buffer being filled
         */
        std::size_t size() const;

        /**
         * @brief return the remaining space that can be filled of the buffer being filled
         */
        std::size_t remaining_capacity() const;

        /**
         * @brief set a new full buffer handler
         */
        void full_buffer_handler(FullBufferHandlerT const& handler);

        /**
         * @brief resize the current buffer
         */
        void resize(std::size_t number_of_spectra, std::size_t number_of_channels) const;

        /**
         * @brief return the timefrequency metadata
         */
        typename data::TimeFrequencyMetadata const& metadata() const;

        /**
         * @brief set time frequency metadata
         */
        void metadata(typename data::TimeFrequencyMetadata const& metadata);

        /**
         * @brief add nano seconds the time object
         */
        void add_nsec(struct timespec& temp, long nsec);

    private:
        FullBufferHandlerT _fn;
        std::size_t _overlap;
        std::shared_ptr<AggregationBufferType> _current;
        struct timespec _t0;
};

} // namespace ddtr
} // namespace modules
} // namespace cheetah
} // namespace ska

#include "AggregationBufferFiller.cpp"

#endif // SKA_CHEETAH_MODULES_DDTR_AGGREGATIONBUFFERFILLER_H
