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
#ifndef SKA_CHEETAH_DATA_DMTIMEBUFFERSLICE_H
#define SKA_CHEETAH_DATA_DMTIMEBUFFERSLICE_H

#include "cheetah/data/DmTime.h"
#include "cheetah/utils/Architectures.h"

#include "panda/DataChunk.h"

#include <memory>
#include <list>


namespace ska {
namespace cheetah {
namespace data {
namespace detail {

template <typename DmTimeType>
class DmTimeSliceIterator;

/**
 * @brief      Class that wraps a subset of DMs from a DmTime object
 *
 * @details    DmTimeSlice objects are intended to hide the underlying
 *             storage implementation of blocks of DmTime objects. DmTimeSlice
 *             objects are passed to Tdas and Fdas for processing of blocks of DMs at
 *             a time in an async or sync task.
 */

template <typename DmTimeType>
class DmTimeSlice: public panda::DataChunk<DmTimeSlice<DmTimeType>>
{
    private:
        typedef DmTimeSlice<DmTimeType> SelfType;

    public:
        typedef std::shared_ptr<DmTimeType> ParentType;
        typedef typename DmTimeType::ContainerType ContainerType;
        typedef DmTimeSliceIterator<SelfType> Iterator;
        typedef DmTimeSliceIterator<SelfType const> ConstIterator;

    public:
        ~DmTimeSlice();

        /**
         * @brief      Get an iterator poiting to the start of the slice
         *
         * @return     An iterator poiting to the first DM trial in the slice
         */
        Iterator begin();
        ConstIterator cbegin() const;

        /**
         * @brief      Get an iterator poiting to the end of the slice
         *
         * @return     An iterator poiting to the end of the last DM trial in the slice
         */
        Iterator end();
        ConstIterator cend() const;

        /**
         * @brief      Get a constant reference to the underlying list of DmTime shared pointers
         *
         * @return     A reference to the underlying list
         */
        ContainerType const& blocks() const;

    private:
        friend panda::DataChunk<SelfType>;

        /**
         * @brief      Create a new instance
         *
         * @param[in]  start_dm_idx          The start dm index (refers to the start index of the
         *                                   DM list contained in each of the DmTime instances in
         *                                   the blocks member)
         * @param[in]  number_dms_per_slice  The number DM trials per slice
         * @param[in]  parent                A shared_ptr to the parent DmTime object
         */
        DmTimeSlice(std::size_t start_dm_idx, std::size_t number_dms_per_slice,
            ParentType parent);

    private:
        ParentType _parent;
        std::size_t _start_dm_idx;
        std::size_t _number_dms_per_slice;

};

} // namespace detail
} // namespace data
} // namespace cheetah
} // namespace ska

#include "cheetah/data/detail/DmTimeSlice.cpp"

#endif // SKA_CHEETAH_DATA_DMTIMESLICE_H
