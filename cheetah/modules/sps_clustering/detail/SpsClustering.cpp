/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 The SKA organisation
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
#include "cheetah/modules/sps_clustering/SpsClustering.h"


namespace ska {
namespace cheetah {
namespace modules {
namespace sps_clustering {

template<typename NumRepType>
std::shared_ptr<data::SpCcl<NumRepType>> SpsClustering::operator()(std::shared_ptr<data::SpCcl<NumRepType>> const& cands)
{
    if (! _config.active() || cands->size()==0)
        return cands;

    std::shared_ptr<data::SpCcl<NumRepType>> merged_cands = std::make_shared<data::SpCcl<NumRepType>>(cands->start_time());

    SpCandidateType temp;
    auto const& data = cands->data();
    auto candidate = data[0];
    for(unsigned int i=0; i<data.size(); ++i)
    {
        if(abs(candidate.tstart().value()-data[i].tstart().value())<10000.0)
        {
            if(candidate.sigma()<data[i].sigma())
            {
                candidate = data[i];
            }
        }
        else
        {
            merged_cands->push_back(candidate);
            if(i+1<data.size()) candidate = data[i];
        }
    }
    merged_cands->push_back(candidate);
    return merged_cands;
}

} // namespace sps_clustering
} // namespace modules
} // namespace cheetah
} // namespace ska
