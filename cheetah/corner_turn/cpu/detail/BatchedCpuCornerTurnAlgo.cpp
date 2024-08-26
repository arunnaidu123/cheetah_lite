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

namespace ska {
namespace cheetah {
namespace corner_turn {
namespace cpu {

template<class TraitsT>
template <typename SrcT, typename DstT>
void BatchedCpuCornerTurnAlgo<TraitsT>::copy_corner_turn(
                 const SrcT* data_in
               , DstT* data_out
               , std::size_t num_input_innerloop_elements
               , std::size_t num_input_outerloop_elements)
{
    // the value_type of the iterator
    typedef typename TraitsT::CpuTraits CpuTraits;

    if( num_input_innerloop_elements < CpuTraits::nelements_per_cache_lane
     || num_input_outerloop_elements < CpuTraits::nelements_per_cache_lane)
    {
        for(std::size_t i=0; i<num_input_outerloop_elements; ++i)
        {
            for(std::size_t j=0; j<num_input_innerloop_elements; ++j)
            {
                data_out[num_input_outerloop_elements*j+i] = (DstT)data_in[num_input_innerloop_elements*i+j];
            }
        }
    }
    else
    {
        for(std::size_t outerloop_element=0;
            outerloop_element < num_input_outerloop_elements-num_input_outerloop_elements%CpuTraits::nelements_per_cache_lane;
            outerloop_element+=CpuTraits::nelements_per_cache_lane)
        {
            for(std::size_t innerloop_element=0;
               innerloop_element < num_input_innerloop_elements-num_input_innerloop_elements%CpuTraits::nelements_per_cache_lane;
               innerloop_element+=CpuTraits::nelements_per_cache_lane)
            {
                TraitsT::BatchOptimiser::batched_corner_turn(
                                 data_in
                               , data_out
                               , num_input_innerloop_elements
                               , num_input_outerloop_elements
                               , innerloop_element
                               , outerloop_element);
            }
        }

        for(std::size_t outerloop_element=0;
            outerloop_element<num_input_outerloop_elements;
            ++outerloop_element)
        {
            for(std::size_t innerloop_element=num_input_innerloop_elements-num_input_innerloop_elements%CpuTraits::nelements_per_cache_lane;
               innerloop_element<num_input_innerloop_elements;
               ++innerloop_element)
            {
                data_out[num_input_outerloop_elements*innerloop_element+outerloop_element] = (DstT)data_in[num_input_innerloop_elements*outerloop_element+innerloop_element];
            }
        }

        for(std::size_t innerloop_element=0;
           innerloop_element<num_input_innerloop_elements;
           innerloop_element++)
        {
            for(std::size_t outerloop_element=num_input_outerloop_elements-num_input_outerloop_elements%CpuTraits::nelements_per_cache_lane;
                outerloop_element<num_input_outerloop_elements;
                outerloop_element++)
            {
                data_out[num_input_outerloop_elements*innerloop_element+outerloop_element] =
                    (DstT)data_in[num_input_innerloop_elements*outerloop_element+innerloop_element];
            }
        }
    }
}

} // namespace cpu
} // namespace corner_turn
} // namespace cheetah
} // namespace ska
