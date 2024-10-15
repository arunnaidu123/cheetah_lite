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
#ifndef SKA_CHEETAH_MODULES_SPS_CLUSTERING_SPSCLUSTERING_H
#define SKA_CHEETAH_MODULES_SPS_CLUSTERING_SPSCLUSTERING_H

#include "cheetah/modules/sps_clustering/Config.h"
#include "cheetah/modules/sps_clustering/Fof.h"
#include "cheetah/data/SpCcl.h"
#include "cheetah/data/SpCandidate.h"

namespace ska {
namespace cheetah {
namespace modules {
namespace sps_clustering {

/**
 * @brief Perform clustering on a single pulse candidate list (SpCcl) object.
 * Given a SpCcl input, candidates that are close in (Time, DM, Width) space
 * are grouped together, and for each group, only the brightest candidate (with highest S/N)
 * is kept as a representative of the group.
 * @details Given points Ai = (ti, di, wi) in (time, DM, width) space, the algorithm considers
 * the associated set of points Ai' with coordinates:
 *
 * Ai' = (ti / time_tolerance, di / dm_tolerance, log2(wi / pulse_width_tolerance)),
 *
 * where the 'tolerance' parameters are user-defined in the Config. Two points Ai and Aj are
 * part of the same cluster if:
 *
 * || Ai' - Aj' || < linking_length,
 *
 * where linking_length is also user-defined and ||  || denotes Euclidean distance.
 *
 * @todo There is probably a better way of automatically choosing dm_tolerance based on the
 * observing bandwidth (i.e. convert the DM difference between two candidates to a dispersion
 * delay). It would be nice to have a sensible, telescope-independent way of doing clustering
 * without this many user-defined parameters.
 */
class SpsClustering
{
        typedef Fof ClusteringAlgo;
        typedef data::SpCandidate<Cpu, float> SpCandidateType;

    public:
        SpsClustering(Config const& config);
        ~SpsClustering();

        /**
         * @brief Given a single pulse candidate list, find clusters and return a new candidate list with only
         * the brightest representative of each cluster.
         */
        template<typename NumRepType>
        std::shared_ptr<data::SpCcl<NumRepType>> operator()(std::shared_ptr<data::SpCcl<NumRepType>> const& cands);

    private:
        Config const& _config;
        ClusteringAlgo _clustered_candidates;

};


} // namespace sps_clustering
} // namespace modules
} // namespace cheetah
} // namespace ska
#include "cheetah/modules/sps_clustering/detail/SpsClustering.cpp"
#endif // SKA_CHEETAH_MODULES_SPS_CLUSTERING_SPSCLUSTERING_H
