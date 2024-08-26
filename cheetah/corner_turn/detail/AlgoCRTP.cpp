/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016-2022 The SKA organisation
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
#include "panda/DeviceIterator.h"

namespace ska {
namespace cheetah {
namespace corner_turn {

template<typename T1, typename T2, typename TrueType=void>
using EnableIfBothAreRandomAccessIterator =
        typename std::enable_if<panda::IsRandomAccessIterator<T1>::value
                             && panda::IsRandomAccessIterator<T2>::value
                              , TrueType>::type;

// selects sub helpers based on the SourceIteratorT iterator_traits
template<class DerivedT, class ArchT>
template <typename SourceIteratorT, typename DestinIteratorT>
struct AlgoCRTP<DerivedT, ArchT>::CornerTurnHelper<
                        SourceIteratorT
                      , DestinIteratorT
                      , EnableIfBothAreRandomAccessIterator<SourceIteratorT, DestinIteratorT>>
{
    template <class Arch1T, class Arch2T, typename Enable=void>
    struct CornerTurnRandomAccessArchHelper;

    template <class Arch1T>
    struct CornerTurnRandomAccessArchHelper<Arch1T, Arch1T, typename std::enable_if<std::is_same<Arch1T, ArchT>::value>::type>
    {
        template <typename SourceItT, typename DestinItT>
        static inline
        void corner_turn(SourceItT&& src_it
                       , DestinItT&& dst_it
                       , std::size_t num_input_innerloop_elements
                       , std::size_t num_input_outerloop_elements)
        {
            //DerivedT::copy_corner_turn_random_access(
            AlgoCRTP<DerivedT, ArchT>::dispatch(
                                std::forward<SourceItT>(src_it)
                              , std::forward<DestinItT>(dst_it)
                              , num_input_innerloop_elements
                              , num_input_outerloop_elements);
        }
    };

    typedef CornerTurnRandomAccessArchHelper<
                             typename panda::ArchitectureType<SourceIteratorT>::type
                           , typename panda::ArchitectureType<DestinIteratorT>::type
                            > Helper;

    template <typename SourceItT, typename DestinItT>
    inline static void corner_turn(
                             SourceItT&& src_it
                           , DestinItT&& dst_it
                           , std::size_t num_input_innerloop_elements
                           , std::size_t num_input_outerloop_elements)
    {
        Helper::corner_turn( std::forward<SourceItT>(src_it)
                           , std::forward<DestinItT>(dst_it)
                           , num_input_innerloop_elements
                           , num_input_outerloop_elements);
    }
};

template<class DerivedT, class ArchT>
template <typename SourceIteratorT, typename DestinIteratorT>
void AlgoCRTP<DerivedT, ArchT>::copy_corner_turn(
          SourceIteratorT&& src_it
        , DestinIteratorT&& dst_it
        , std::size_t num_input_innerloop_elements
        , std::size_t num_input_outerloop_elements)
{
    static_assert(std::is_same<ArchT, typename DerivedT::Architecture>::value
                , "mismatch in algorithm architecture flags");
    typedef CornerTurnHelper<typename std::remove_reference<SourceIteratorT>::type
                           , typename std::remove_reference<DestinIteratorT>::type
                           > Helper;
    Helper::corner_turn(std::forward<SourceIteratorT>(src_it)
                      , std::forward<DestinIteratorT>(dst_it)
                      , num_input_innerloop_elements
                      , num_input_outerloop_elements);
}

template<class DerivedT, class ArchT>
template <typename SourceIteratorT, typename DestinIteratorT>
void AlgoCRTP<DerivedT, ArchT>::copy_corner_turn_random_access(
          SourceIteratorT&&
        , DestinIteratorT&&
        , std::size_t
        , std::size_t)
{
    static_assert(std::is_same<DerivedT, std::false_type>::value, "please define copy_corner_turn_random_access Method in your DerivedT of corner_turn::AlgoCRTP");
}

template<class DerivedT, class ArchT>
template<typename... Args>
void AlgoCRTP<DerivedT, ArchT>::dispatch(Args&&... args)
{
    DerivedT::copy_corner_turn_random_access(
                             std::forward<Args>(args)...);
}

} // namespace corner_turn
} // namespace cheetah
} // namespace ska
