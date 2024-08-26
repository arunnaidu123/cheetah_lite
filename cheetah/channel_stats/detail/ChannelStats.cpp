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
#include "cheetah/channel_stats/detail/Algorithms.h"
#include <panda/AlgorithmInfo.h>


namespace ska {
namespace cheetah {
namespace channel_stats {


template<typename BeginIteratorT, typename EndIteratorT>
void ChannelStats::stats(BeginIteratorT&&, EndIteratorT&&)
{
    typedef typename panda::ArchitectureType<typename std::remove_reference<BeginIteratorT>::type>::type Arch;

    static_assert(std::is_same<Arch
                             , typename panda::ArchitectureType<typename std::remove_reference<BeginIteratorT>::type>::type>::value,
        "begin and end iterators are on  different archtiectures");

    typedef typename boost::mpl::at<AlgoArchMapType, ArchT>::type AlgoType;

    AlgoType::stats(std::forward<BeginIteratorT>(begin)
                   ,std::forward<EndIteratorT>(end));
}

} // namespace channel_stats
} // namespace cheetah
} // namespace ska
