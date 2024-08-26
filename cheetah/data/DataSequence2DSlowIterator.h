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
#ifndef SKA_CHEETAH_DATA_DATASEQUENCE2DSLOWITERATOR_H
#define SKA_CHEETAH_DATA_DATASEQUENCE2DSLOWITERATOR_H
#include <boost/iterator/iterator_facade.hpp>
#include <type_traits>


namespace ska {
namespace cheetah {
namespace data {

/**
 * @brief Iterator over DataSequence2D types, over "fast" axis. Generic type.
 * 
 */
template <typename Arch, typename Type>
class DataSequence2DSlowIterator : public DataSequence2DIterator<Arch,Type>{
};

template<typename Arch, typename T>
class DataSequence2D;

// Cpu Specialization
template <typename Type>
class DataSequence2DSlowIterator<Cpu, Type> : public boost::iterator_facade<
      DataSequence2DSlowIterator<Cpu, Type>
    , Type
    , std::random_access_iterator_tag
    >
{
    private:
        typedef DataSequence2DSlowIterator<Cpu,Type> SelfType;
        typedef boost::iterator_facade<SelfType, Type, std::random_access_iterator_tag> BaseT;
        typedef typename BaseT::pointer PtrType;
        typedef typename BaseT::reference RefType;
        typedef typename BaseT::difference_type DiffType;

    public:
        explicit DataSequence2DSlowIterator(const DataSequence2D<Cpu,typename std::remove_cv<Type>::type> &data,
                std::size_t fast_offset=0);

        explicit DataSequence2DSlowIterator(DataSequence2D<Cpu,typename std::remove_cv<Type>::type> &data,
                std::size_t fast_offset=0);
        ~DataSequence2DSlowIterator();

    private:
        friend class boost::iterator_core_access;

        bool equal(SelfType const& other) const;
        void increment();
        void decrement();
        void advance(std::size_t n);
        RefType dereference() const;
        DiffType distance_to(SelfType const& right) const;

    private:
        const DataSequence2D<Cpu,typename std::remove_cv<Type>::type>& _data;
        std::size_t _pos;
        std::size_t _offset;
};

} // namespace data
} // namespace cheetah
} // namespace ska
#include "cheetah/data/detail/DataSequence2DSlowIterator.cpp"

#endif // SKA_CHEETAH_DATA_DATASEQUENCE2DSLOWITERATOR_H 
