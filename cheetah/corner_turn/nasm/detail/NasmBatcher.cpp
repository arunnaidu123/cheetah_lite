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
#include "cheetah/corner_turn/cpu/StandardCpuBatcher.h"
#include <type_traits>

namespace ska {
namespace cheetah {
namespace corner_turn {
namespace nasm {

extern "C" void nasm_cornerturn_char(const void *data_in
                                 , void *data_out
                                 , std::size_t *num_input_innerloop_elements
                                 , std::size_t *num_input_outerloop_elements
                                 , std::size_t *innerloop_element
                                 , std::size_t *outerloop_element);

extern "C" void nasm_cornerturn_short(const void *data_in
                                 , void *data_out
                                 , std::size_t *num_input_innerloop_elements
                                 , std::size_t *num_input_outerloop_elements
                                 , std::size_t *innerloop_element
                                 , std::size_t *outerloop_element);

extern "C" void nasm_cornerturn_char_short(const void *data_in
                                 , void *data_out
                                 , std::size_t *num_input_innerloop_elements
                                 , std::size_t *num_input_outerloop_elements
                                 , std::size_t *innerloop_element
                                 , std::size_t *outerloop_element);

namespace {

template<std::size_t SrcValueTypeSize, std::size_t DstValueTypeSize, class CpuTraitsT>
struct NasmBatcherHelper {
    template <typename SourceT, typename DstT, typename Enable=void>
    static inline
    void exec( const SourceT* data_in
             , DstT* data_out
             , std::size_t num_input_innerloop_elements
             , std::size_t num_input_outerloop_elements
             , std::size_t innerloop_element
             , std::size_t outerloop_element)
    {
        cpu::StandardCpuBatcher<CpuTraitsT>::batched_corner_turn(
                            data_in
                          , data_out
                          , num_input_innerloop_elements
                          , num_input_outerloop_elements
                          , innerloop_element
                          , outerloop_element
                          );
    }
};

template<class CpuTraitsT>
struct NasmBatcherHelper<1, 1, CpuTraitsT>
{
    template <typename SourceT, typename DstT, typename Enable=void>
    static inline
    void exec( const SourceT data_in
             , DstT data_out
             , std::size_t num_input_innerloop_elements
             , std::size_t num_input_outerloop_elements
             , std::size_t innerloop_element
             , std::size_t outerloop_element)
    {
        nasm_cornerturn_char( static_cast<const void*>(data_in)
                    , static_cast<void*>(data_out)
                    , &num_input_innerloop_elements
                    , &num_input_outerloop_elements
                    , &innerloop_element
                    , &outerloop_element);
    }
};

template<class CpuTraitsT>
struct NasmBatcherHelper<2, 2, CpuTraitsT>
{
    template <typename SourceT, typename DstT, typename Enable=void>
    static inline
    void exec( const SourceT data_in
             , DstT data_out
             , std::size_t num_input_innerloop_elements
             , std::size_t num_input_outerloop_elements
             , std::size_t innerloop_element
             , std::size_t outerloop_element)
    {
        nasm_cornerturn_short( static_cast<const void*>(data_in)
                    , static_cast<void*>(data_out)
                    , &num_input_innerloop_elements
                    , &num_input_outerloop_elements
                    , &innerloop_element
                    , &outerloop_element);
    }
};

template<class CpuTraitsT>
struct NasmBatcherHelper<1, 2, CpuTraitsT>
{
    template <typename SourceT, typename DstT, typename Enable=void>
    static inline
    void exec( const SourceT data_in
             , DstT data_out
             , std::size_t num_input_innerloop_elements
             , std::size_t num_input_outerloop_elements
             , std::size_t innerloop_element
             , std::size_t outerloop_element)
    {
        nasm_cornerturn_char_short( static_cast<const void*>(data_in)
                    , static_cast<void*>(data_out)
                    , &num_input_innerloop_elements
                    , &num_input_outerloop_elements
                    , &innerloop_element
                    , &outerloop_element);
    }
};

} // namespace

template<class CpuTraitsT>
template <typename SrcT, typename DstT>
void NasmBatcher<CpuTraitsT>::batched_corner_turn(
                              const SrcT* data_in
                            , DstT* data_out
                            , std::size_t num_input_innerloop_elements
                            , std::size_t num_input_outerloop_elements
                            , std::size_t innerloop_element
                            , std::size_t outerloop_element)
{
    NasmBatcherHelper<sizeof(SrcT), sizeof(DstT), CpuTraitsT>::exec(
                            data_in
                          , data_out
                          , num_input_innerloop_elements
                          , num_input_outerloop_elements
                          , innerloop_element
                          , outerloop_element);
}

} // namespace nasm
} // namespace corner_turn
} // namespace cheetah
} // namespace ska
