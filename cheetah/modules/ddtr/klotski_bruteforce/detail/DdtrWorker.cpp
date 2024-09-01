/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016-2023 The SKA organisation
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
#include "cheetah/modules/ddtr/klotski_bruteforce/detail/DdtrWorker.h"
#include "cheetah/modules/ddtr/klotski_bruteforce/DdtrProcessor.h"
#include "cheetah/corner_turn/CornerTurn.h"
#include "cheetah/data/FrequencyTime.h"
#include "cheetah/data/TimeSeries.h"
#include <iostream>
namespace ska {
namespace cheetah {
namespace modules {
namespace ddtr {
namespace klotski_bruteforce {



template<typename DdtrTraits>
DdtrWorker<DdtrTraits>::DdtrWorker()
{
}

template<typename DdtrTraits>
template<typename BufferType, typename CallBackT>
std::shared_ptr<typename DdtrWorker<DdtrTraits>::DmTrialsType> DdtrWorker<DdtrTraits>::operator()(
                                                                      std::shared_ptr<BufferType> agg_buf
                                                                    , std::shared_ptr<DedispersionPlan<DdtrTraits>> plan
                                                                    , CallBackT const& call_back)
{
    if (agg_buf->capacity() < (std::size_t) plan->dedispersion_strategy()->maxshift())
    {
        panda::Error e("Ddtr klotskBruteforce: data buffer size < maxshift (");
        e << agg_buf->capacity() << "<" << plan->dedispersion_strategy()->maxshift() << ")";
        throw e;
    }


    plan->dm_trials()->start_time(agg_buf->start_time());

    panda::copy(agg_buf->begin(), agg_buf->end(), plan->dedispersion_strategy()->temp_work_area()->begin());
    DdtrProcessor<DdtrTraits> ddtr(plan, plan->dm_trials());

    while(!ddtr.finished())
    {
        ++ddtr;
    }

    DmTrialsType& dmtrials = *(plan->dm_trials());
    call_back(dmtrials, plan->dedispersion_strategy()->ndms());

    return plan->dm_trials();
}

} // namespace klotski_bruteforce
} // namespace ddtr
} // namespace modules
} // namespace cheetah
} // namespace ska
