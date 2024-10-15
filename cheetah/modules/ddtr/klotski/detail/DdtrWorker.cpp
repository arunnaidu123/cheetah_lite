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
#include "cheetah/modules/ddtr/klotski/detail/DdtrWorker.h"
#include "cheetah/modules/ddtr/klotski/DdtrProcessor.h"
#include "cheetah/corner_turn/CornerTurn.h"
#include "cheetah/data/FrequencyTime.h"
#include "cheetah/data/TimeSeries.h"

namespace ska {
namespace cheetah {
namespace modules {
namespace ddtr {
namespace klotski {



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
    std::lock_guard<std::mutex> lock(plan->dm_trials()->mutex());

    if (agg_buf->capacity() < (std::size_t) plan->dedispersion_strategy()->maxshift())
    {
        panda::Error e("DdtrKlotski: data buffer size < maxshift (");
        e << agg_buf->capacity() << "<" << plan->dedispersion_strategy()->maxshift() << ")";
        throw e;
    }
    auto ddtr_start = std::chrono::high_resolution_clock::now();
    panda::copy(agg_buf->begin(), agg_buf->end(), plan->dedispersion_strategy()->temp_work_area()->begin());

    auto dmtrials_ptr = plan->dm_trials();
    auto spdt_dmtrials_ptr = plan->spdt_dm_trials();

    dmtrials_ptr->start_time(agg_buf->start_time());
    spdt_dmtrials_ptr->start_time(agg_buf->start_time());

    DdtrProcessor<DdtrTraits> ddtr(plan, dmtrials_ptr);

    while(!ddtr.finished())
    {
        ++ddtr;
    }
    auto ddtr_stop = std::chrono::high_resolution_clock::now();

    DmTrialsType& dmtrials = *(dmtrials_ptr);
    call_back(dmtrials, plan->dedispersion_strategy()->ndms());


    spdt_dmtrials_ptr->wait();
    dmtrials_ptr->swap(*spdt_dmtrials_ptr);

    PANDA_LOG<<" Beam id: "<<plan->beam_id()<<" Ddtr time: "<<std::chrono::duration_cast<std::chrono::nanoseconds>(ddtr_stop - ddtr_start).count()/1000000.0<<" ms";

    return spdt_dmtrials_ptr;
}

} // namespace klotski
} // namespace ddtr
} // namespace modules
} // namespace cheetah
} // namespace ska
