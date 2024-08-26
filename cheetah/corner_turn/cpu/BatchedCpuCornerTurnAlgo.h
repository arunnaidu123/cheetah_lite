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
#ifndef SKA_CHEETAH_CORNER_TURN_CPU_BATCHEDCPUCORNERTURNALGO_H
#define SKA_CHEETAH_CORNER_TURN_CPU_BATCHEDCPUCORNERTURNALGO_H


namespace ska {
namespace cheetah {
namespace corner_turn {
namespace cpu {

/**
 * @brief Default Traits type for the Cpu specific parts of the BatchedCpuCornerTurnAlgo
 * @tparam InternalBatcherT The inner loop optimistaion to use
 *         This should be a struct with a static method batched_corner_turn
 *         defined
 */
struct BatchedAlgoDefaultCpuTraits
{
    /*
      Here we have used the 32 so the memory access will always be in multiples 256 bit which is the lenght of xmm registers
      which are mostly used in the c++ standard library.
    */
    static const std::size_t nelements_per_cache_lane = 32;
};

/**
 * @brief A fast algorithm designed to allow compilers to optimise the corner_turn
 * @details Cornerturn is essentially rearrangement of data and is a memory bound problem. Here we divide the data into small
 *         batchs of size (nelements_per_cache_lane * nelements_per_cache_lane) and perform cornerturn on these smaller batches.
 *         This method will use the available CPU cache more efficiently and reduce the memory access overhead.
 *
 * @tparam TraitsT
 *         requires the following typedefs to be present:
 *         CpuTraits : the Cpu Traits describing the cpu charteristics
 *                     It should define:
 *                     `static const size_t nelements_per_cache_lane`
 *                     You can use the BatchedAlgoDefaultCpuTraits
 *
 *         BatchOptimiser  : an optimser for the inner loop that perform the corner_turn
 *                           this class must contain which performs cornerturn on the small chunks and
 *                           the method should be as following
 * ~~~~{.cpp}
 *                template <typename SrcT, typename DstT>
 *                static inline
 *                void batched_corner_turn(const SrcT* data_in
 *                           , DstT* data_out
 *                           , std::size_t num_input_innerloop_elements
 *                           , std::size_t num_input_outerloop_elements
 *                           , std::size_t innerloop_element
 *                           , std::size_t outerloop_element);
 * ~~~~
 *
 *               where:
 *                   data_in                     : is the iterator to the begin of the data type.
 *                   data_out                    : is the iterator to the begin of the data type.
 *                   num_input_innerloop_elements: number of elements in the inner dimension
 *                                     (e.g. number of channels in TF datatype and number of spectra for FT datatype)
 *                   num_input_outerloop_elements: number of elements in the outer dimension
 *                                     (e.g. number of spectra in TF datatype and number of channels for FT datatype)
 *                   innerloop_element           : location of first element of the inner dimension of the chunk
 *                   outerloop_element           : location of first element of the outer dimension of the chunk
 *
 *                   The innerloop_element and outerloop_element should be incremented by nelements_per_cache_lane for
 *                   every iteration. The cornerturn results in nelements_per_cache_lane vectors of length
 *                   nelements_per_cache_lane which must be arranged in appropriate places in the data_out.
 *
 *                   This method will process blocks of nelements_per_cache_lane * nelements_per_cache_lane
 *                   whose location is given by last two arguments.
 *                   The resulting nelements_per_cache_lane vectors of size nelements_per_cache_lane
 *                   should be moved to the appropriate location in data_out.
 *
 *
 *                   Example:  for 4x3 block
 *                                                                                                         a4 b4 **
 *                   c1 c2 c3 c4  nelements_per_cache_lane = 2, innerloop_element=2 outerloop_element=0    a3 b3 **
 *                   b1 b2 b3 b4  --------------------------------------------------------------------->   ** ** **
 *                   a1 a2 a3 a4                                                                           ** ** **
 *
 *
 */

template<class TraitsT>
class BatchedCpuCornerTurnAlgo
{
    public:
        /**
         * @brief copy constructor not available
         * @internal if you add a copy constructor please remember "rule of 5" and unit tests
         */
        BatchedCpuCornerTurnAlgo(BatchedCpuCornerTurnAlgo const&) = delete;

        /**
         * @brief executes the copy and turn algorithm
         * @details dst container must be different from the src container
         */
        template <typename SrcT, typename DstT>
        static inline
        void copy_corner_turn(const SrcT* data_in
               , DstT* data_out
               , std::size_t num_input_innerloop_elements
               , std::size_t num_input_outerloop_elements);

};


} // namespace cpu
} // namespace corner_turn
} // namespace cheetah
} // namespace ska
#include "detail/BatchedCpuCornerTurnAlgo.cpp"

#endif // SKA_CHEETAH_CORNER_TURN_CPU_BATCHEDCPUCORNERTURNALGO_H
