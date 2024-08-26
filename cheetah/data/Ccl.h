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
#ifndef SKA_CHEETAH_DATA_CCL_H
#define SKA_CHEETAH_DATA_CCL_H

#include "cheetah/data/Candidate.h"
#include "cheetah/data/VectorLike.h"
#include "cheetah/utils/Architectures.h"
#include "panda/DataChunk.h"
#include <vector>

namespace ska {
namespace cheetah {
namespace data {

/**
 * @brief
 *    Candidate list
 * @details
 *
 *
 */

class Ccl
    : public ska::panda::DataChunk<Ccl>
    , public VectorLike<std::vector<Candidate<Cpu, float>>>
{
    public:
        typedef Candidate<Cpu, float> CandidateType;
        typedef VectorLike<std::vector<CandidateType>> BaseT;

    public:
        Ccl();
        ~Ccl();

        /**
         * @brief Add a candidate to the Ccl.
         *
         * @param [in] candidate Candidate to be added to the Ccl
         */
        void add(CandidateType const& candidate);

        /**
         * @brief Move a candidate to the Ccl.
         *
         * @param [in] candidate Candidate to be moved to the Ccl
         */
        void emplace(CandidateType&& candidate);

};


} // namespace data
} // namespace cheetah
} // namespace ska

#endif // SKA_CHEETAH_DATA_CCL_H
