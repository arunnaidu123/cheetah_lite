#ifndef SKA_CHEETAH_DATA_DMTIMEITERATOR_H
#define SKA_CHEETAH_DATA_DMTIMEITERATOR_H

#include "cheetah/data/DmTime.h"
#include "cheetah/data/detail/DmTimeSlice.h"
#include <cstddef>
#include <memory>

namespace ska {
namespace cheetah {
namespace data {

template <typename DmTrialsType>
class DmTime;

namespace detail {

/**
 * @brief     Provides an iterator over a DmTime in slices of DMs
 *
 * @details   The DmTimeIterator is used to slice a DmTime into
 *            multiple SliceType instances, each of which provides
 *            access to a subset of DM trials stored in the DmTime.
 *
 */

template <typename DmTimeType>
class DmTimeIterator
{
    public:
        typedef typename DmTimeType::SliceType SliceType;

    public:
        /**
         * @brief      Create a new instances
         *
         * @param[in]  current_dm_idx        The current dm index
         * @param[in]  number_dms_per_slice  The number dms per slice
         * @param[in]  parent                The parent DmTime object
         */
        DmTimeIterator(std::size_t current_dm_idx,
            std::size_t number_dms_per_slice,
            std::shared_ptr<DmTimeType> parent);
        ~DmTimeIterator();

        /**
         * @brief      Increment the iterator
         *
         * @return     A reference to an iterator pointing to the next
         *             SliceType object.
         */
        DmTimeIterator& operator++();

        /**
         * @brief      Dereference the iterator
         *
         * @return     Returns a SliceType object containing
         *             a subset of DM trials.
         */
        std::shared_ptr<SliceType> operator*() const;

        /**
         * @brief      Test if another DmTimeIterator is different to this one
         *
         * @param      other  Another DmTimeIterator instance
         *
         * @return     true or false
         */
        bool operator!=(DmTimeIterator const& other) const;

        /**
         * @brief      Test if another DmTimeIterator is the same as this one
         *
         * @param      other  Another DmTimeIterator instance
         *
         * @return     true or false
         */
        bool operator==(DmTimeIterator const& other) const;

        /**
         * @brief      Test if another DmTimeIterator is the before this one
         *
         * @param      other  The other
         *
         * @return     true or false
         */
        bool operator<(DmTimeIterator const& other) const;

        /**
         * @brief      Test if another DmTimeIterator is the after this one
         *
         * @param      other  The other
         *
         * @return     true or false
         */
        bool operator>(DmTimeIterator const& other) const;

    private:
        std::size_t _current_dm_idx;
        std::size_t _number_dms_per_slice;
        std::shared_ptr<DmTimeType> _parent;
};

} // namespace detail
} // namespace data
} // namespace cheetah
} // namespace ska

#include "cheetah/data/detail/DmTimeIterator.cpp"

#endif // SKA_CHEETAH_DATA_DMTIMEITERATOR_H
