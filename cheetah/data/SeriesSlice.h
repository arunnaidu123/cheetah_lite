#ifndef SKA_CHEETAH_DATA_SERIESSLICE_H
#define SKA_CHEETAH_DATA_SERIESSLICE_H
#include <iterator>


namespace ska {
namespace cheetah {
namespace data {

/**
 * @brief A view into a series-like container
 *
 * @details
 *
 */
template <typename SeriesType>
class SeriesSlice
{
    public:
        typedef typename SeriesType::Iterator Iterator;
        typedef typename SeriesType::ConstIterator ConstIterator;
        typedef typename std::iterator_traits<Iterator>::pointer Pointer;
        typedef typename std::iterator_traits<ConstIterator>::pointer ConstPointer;
        typedef typename std::iterator_traits<Iterator>::reference Reference;
        typedef typename std::iterator_traits<ConstIterator>::reference ConstReference;
        typedef typename std::iterator_traits<Iterator>::value_type ValueType;

    public:
        /**
         * @brief      Construct a new instance
         *
         * @param[in]  start_it  The start iterator
         * @param[in]  end_it    The end iterator
         */
        SeriesSlice(Iterator start_it, Iterator end_it);
        virtual ~SeriesSlice();

        /**
         * @brief      Return an iterator pointing to the start of the slice
         */
        Iterator begin();
        ConstIterator begin() const;
        ConstIterator cbegin() const;

        /**
         * @brief      Return an iterator pointing to the end of the slice
         */
        Iterator end();
        ConstIterator end() const;
        ConstIterator cend() const;

        /**
         * @brief      Return a reference to the nth element of the slice
         *
         * @param[in]  n     index of the element
         */
        Reference operator[](std::size_t n);
        ConstReference operator[](std::size_t n) const;

        /**
         * @brief      Return the number of elements in the slice
         */
        std::size_t size() const;

        /**
         * @brief      Return a pointer to the first element of the slice
         */
        Pointer data();
        ConstPointer data() const;

    protected:
        Iterator _start_it;
        Iterator _end_it;
};


} // namespace data
} // namespace cheetah
} // namespace ska

#include "cheetah/data/detail/SeriesSlice.cpp"

#endif // SKA_CHEETAH_DATA_SERIESSLICE_H
