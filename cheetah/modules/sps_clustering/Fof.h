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
#ifndef SKA_CHEETAH_MODULES_SPS_CLUSTERING_FOF_H
#define SKA_CHEETAH_MODULES_SPS_CLUSTERING_FOF_H

#include "cheetah/modules/sps_clustering/Config.h"
#include "cheetah/data/SpCcl.h"
#include <boost/geometry/geometry.hpp>
#include <vector>

namespace ska {
namespace cheetah {
namespace modules {
namespace sps_clustering {

/**
 * @brief Friend Of Friends Clustering Algorithm for SpCandidates
 * @details
 */


class Fof
{
    static constexpr std::size_t ClusteringParams = 3;
    typedef boost::geometry::model::point<double, ClusteringParams, boost::geometry::cs::cartesian> PointType;

    public:
    Fof(Config const& config);
    ~Fof();

    void linking_length(double const& l);

    double linking_length() const;

    /**
     * @brief Group the candidates using the fof algorithm
     * @return a vector containing the groups of candidates.
     *         each group is represented as a vector of indices
     *         for the candidatate in the input data.
     */
    template<typename NumRepType>
    std::vector<std::vector<size_t>> operator()(data::SpCcl<NumRepType> const& cands);

    private:
    Config const& _config;
    double _linking_length;
    double _linking_length_2;
};


} // namespace sps_clustering
} // namespace modules
} // namespace cheetah
} // namespace ska
#include "cheetah/modules/sps_clustering/detail/Fof.cpp"
#endif // SKA_CHEETAH_MODULES_SPS_CLUSTERING_FOF_H
