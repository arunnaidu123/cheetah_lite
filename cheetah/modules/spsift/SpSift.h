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
#ifndef SKA_CHEETAH_MODULES_SPSIFT_SPSIFT_H
#define SKA_CHEETAH_MODULES_SPSIFT_SPSIFT_H

#include "cheetah/modules/spsift/Config.h"
#include "cheetah/data/SpCcl.h"

namespace ska {
namespace cheetah {
namespace modules {
namespace spsift {

/**
 * @brief
 * A module that removes single pulse candidates from a SpCcl object,
 * based on 3 user-defined thresholds in DM, pulse width, and signal-to-noise ratio (S/N).
 * @details A candidate is removed if either of the following conditions are true:
 * 1. DM is below threshold
 * 2. S/N is below threshold
 * 3. Width is *above* threshold (wide pulses are more likely to be RFI)
 * There is an additional configuration parameter called 'maximum_candidates';
 * if non-zero, the input SpCcl is trimmed down to that number of candidates.
 * @todo The maximum_candidates condition is currently enforced before the thresholding,
 * which is a mistake, it should be enforced *after*.
 */

class SpSift
{
    public:
        SpSift(Config const& config);
        ~SpSift();

        /**
         * @brief Return the Configuration object
         */
        Config const& config() const;

        /**
         * @brief Remove all candidates on the wrong side of any threshold.
         * @note This method modifies its argument.
         */
        template<typename NumRep>
        void operator()(data::SpCcl<NumRep>& candidate_list) const;

    private:
        Config const& _config;
};

} // namespace spsift
} // namespace modules
} // namespace cheetah
} // namespace ska
#include "cheetah/modules/spsift/detail/SpSift.cpp"
#endif // SKA_CHEETAH_MODULES_SPSIFT_SPSIFT_H
