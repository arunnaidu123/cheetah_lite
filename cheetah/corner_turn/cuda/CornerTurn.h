/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2022 The SKA organisation
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
#include "cheetah/corner_turn/detail/AlgoCRTP.h"
#include "cheetah/utils/Architectures.h"
#include <cstddef>

#ifndef SKA_CHEETAH_CORNERTURN_CUDA_CORNERTURN_H
#define SKA_CHEETAH_CORNERTURN_CUDA_CORNERTURN_H

namespace ska {
namespace cheetah {
namespace corner_turn {
namespace cuda {

/**
 * @brief corner turn class
 * @details Provides the implementation details for the corner_turn function
 *          and provides a suitable interface for use as an async panda task if desired.
 */
class CornerTurn : public corner_turn::AlgoCRTP<CornerTurn, Cuda>
{
        typedef corner_turn::AlgoCRTP<CornerTurn, Cuda> BaseT;
        friend BaseT;

    public:
        typedef Cuda Architecture;

    private:
        template<typename SourceIteratorT, typename DestinIteratorT>
        static inline
        void copy_corner_turn_random_access(
              SourceIteratorT&& src_it
            , DestinIteratorT&& dst_it
            , std::size_t num_input_innerloop_elements
            , std::size_t num_input_outerloop_elements);
};

/**
 * @brief copy and cornerturn method on an nvidia GPU
 */
template <typename SourceIteratorT, typename DestinIteratorT>
void corner_turn( SourceIteratorT&& src_it
                , DestinIteratorT&& dst_it
                , std::size_t num_input_innerloop_elements
                , std::size_t num_input_outerloop_elements);

} // namespace cuda
} // namespace corner_turn
} // namespace cheetah
} // namespace ska

#include "detail/CornerTurn.cpp"

#endif // SKA_CHEETAH_CORNERTURN_CUDA_CORNERTURN_H
