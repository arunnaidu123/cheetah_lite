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
#include "cheetah/modules/spsift/SpSift.h"
#include <algorithm>
#include <chrono>

namespace ska {
namespace cheetah {
namespace modules {
namespace spsift {

template<typename NumRep>
void SpSift::operator()(data::SpCcl<NumRep>& candidate_list) const
{
    PANDA_LOG << "Total number of candidates: " << candidate_list.size();
    /*
    //auto start = std::chrono::high_resolution_clock::now();
    if(!_config.active()) return;
    /// TODO: The trimming of the candidate list needs to be done *AFTER* the thresholding !
    /// i.e. first remove the candidates that are certainly spurious, and only then limit the
    /// size of the list if necessary.
    /// Currently we can can lose good candidates for no reason.
    if (_config.maximum_candidates() > 0 && candidate_list.size() > _config.maximum_candidates())
    {
	    candidate_list.resize(_config.maximum_candidates());
    }
    candidate_list.remove_if(
                [this](typename data::SpCcl<NumRep>::SpCandidateType const& candidate)
                {
                    return (candidate.dm() < _config.dm_threshold()
                    || candidate.width() > _config.pulse_width_threshold()
                    || candidate.sigma() < _config.sigma_threshold());
                });
    */
}

} // namespace spsift
} // namespace modules
} // namespace cheetah
} // namespace ska
