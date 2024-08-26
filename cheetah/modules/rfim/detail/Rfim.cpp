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
#include "cheetah/modules/rfim/Rfim.h"
#include "cheetah/modules/rfim/ampp/Rfim.h"
#include "cheetah/modules/rfim/cuda/Rfim.h"
#include "cheetah/modules/rfim/sum_threshold/Rfim.h"
#include "cheetah/modules/rfim/iqrmcpu/Rfim.h"
#include "panda/Error.h"
#include "panda/Log.h"

namespace ska {
namespace cheetah {
namespace modules {
namespace rfim {

namespace {
    template<typename Policy>
    class Null
    {
            typedef typename PolicyInfo<Policy>::DataArgumentType ArgumentType;
            typedef typename PolicyInfo<Policy>::ReturnType ReturnType;

        public:
            typedef cheetah::Cpu Architecture;

            template<typename ResourceType>
            ReturnType operator()(ResourceType& , std::shared_ptr<typename std::remove_reference<ArgumentType>::type> const& data)
            {
                return _policy.null_op(*data);
            }


        private:
            policy::Policy<Policy> _policy;
    };

template<typename Algo>
struct RfimCreateHelper {
    template<typename ConfigT, typename BandpassHandler>
    static inline
    Algo create(ConfigT const& config, BandpassHandler&&)
    {
        return Algo(config.template config<typename Algo::Config>());
    }
};

template<typename TraitsT>
struct RfimCreateHelper<cuda::Rfim<TraitsT>>
{
    template<typename ConfigT, typename BandpassHandler>
    static inline
    cuda::Rfim<TraitsT> create(ConfigT const& config, BandpassHandler&&)
    {
        PANDA_LOG << "rfim::cuda algorithm activated";
        return cuda::Rfim<TraitsT>(config.cuda_algo_config());
    }
};

template<typename TraitsT>
struct RfimCreateHelper<iqrmcpu::Rfim<TraitsT>>
{
    template<typename ConfigT, typename BandpassHandler>
    static inline
    iqrmcpu::Rfim<TraitsT> create(ConfigT const& config, BandpassHandler&&)
    {
        PANDA_LOG << "rfim::iqrmcpu algorithm activated";
        return iqrmcpu::Rfim<TraitsT>(config.iqrmcpu_algo_config());
    }
};

template<typename TraitsT>
struct RfimCreateHelper<sum_threshold::Rfim<TraitsT>>
{
    template<typename ConfigT, typename BandpassHandler>
    static inline
    sum_threshold::Rfim<TraitsT> create(ConfigT const& config, BandpassHandler&&)
    {
        PANDA_LOG << "rfim::sum_threshold algorithm activated";
        return sum_threshold::Rfim<TraitsT>(config.sum_threshold_algo_config());
    }
};

template<typename TraitsT>
struct RfimCreateHelper<ampp::Rfim<TraitsT>>
{
    template<typename ConfigT, typename BandpassHandler>
    static inline
    ampp::Rfim<TraitsT> create(ConfigT const& config, BandpassHandler&& handler)
    {
        PANDA_LOG << "rfim::ampp algorithm activated";
        return ampp::Rfim<TraitsT>(config.ampp_algo_config(), std::forward<BandpassHandler>(handler));
    }
};

template<typename ConfigType, typename RfimTraits>
struct RfimFactory
{
    public:
        RfimFactory(ConfigType const& config, typename RfimTraits::BandPassHandler& bp_handler)
            : _config(config)
            , _bp_handler(bp_handler)
        {}

        template<typename Algo>
        Algo create()
        {
            return RfimCreateHelper<Algo>::create(_config, _bp_handler);
        }

        template<typename Algo>
        bool active() const {
            return _config.template config<typename Algo::Config>().active();
        }

    private:
        ConfigType const& _config;
        typename RfimTraits::BandPassHandler& _bp_handler;
};

} // namespace



template<typename TimeFrequencyType, typename RfimTraits, typename ConfigType>
Rfim<TimeFrequencyType, RfimTraits, ConfigType>::Rfim(ConfigType const& c
                                                     , typename RfimTraits::RfimHandler& handler
                                                     , typename RfimTraits::BandPassHandler& bp_handler
                                                     )
    : _task(c.pool(), handler)
{
    // select algos according to specified configuration options
    RfimFactory<ConfigType, RfimTraits> factory(c, bp_handler);
    if(!_task.configure(factory
                      , Select<rfim::sum_threshold::Rfim<RfimTraits>>(c.sum_threshold_algo_config().active())
                      , Select<rfim::cuda::Rfim<RfimTraits>>(c.cuda_algo_config().active())
                      , Select<rfim::ampp::Rfim<RfimTraits>>(c.ampp_algo_config().active())
                      , Select<rfim::iqrmcpu::Rfim<RfimTraits>>(c.iqrmcpu_algo_config().active())
                   ))
    {
        _task.template set_algorithms<Null<PolicyType>>(std::move(Null<PolicyType>()));
        PANDA_LOG_WARN << "WARNING: no RFI mitigation algorithm has been specified";
    }
}

template<typename TimeFrequencyType, typename RfimTraits, typename ConfigType>
Rfim<TimeFrequencyType, RfimTraits, ConfigType>::~Rfim()
{
}

template<typename TimeFrequencyType, typename RfimTraits, typename ConfigType>
void Rfim<TimeFrequencyType, RfimTraits, ConfigType>::run(ArgumentType data)
{
    auto ptr = data.shared_from_this();
    _task.submit(ptr);
}

} // namespace rfim
} // namespace modules
} // namespace cheetah
} // namespace ska
