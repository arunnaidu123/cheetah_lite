#ifndef SKA_CHEETAH_DATA_DMTIMESLICEITERATOR_H
#define SKA_CHEETAH_DATA_DMTIMESLICEITERATOR_H

#include "cheetah/data/detail/DmTimeDm.h"

#include <cstddef>
#include <memory>

namespace ska {
namespace cheetah {
namespace data {
namespace detail {

/**
 * @brief     An iterator over a DmTimeSlice object
 *
 * @details   Provides methods to acqurire DmTimeDm
 *            objects pointing to individual DM trials in
 *            the parent DmTimeSlice object.
 */

template <typename DmTimeSliceType>
class DmTimeSliceIterator
{
    public:
        typedef DmTimeDm<DmTimeSliceType> DmTimeDmType;

    public:

        /**
         * @brief      Create a new instances
         *
         * @param[in]  current_dm_idx  The current dm index in the slice
         * @param[in]  parent          The parent DmTimeSlice object
         */
        DmTimeSliceIterator(std::size_t current_dm_idx,
            std::shared_ptr<const DmTimeSliceType> parent);
        ~DmTimeSliceIterator();

        /**
         * @brief      Dereference the iterator
         *
         * @return     A DmTimeDm object
         */
        DmTimeDmType operator*() const;

        /**
         * @brief      Increment the iterator
         *
         * @return     A reference to a DmTimeSliceIterator pointing
         *             to the next DM trial in the slice.
         */
        DmTimeSliceIterator& operator++();

        /**
         * @brief      Test if another DmTimeSliceIterator is different to this one.
         *
         * @param      other  The other DmTimeSliceIterator object
         *
         * @return     true or false
         */
        bool operator!=(DmTimeSliceIterator const& other);

        /**
         * @brief      Test if another DmTimeSliceIterator is the same as this one.
         *
         * @param      other  The other DmTimeSliceIterator object
         *
         * @return     true or false
         */
        bool operator==(DmTimeSliceIterator const& other);

    private:
        std::size_t _current_dm_idx;
        std::shared_ptr<const DmTimeSliceType> _parent;
};

} // namespace detail
} // namespace data
} // namespace cheetah
} // namespace ska

#include "cheetah/data/detail/DmTimeSliceIterator.cpp"

#endif // SKA_CHEETAH_DATA_DMTIMESLICEITERATOR_H
