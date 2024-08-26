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
#ifndef SKA_CHEETAH_DATA_DMTIME_H
#define SKA_CHEETAH_DATA_DMTIME_H

#include "cheetah/data/detail/DmTimeSlice.h"
#include "cheetah/utils/Architectures.h"
#include "panda/DataChunk.h"

#include <memory>
#include <list>

namespace ska {
namespace cheetah {
namespace data {
namespace detail {
    template <typename DmTimeType>
    class DmTimeIterator;
} // namespace detail

/**
 * @brief      A wrapper class for a list of DmTime instances
 *
 * @details    This class provides methods for organising access to
 *             a container (currently a list) of DmTime instances.
 *             The DmTime list remains sorted at all times.
 *             The underlying data structure can be iterated over
 *             in blocks of DMs. This allows the DMs to be split up
 *             for dispatch to processing algorithms that require the
 *             full observation length for processing (e.g. Fdas and Tdas)
 *
 *             Note that the DmTime class does not guarantee that all DmTrials
 *             instances it contains are compatible (i.e. contain the same
 *             of DMs and are contiguous in time). The Psbc class which wraps
 *             a DmTime instance is intended to handle this functionality and
 *             act as an aggregation buffer for passing DmTrials between processing
 *             algorithms.
 *
 *             A typical use case for the DmTime class is as follows:
 *
 * @code       //Create a new instance
 *             auto dm_time = DmTime::make_shared();
 *
 *             //Add some sets of DmTrials to the instance. The trials will be inserted
 *             //into the underlying list in time order.
 *             dm_time->add(<DmTrials instance 0>);
 *             dm_time->add(<DmTrials instance 1>);
 *             dm_time->add(<DmTrials instance 2>);
 *
 *             //To access the underlying data in a useful manner, we employ iterators.
 *             //The begin iterator of the DmTime object will return a slice iterator
 *             //that iterates over all the underlying DMs in groups defined by the user.
 *
 *             for (auto it = dm_time->begin(<number of DMs per slice>);
 *                 it != dm_time->end();
 *                 ++it)
 *             {
 *                 //If we wish to iterate over the slice we do the following
 *                 for (auto dm_it = *(it)->begin();
 *                     dm_it != *(it)->end();
 *                     ++dm_it)
 *                 {
 *                     ...do somthing with each DmTrial...
 *                 }
 *             }
 * @endcode
 */

template <typename DmTrialsType>
class DmTime
    : public panda::DataChunk<DmTime<DmTrialsType>>
{
    private:
        typedef DmTime<DmTrialsType> SelfType;

    public:
        typedef std::list<std::shared_ptr<DmTrialsType>> ContainerType;
        typedef std::shared_ptr<DmTrialsType> ValueType;
        typedef detail::DmTimeSlice<SelfType> SliceType;
        typedef detail::DmTimeIterator<SelfType> Iterator;
        typedef detail::DmTimeIterator<SelfType const> ConstIterator;
        typedef typename DmTrialsType::TimeType Seconds;

    public:
        ~DmTime();
        /**
         * @brief      Insert a new DmTime object into the buffer
         *
         * @details    The shared_ptr is inserted into the underlying list
         *             in order based upon the start_time() method of the
         *             DmTime object.
         *
         * @param[in]  data  The data to be added
         */
        void add(ValueType data);

        /**
         * @brief      Clear the DmTime
         */
        void clear();

        /**
         * @brief      Get an iterator pointing to the first DM trial that iterates over slices of DMs
         *
         * @param[in]  number_dms_per_slice  The number DM trials per slice
         *
         * @return     An iterator that dereferences to a DmTimeSlice object
         */
        Iterator begin(std::size_t number_dms_per_slice);
        ConstIterator cbegin(std::size_t number_dms_per_slice) const;

        /**
         * @brief      Get an iterator that points to the last DM trial in the buffer
         *
         * @return     An iterator that dereferences to a DmTimeSlice object
         */
        Iterator end();
        ConstIterator cend() const;

        /**
         * @brief      Get a reference to the underlying list
         *
         * @return     A constant reference to the underlying list object
         */
        ContainerType const& blocks() const;
        ContainerType& blocks();

        /**
         * @brief  called when the the DmTime object is ready to be processed
         */
        void ready();

        /**
         * @brief  wait function to wait till the DmTime object is ready to be processed
         */
        void wait();

        /**
         * @brief  return the dump_time in seconds
         */
        Seconds dump_time() const;

        /**
         * @brief  set the dump time in seconds
         */
        void dump_time(Seconds dump_time);

    private:
        /**
         * Note: private constructors as should be constructed via ::make_shared()
         */
        friend panda::DataChunk<SelfType>;
        DmTime();

    private:
        ContainerType _data;
        std::mutex _m;
        std::condition_variable _cv;
        bool _ready;
        Seconds _dump_time;
};

} // namespace data
} // namespace cheetah
} // namespace ska

#include "cheetah/data/detail/DmTimeIterator.h"
#include "cheetah/data/detail/DmTime.cpp"

#endif // SKA_CHEETAH_DATA_DMTIME_H
