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
#ifndef SKA_CHEETAH_MODULES_DDTR_DDTRMODULE_H
#define SKA_CHEETAH_MODULES_DDTR_DDTRMODULE_H

#include "cheetah/modules/ddtr/detail/Buffering.h"
#include "panda/ConfigurableTask.h"
#include "panda/Method.h"
#include "panda/TupleUtilities.h"
#include <atomic>
#include <type_traits>

namespace ska {
namespace cheetah {
namespace modules {
namespace ddtr {

TYPEDEF_TESTER(HasAlgoFactoryTypedef, AlgoFactory);

namespace detail {
template<typename T>
class DdtrAlgoFactory;

template<class DdtrTraits, typename Enable=void>
struct DdtrTraitsGen : DdtrTraits
{
};

template<class DdtrTraits>
struct DdtrTraitsGen<DdtrTraits
                    , typename std::enable_if<!HasAlgoFactoryTypedef<DdtrTraits>::value>::type>
     : DdtrTraits
{
    typedef DdtrAlgoFactory<DdtrTraits> AlgoFactory;
};

} // namesapce detail

/**
 * @brief Wrap Ddtr Algorithms to provide buffering and the top level interfaces
 * @details
 */

template<typename DdtrTraits, template<typename> class... DdtrAlgorithms>
class DdtrModule
{
    private: // typedefs
        typedef CommonDedispersionPlan<DdtrTraits, DdtrAlgorithms<DdtrTraits>...> DedispersionPlanType;
        typedef detail::DdtrTraitsGen<DdtrTraits> DdtrTraitsFactory;
        typedef typename DdtrTraitsFactory::AlgoFactory AlgoFactoryType;

    private: // structs
        template<typename FactoryT>
        struct FactoryWrap;
        struct SetDedispersionHelper
        {
            template<typename Algo
                    , typename DedispersionPlanType
                    , typename std::enable_if<(DedispersionPlanType::template has_algo<Algo>()), bool>::type = true
                    >
            void operator()(Algo& algo, DedispersionPlanType const& plan) {
                algo.plan(plan.template plan<Algo>());
            }

            template<typename Algo
                    , typename DedispersionPlanType
                    , typename std::enable_if<(!DedispersionPlanType::template has_algo<Algo>()), bool>::type = true
                    >
            void operator()(Algo&, DedispersionPlanType const&)
            {
            }
        };

        typedef panda::ConfigurableTask<typename DdtrTraits::Pool
                                      , typename DdtrTraits::DedispersionHandler
                                      , panda::Method<SetDedispersionHelper, DedispersionPlanType const&>
                                      , std::shared_ptr<typename DdtrTraits::BufferType>&> TaskType;

        typedef typename DdtrTraits::Config ConfigType;
        typedef Buffering<DdtrTraits, CommonDedispersionPlanBase<DdtrTraits>> BufferType;

    public:
        template<typename DedispersionHandler
                ,typename std::enable_if<!HasAlgoFactoryTypedef<DdtrTraits>::value && !std::is_same<void, DedispersionHandler>::value, bool>::type = true>
        DdtrModule(ConfigType const& config, DedispersionHandler&& handler);

        template<typename DedispersionHandler, typename... AggBufferArgs>
        DdtrModule(ConfigType const& config, DedispersionHandler&& handler
                 , AlgoFactoryType const& algo_factory
                 , AggBufferArgs&&... agg_buffer_args
                 );

        template<typename DataT>
        void operator()(DataT&& data);

    private:
        template<typename... Algos>
        struct ExtendedDedispersionPlan
            : public ExtendedDedispersionPlan<DdtrAlgorithms<DdtrTraits>...>
        {
            using ExtendedDedispersionPlan<DdtrAlgorithms<DdtrTraits>...>::ExtendedDedispersionPlan;
        };

        template<typename Algo, typename... Algos>
        struct ExtendedDedispersionPlan<Algo, Algos...> : public CommonDedispersionPlanFilter<DedispersionPlanType, Algo, Algos...>
        {
            typedef CommonDedispersionPlanFilter<DedispersionPlanType, Algo, Algos...> BaseT;
            typedef typename BaseT::TimeFrequencyType TimeFrequencyType;
            ExtendedDedispersionPlan(ConfigType const& config, TaskType& task)
                : BaseT(config)
                , _task(task)
            {
            }

            void set_plans() override {
                _task(static_cast<BaseT const&>(*this));
            }

            data::DimensionSize<data::Time> buffer_overlap() const override
            {
                return BaseT::buffer_overlap();
            }

            data::DimensionSize<data::Time> dedispersion_strategy(TimeFrequencyType const& tf) override
            {
                return BaseT::dedispersion_strategy(tf);
            }
            private:
                TaskType& _task;
        };

    private:
        SetDedispersionHelper _plan_setter;

    protected:
        TaskType _task;
        BufferType _buffer;
};

} // namespace ddtr
} // namespace modules
} // namespace cheetah
} // namespace ska
#include "cheetah/modules/ddtr/detail/DdtrModule.cpp"

#endif // SKA_CHEETAH_MODULES_DDTR_DDTRMODULE_H
