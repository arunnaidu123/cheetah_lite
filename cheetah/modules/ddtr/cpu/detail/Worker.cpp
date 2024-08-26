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
#include "cheetah/modules/ddtr/cpu/DdtrProcessor.h"
#include "cheetah/data/TimeFrequency.h"
#include "panda/Log.h"


namespace ska {
namespace cheetah {
namespace modules {
namespace ddtr {
namespace cpu {


template<typename DdtrTraits>
Worker<DdtrTraits>::Worker()
{
}

template<typename DdtrTraits>
template<typename CallBackT>
std::shared_ptr<typename Worker<DdtrTraits>::DmTrialsType> Worker<DdtrTraits>::operator()( BufferType const& buffer
                                                                                         , std::shared_ptr<DedispersionPlan<DdtrTraits>> plan
                                                                                         , CallBackT const& call_back)
{
    auto const& data = buffer.buffer();
    FrequencyTimeType data_copy(data);
    std::size_t nchans = data.number_of_channels();
    plan->reset(data);
    auto dm_trial_metadata = plan->dm_trials_metadata(data.metadata(), data.template dimension<data::Time>());
    std::shared_ptr<DmTrialsType> dmtrials_ptr = DmTrialsType::make_shared(dm_trial_metadata, data.start_time());
    if (data.data_size() < plan->buffer_overlap() * nchans)
    {
        PANDA_LOG_WARN << "AggregationBuffer is too small to be processed ("
                       << data.data_size() << " < " << plan->buffer_overlap()*nchans << ")\n"<<"Skipping Current Buffer";
        return dmtrials_ptr;
    }
    auto ddtr = DdtrProcessor<DdtrTraits>(plan, data_copy, dmtrials_ptr);

    while(!ddtr.finished())
    {
        ++ddtr;
    }

    call_back(dmtrials_ptr);
    return dmtrials_ptr;
}

} // namespace cpu
} // namespace ddtr
} // namespace modules
} // namespace cheetah
} // namespace ska
