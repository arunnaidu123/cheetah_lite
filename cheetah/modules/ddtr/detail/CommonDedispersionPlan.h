/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 The SKA organisation
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
#ifndef SKA_CHEETAH_MODULES_DDTR_COMMONDEDISPERSIONPLAN_H
#define SKA_CHEETAH_MODULES_DDTR_COMMONDEDISPERSIONPLAN_H

#include "cheetah/modules/ddtr/cpu/DedispersionPlan.h"
#include "cheetah/modules/ddtr/cpu/Ddtr.h"
#include "cheetah/modules/ddtr/Config.h"
#include "cheetah/data/Units.h"
#include "panda/ConfigurableTask.h"
#include "panda/Method.h"
#include "panda/TupleUtilities.h"
#include <tuple>


namespace ska {
namespace cheetah {
namespace modules {
namespace ddtr {

template<typename AlgoT>
struct PlanType {
    typedef typename AlgoT::DedispersionPlan type;
};

template<typename TraitsT>
class CommonDedispersionPlanBase
{
        typedef typename TraitsT::TimeFrequencyType TimeFrequencyType;

    public:
        virtual ~CommonDedispersionPlanBase() {}
        virtual data::DimensionSize<data::Time> buffer_overlap() const;
        virtual data::DimensionSize<data::Time> dedispersion_strategy(TimeFrequencyType const&);
};

template<typename TraitsT, typename... AlgosT>
class CommonDedispersionPlan : public CommonDedispersionPlanBase<TraitsT>
{
    protected:
        typedef std::tuple<typename AlgosT::Architecture ...> Architectures;

    public:
        typedef typename TraitsT::Config ConfigType;
        typedef typename TraitsT::BeamConfigType BeamConfigType;
        typedef typename TraitsT::TimeFrequencyType TimeFrequencyType;

    public:
        CommonDedispersionPlan(BeamConfigType const& beam_config, ConfigType const& config);
        virtual ~CommonDedispersionPlan();

        data::DimensionSize<data::Time> buffer_overlap() const override;
        data::DimensionSize<data::Time> dedispersion_strategy(TimeFrequencyType const&) override;

        /**
         * @brief return the plan corresponding to the specified algorithm
         */
        template<typename AlgoT>
        typename PlanType<AlgoT>::type const& plan() const;

        template<typename AlgoT>
        static constexpr bool has_algo()
        {
            return panda::HasType<typename PlanType<AlgoT>::type, decltype(_plans)>::value;
        }

    protected:
        virtual void set_plans() = 0;

    protected:
        std::tuple<typename PlanType<AlgosT>::type...> _plans;
};


template<class CommonDedispersionPlanType, typename... AlgosT>
class CommonDedispersionPlanFilter : public CommonDedispersionPlanType
{
    public:
        typedef CommonDedispersionPlanType BaseT;
        typedef typename BaseT::BeamConfigType BeamConfigType;
        typedef typename BaseT::ConfigType ConfigType;
        typedef typename BaseT::TimeFrequencyType TimeFrequencyType;
        typedef std::tuple<typename AlgosT::Architecture ...> Architectures;
        typedef std::tuple<typename PlanType<AlgosT>::type*...> FilteredPlansType;

    private:
        struct FilterHelper
        {
            FilterHelper(FilteredPlansType& filtered_plans)
                : _filtered_plans(filtered_plans)
            {}

            template<typename Plan
                    , typename std::enable_if<panda::HasType<Plan*, FilteredPlansType>::value, bool>::type = true
                    >
            void operator()(Plan& plan) {
                std::get<panda::Index<Plan*, FilteredPlansType>::value>(_filtered_plans) = &plan;
            }

            template<typename Plan
                    , typename std::enable_if<!panda::HasType<Plan*, FilteredPlansType>::value, bool>::type = true
                    >
            void operator()(Plan&) {
            }

            private:
                FilteredPlansType& _filtered_plans;

        };

    public:
        CommonDedispersionPlanFilter(BeamConfigType const& beam_config, ConfigType const& config)
            : BaseT(beam_config, config)
        {
            FilterHelper helper(_filtered_plans);
            panda::for_each(this->_plans, helper);
        }

        data::DimensionSize<data::Time> buffer_overlap() const override;
        data::DimensionSize<data::Time> dedispersion_strategy(TimeFrequencyType const&) override;

        template<typename AlgoT>
        static constexpr bool has_algo();

    private:
        FilteredPlansType _filtered_plans;
};

} // namespace ddtr
} // namespace modules
} // namespace cheetah
} // namespace ska
#include "cheetah/modules/ddtr/detail/CommonDedispersionPlan.cpp"
#endif // SKA_CHEETAH_MODULES_DDTR_COMMONDEDISPERSIONPLAN_H
