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
#ifndef SKA_CHEETAH_DATA_TIMEFREQUENCYCONTAINER_H
#define SKA_CHEETAH_DATA_TIMEFREQUENCYCONTAINER_H

#include "TimeFrequency.h"
#include "panda/MultiIterator.h"
#include <memory>
#include <vector>

namespace ska {
namespace cheetah {
namespace data {


/**
 * @class TimeFrequencyContainer
 * @brief Provides the TimeFrequency API over a set of TimeFrequency objects
 */
template <class Arch, typename NumericalT>
class TimeFrequencyContainer
{
    public:
        typedef TimeFrequency<Arch, NumericalT> TimeFrequencyType;
        typedef typename TimeFrequencyType::FrequencyType FrequencyType;
        typedef typename TimeFrequencyType::TimeType TimeType;
        typedef typename TimeFrequencyType::TimePointType TimePointType;
        template<class ChannelIterationEndHandler=panda::NoOp, class TimeDiscontinuityHandler=panda::NoOp>
        struct ConstChannelIterator;
        struct ConstEndChannelIterator;

    public:
        TimeFrequencyContainer();
        ~TimeFrequencyContainer();

        /**
         * @brief add data to the collection
         */
        void add(TimeFrequencyType& data);
        void add(std::shared_ptr<TimeFrequencyType> const& data);

        /**
         * @brief clear all data 
         */
        void clear();

        /**
         * @brief return true if there is no data
         */
        bool empty() const;

        /**
         * @returns the absolute time the first time sample corresponds to.
         */
        TimePointType const& start_time() const;

        /**
         * @returns the integer number of channels in the block
         */
        std::size_t number_of_channels() const;

        /**
         * @returns the total number of samples across all blocks
         */
        std::size_t number_of_spectra() const;

        /**
         * @returns the sample interval in seconds
         */
        TimeType sample_interval() const;

        /**
         * @returns a vector of frequencies that are represented in each time sample
         */
        std::vector<FrequencyType> const& channel_frequencies() const;

        /**
         * @returns an iterator that extends across all the channels in a single sample
         * @param ChannelIterationEndHandler called at the end of each time_sample
         * @param TimeDiscontinuityHandler called any time the sample_interval changes or the start_time() is not
         *        contiguous from previous (will also be called on initialisation_
         *        The function should take two arguments e.g.:
         *        @code
         *        auto my_td_handler = [](TimePointType const&, TimeType const& interval) 
         *        { 
         *              // do something
         *        }
         *        @endcode
         *        The TimePointType passed refers to the start time of the new block.
         *        THe interval is the sample interval after the discontinuity.
         */
        template<class ChannelIterationEndHandler, class TimeDiscontinuityHandler>
        ConstChannelIterator<ChannelIterationEndHandler, TimeDiscontinuityHandler> begin(ChannelIterationEndHandler, TimeDiscontinuityHandler) const;

        /**
         * @brief return the end iterator
         */
        ConstEndChannelIterator end() const;

    private:
        std::vector<std::shared_ptr<TimeFrequencyType>> _data;
        std::size_t _number_of_spectra;
};


} // namespace data
} // namespace cheetah
} // namespace ska
#include "detail/TimeFrequencyContainer.cpp"

#endif // SKA_CHEETAH_DATA_TIMEFREQUENCYCONTAINER_H
