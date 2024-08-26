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
#ifndef SKA_CHEETAH_CORNER_TURN_CPU_CORNERTURNARCHHELPER_H
#define SKA_CHEETAH_CORNER_TURN_CPU_CORNERTURNARCHHELPER_H

#include "cheetah/corner_turn/cpu/BatchedCpuCornerTurnAlgo.h"
#include "cheetah/corner_turn/cpu/StandardCpuBatcher.h"

namespace ska {
namespace cheetah {
namespace corner_turn {
namespace cpu {
namespace detail {

struct BatcherTraits {
    typedef cpu::BatchedAlgoDefaultCpuTraits CpuTraits;
    typedef cpu::StandardCpuBatcher<CpuTraits> BatchOptimiser;
};

} // detail

template <typename SourceIteratorT, typename DestinIteratorT>
void CornerTurn::copy_corner_turn_random_access(
                             SourceIteratorT&& src_it
                           , DestinIteratorT&& dst_it
                           , std::size_t num_input_innerloop_elements
                           , std::size_t num_input_outerloop_elements)
{
    cpu::BatchedCpuCornerTurnAlgo<detail::BatcherTraits>::copy_corner_turn(
              &(*src_it)
            , &(*dst_it)
            , num_input_innerloop_elements
            , num_input_outerloop_elements);
}

template<typename SourceIteratorT, typename DestinIteratorT>
inline
void corner_turn(SourceIteratorT&& src_it
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

} // namespace cpu
} // namespace corner_turn
} // namespace cheetah
} // namespace ska
#endif // SKA_CHEETAH_CORNER_TURN_CPU_CORNERTURNARCHHELPER_H
