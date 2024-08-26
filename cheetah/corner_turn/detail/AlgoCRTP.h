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
#ifndef SKA_CHEETAH_CORNER_TURN_ALGOCRTP_H
#define SKA_CHEETAH_CORNER_TURN_ALGOCRTP_H


namespace ska {
namespace cheetah {
namespace corner_turn {

/**
 * @brief CRTP base class for all CornerTurn algos
 * @tparam DerivedT The algorithms Api class
 * @tparam ArchT the architecture tag on which the algo should run
 */

template<class DerivedT, class ArchT>
class AlgoCRTP
{
    public:
        typedef ArchT Architecture;

    public:
        /**
         * @brief constructor
         */
        AlgoCRTP() = default;

        /**
         * @brief copy constructor not available
         * @internal if you add a copy constructor please remember "rule of 5" and unit tests
         */
        AlgoCRTP(AlgoCRTP const&) = delete;

        /**
         * @brief     A generic cornerturn method for different Architectures
         *
         * @details   This function copies data from one container to another transposing
         *            the data as it does so (i.e changing the index ordering from XY to YX).
         *
         * @tparam    SourceIteratorT type of the source iterator.
         * @tparam    DestinIteratorT type of the destination iterator
         * @param     src_it      begin iterator to the data block to be conerturned (input)
         * @param     dst_it      begin iterator to destination data block (cannot be in the same container as the src)
         * @param     num_input_innerloop_elements  size of inner_loop_elements of input
         * @param     num_input_outerloop_elements  size of outer_loop_elements of input
         */
        template <typename SourceIteratorT, typename DestinIteratorT>
        static inline
        void copy_corner_turn(
                         SourceIteratorT&& src_it
                       , DestinIteratorT&& dst_it
                       , std::size_t num_input_innerloop_elements
                       , std::size_t num_input_outerloop_elements);

    protected:
        /**
         * @brief DerivedT must provide this function
         * @details the input iterators are guaranteed to be on
         *          the ArchT tag and have been verified as random access iterators
         */
        template <typename SourceIteratorT, typename DestinIteratorT>
        static inline
        void copy_corner_turn_random_access(
                         SourceIteratorT&& src_it
                       , DestinIteratorT&& dst_it
                       , std::size_t num_input_innerloop_elements
                       , std::size_t num_input_outerloop_elements);

    private:
        template <typename SourceIteratorT, typename DestinIteratorT, typename Enable=void>
        struct CornerTurnHelper;

        template<typename... Args>
        inline static
        void dispatch(Args&&...);
};

} // namespace corner_turn
} // namespace cheetah
} // namespace ska
#include "cheetah/corner_turn/detail/AlgoCRTP.cpp"

#endif // SKA_CHEETAH_CORNER_TURN_ALGOCRTP_H
