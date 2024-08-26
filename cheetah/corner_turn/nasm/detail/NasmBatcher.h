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
#ifndef SKA_CHEETAH_CORNER_TURN_NASM_NASMBATCHER_H
#define SKA_CHEETAH_CORNER_TURN_NASM_NASMBATCHER_H


namespace ska {
namespace cheetah {
namespace corner_turn {
namespace nasm {

/**
 * @brief Internal Batcher for activating AVX assembly in cpu::BatchedCpuCornerTurnAlgo
 */
template<class CpuTraitsT>
class NasmBatcher
{
    public:
        template <typename SrcT, typename DstT>
        static inline
        void batched_corner_turn(const SrcT* data_in
                            , DstT* data_out
                            , std::size_t num_input_innerloop_elements
                            , std::size_t num_input_outerloop_elements
                            , std::size_t innerloop_element
                            , std::size_t outerloop_element);
};

} // namespace nasm
} // namespace corner_turn
} // namespace cheetah
} // namespace ska
#include "NasmBatcher.cpp"

#endif // SKA_CHEETAH_CORNER_TURN_NASM_NASMBATCHER_H
