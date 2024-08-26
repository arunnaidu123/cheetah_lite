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
#ifndef SKA_CHEETAH_DATA_OCLD_H
#define SKA_CHEETAH_DATA_OCLD_H

#include "cheetah/data/OptimisedCandidate.h"
#include "cheetah/data/VectorLike.h"

namespace ska {
namespace cheetah {
namespace data {

/**
 * @class Ocld
 *
 * @brief Optimised Candidate List and Data
 *
 * @details Holds an vector of optimised candidates, i.e. folded data cubes output
 *          by the FLDO module with dimensions of (Time x Frequency x Phase), as
 *          well as a list of the parameters used by FLDO to generate the data cubes.
 *
 */

template<typename NumericalT>
class Ocld : public VectorLike<std::vector<OptimisedCandidate<NumericalT>>>
{
    public:
        typedef VectorLike<std::vector<OptimisedCandidate<NumericalT>>> BaseT;
        typedef typename BaseT::Iterator OptimisedCandidateIterator;
        typedef typename BaseT::ConstIterator ConstOptimisedCandidateIterator;

    public:
        Ocld();
        ~Ocld();

        /**
         * @brief Add a candidate to the Ocld.
         *
         * @details Add a candidate (data cube plus metadata) to the
         *          Ocld. Currently, candidates will appear in the
         *          Ocld in the order in which they are added.
         *
         * @param [in] data   Candidate to be added to the Ocld
         */
        void add(OptimisedCandidate<NumericalT> const& data);
};

} // namespace data
} // namespace cheetah
} // namespace ska

#include "detail/Ocld.cpp"

#endif // SKA_CHEETAH_DATA_OCLD_H
