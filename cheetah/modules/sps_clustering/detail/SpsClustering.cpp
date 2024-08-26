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
    if (! _config.active())
        return cands;

    data::DimensionIndex<data::Time> offset_samples(0);
    if(cands->tf_blocks().size() !=0)
    {
        offset_samples = data::DimensionIndex<data::Time>(cands->offset_time().value()/1000.0/(cands->tf_blocks()[0]->sample_interval().value()));
    }

    std::shared_ptr<data::SpCcl<NumRepType>> merged_cands = std::make_shared<data::SpCcl<NumRepType>>(cands->tf_blocks(), offset_samples);
    std::vector<std::vector<size_t>> groups = _clustered_candidates(*cands);

    PANDA_LOG_DEBUG << "Picking best candidate from  " << groups.size() << " clusters...";
    std::vector<data::SpCandidate<Cpu, float>> group_cands;
    for ( auto const& group : groups )
    {
        // use the max sigma candidate as the representative of any group
        // Filter out all candidates with these indices into a separate vector
        PANDA_LOG_DEBUG << "Size of each cluster: " << group.size();
        std::size_t max_sigma_index = group[0];
        typename data::SpCcl<NumRepType>::SpCandidateType::NumericalRep max_sigma = (*cands)[max_sigma_index].sigma();
        for (std::size_t ii=1; ii < group.size(); ++ii)
        {
            auto const& candidate = (*cands)[group[ii]];
            if( candidate.sigma() > max_sigma ) {
                max_sigma = candidate.sigma();
                max_sigma_index = group[ii];
            }
        }
        PANDA_LOG_DEBUG << "Parameters for max S/N candidate";
        PANDA_LOG_DEBUG << "DM: " << (*cands)[max_sigma_index].dm();
        PANDA_LOG_DEBUG << "Width: " << (*cands)[max_sigma_index].width();
        PANDA_LOG_DEBUG << "S/N: " << (*cands)[max_sigma_index].sigma();
        if ((*cands)[max_sigma_index].dm() >= (*cands).dm_range().first + (1.0 * pss::astrotypes::units::parsecs_per_cube_cm))
        {
            merged_cands->push_back((*cands)[max_sigma_index]);
        }
    }
    return merged_cands;
}

} // namespace sps_clustering
} // namespace modules
} // namespace cheetah
} // namespace ska
