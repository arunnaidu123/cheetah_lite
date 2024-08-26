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

#include "panda/PoolLimits.h"
#include "panda/TupleUtilities.h"


namespace ska {
namespace cheetah {
namespace modules {
namespace ddtr {

template<typename TraitsT>
data::DimensionSize<data::Time> CommonDedispersionPlanBase<TraitsT>::buffer_overlap() const
{
    return data::DimensionSize<data::Time>(0);
}

template<typename TraitsT>
data::DimensionSize<data::Time> CommonDedispersionPlanBase<TraitsT>::dedispersion_strategy(TimeFrequencyType const&)
{
    return data::DimensionSize<data::Time>(0);
}

template<typename TraitsT, typename... AlgosT>
CommonDedispersionPlan<TraitsT, AlgosT...>::CommonDedispersionPlan(ConfigType const& config)
    : _plans(typename PlanType<AlgosT>::type(config
                                           , panda::PoolLimits::template minimum_memory<typename AlgosT::Architecture>(config.pool())/sizeof(typename TraitsT::value_type))...)
{
}

template<typename TraitsT, typename... AlgosT>
CommonDedispersionPlan<TraitsT, AlgosT...>::~CommonDedispersionPlan()
{
}

namespace {

class BufferOverlapHelper {
    public:
        BufferOverlapHelper()
            : _max(0)
        {}

        template<typename Plan>
        void operator()(Plan const& plan) {
            auto bs = plan.buffer_overlap();
            if(bs > _max) {
                _max = bs;
            }
        }

        template<typename Plan>
        void operator()(Plan* plan) {
            this->operator()(*plan);
        }

        data::DimensionSize<data::Time> const& value() const
        {
            return _max;
        }

    private:
        data::DimensionSize<data::Time> _max;
};

class DispersionStrategyHelper {
    public:
        DispersionStrategyHelper()
            : _min(std::numeric_limits<std::size_t>::max())
        {}

        template<typename Plan, typename... Args>
        void operator()(Plan& plan, Args&&... args) {
            auto bs = plan.reset(std::forward<Args>(args)...);
            if(bs < _min) {
                _min = bs;
            }
        }

        template<typename Plan, typename... Args>
        void operator()(Plan* plan, Args&&... args) {
            this->operator()(*plan, std::forward<Args>(args)...);
        }

        data::DimensionSize<data::Time> const& value() const
        {
            return _min;
        }

    private:
        data::DimensionSize<data::Time> _min;
};

class DispersionStrategySyncer {
    public:
        template<typename Plan>
        void operator()(Plan& plan, data::DimensionSize<data::Time> const& spectra) {
            if(plan.number_of_spectra()!=spectra)
            {
                plan.reset(spectra);
                if(plan.number_of_spectra()!=spectra)
                    throw panda::Error("Unable to set number of spectra");
            }
        }

        template<typename Plan>
        void operator()(Plan* plan, data::DimensionSize<data::Time> const& spectra) {
            this->operator()(*plan, spectra);
        }
};

} // namespace

template<typename TraitsT, typename... AlgosT>
data::DimensionSize<data::Time> CommonDedispersionPlan<TraitsT, AlgosT...>::buffer_overlap() const
{
    BufferOverlapHelper helper;
    panda::for_each(_plans, helper);
    return helper.value();
}

template<typename TraitsT, typename... AlgosT>
data::DimensionSize<data::Time> CommonDedispersionPlan<TraitsT, AlgosT...>::dedispersion_strategy(TimeFrequencyType const& data)
{
    DispersionStrategyHelper helper;
    panda::for_each(_plans, helper, data);

    // setting the minimum dedipsersion samples for each plan
    DispersionStrategySyncer sync_helper;
    panda::for_each(_plans, sync_helper, helper.value());

    // propagate dedispersionplans to dedispersion algorithms
    this->set_plans();

    return helper.value();
}

template<typename TraitsT, typename... AlgosT>
template<typename AlgoT>
typename PlanType<AlgoT>::type const& CommonDedispersionPlan<TraitsT, AlgosT...>::plan() const
{
    return std::get<panda::Index<typename PlanType<AlgoT>::type, decltype(_plans)>::value>(_plans);
}

template<class CommonDedispersionPlanType, typename... AlgosT>
data::DimensionSize<data::Time> CommonDedispersionPlanFilter<CommonDedispersionPlanType, AlgosT...>::buffer_overlap() const
{
    BufferOverlapHelper helper;
    panda::for_each(_filtered_plans, helper);
    return helper.value();
}

template<class CommonDedispersionPlanType, typename... AlgosT>
data::DimensionSize<data::Time> CommonDedispersionPlanFilter<CommonDedispersionPlanType, AlgosT...>::dedispersion_strategy(TimeFrequencyType const& data)
{
    DispersionStrategyHelper helper;
    panda::for_each(_filtered_plans, helper, data);

    // setting the minimum dedipsersion samples for each plan
    DispersionStrategySyncer sync_helper;
    panda::for_each(_filtered_plans, sync_helper, helper.value());

    // propagate dedispersionplans to dedispersion algorithms
    this->set_plans();

    return helper.value();
}

template<class CommonDedispersionPlanType, typename... AlgosT>
template<typename AlgoT>
constexpr bool CommonDedispersionPlanFilter<CommonDedispersionPlanType, AlgosT...>::has_algo()
{
    return panda::HasType<typename PlanType<AlgoT>::type*, decltype(_filtered_plans)>::value;
}

} // namespace ddtr
} // namespace modules
} // namespace cheetah
} // namespace ska
