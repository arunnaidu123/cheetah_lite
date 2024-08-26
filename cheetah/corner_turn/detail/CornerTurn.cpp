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

#include "cheetah/corner_turn/detail/Algorithms.h"
#include "panda/AlgorithmInfo.h"

namespace ska {
namespace cheetah {
namespace corner_turn {

namespace {

class CornerTurn
{
        typedef panda::MixedAlgoTraits<ActiveAlgos> AlgoTraits;

    public:
        typedef typename AlgoTraits::Architectures Architectures;
        typedef typename AlgoTraits::Capabilities  ArchitectureCapabilities;

    private:
        typedef typename AlgoTraits::AlgoArchMapType AlgoArchMapType;

        template<class Arch1T, class Arch2T>
        struct CornerTurnHelper {
            template <typename SourceIteratorT, typename DestinIteratorT>
            static inline
            void copy_corner_turn(
                  SourceIteratorT&& src_it
                , DestinIteratorT&& dst_it
                , std::size_t num_input_innerloop_elements
                , std::size_t num_input_outerloop_elements)
            {
                static_assert(std::is_same<Arch1T, Arch2T>::value
                    , "corner turn between different Archs not yet supported");
            }
        };

        template<class ArchT>
        struct CornerTurnHelper<ArchT, ArchT> {
            typedef typename boost::mpl::at<AlgoArchMapType, ArchT>::type AlgoType;

            template <typename SourceIteratorT, typename DestinIteratorT>
            static inline
            void copy_corner_turn(
                  SourceIteratorT&& src_it
                , DestinIteratorT&& dst_it
                , std::size_t num_input_innerloop_elements
                , std::size_t num_input_outerloop_elements)
            {
                AlgoType::copy_corner_turn(
                                   std::forward<SourceIteratorT>(src_it)
                                 , std::forward<DestinIteratorT>(dst_it)
                                 , num_input_innerloop_elements
                                 , num_input_outerloop_elements);
            }
        };

    public:
        template <typename SourceIteratorT, typename DestinIteratorT>
        static inline
        void copy_corner_turn(
              SourceIteratorT&& src_it
            , DestinIteratorT&& dst_it
            , std::size_t num_input_innerloop_elements
            , std::size_t num_input_outerloop_elements)
        {
            typedef typename std::remove_reference<SourceIteratorT>::type SrcIt;
            typedef typename std::remove_reference<DestinIteratorT>::type DstIt;

            typedef CornerTurnHelper<
                 typename panda::ArchitectureType<SrcIt>::type
               , typename panda::ArchitectureType<DstIt>::type
                           > Helper;

            Helper::copy_corner_turn(
                        std::forward<SourceIteratorT>(src_it)
                      , std::forward<DestinIteratorT>(dst_it)
                      , num_input_innerloop_elements
                      , num_input_outerloop_elements);
        }

};

} // namespace

template <typename SourceIteratorT, typename DestinIteratorT>
void corner_turn( SourceIteratorT&& src_it
                , DestinIteratorT&& dst_it
                , std::size_t num_input_innerloop_elements
                , std::size_t num_input_outerloop_elements)
{
    CornerTurn::copy_corner_turn(
                            std::forward<SourceIteratorT>(src_it)
                          , std::forward<DestinIteratorT>(dst_it)
                          , num_input_innerloop_elements
                          , num_input_outerloop_elements);
}

} // namespace corner_turn
} // namespace cheetah
} // namespace ska
