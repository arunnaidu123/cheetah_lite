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
#include "cheetah/utils/TaskConfigurationSetter.h"
#include <thread>

namespace ska {
namespace cheetah {
namespace modules {
namespace ddtr {
namespace detail {

template<typename T, typename DdtrTraits>
struct CreateHelper
{
    template<typename DdtrAlgoFactory>
    static inline T create(DdtrAlgoFactory& algo_factory)
    {
        return T(algo_factory._config);
    }
};

template<typename DdtrTraits>
struct CreateHelper<cpu::Ddtr<DdtrTraits>, DdtrTraits>
{
    template<typename DdtrAlgoFactory>
    static inline cpu::Ddtr<DdtrTraits> create(DdtrAlgoFactory& algo_factory)
    {
        return cpu::Ddtr<DdtrTraits>(algo_factory._config);
    }
};

template<class DdtrTraits>
class DdtrAlgoFactory
{
    private:
        template<typename, typename>
        friend struct detail::CreateHelper;
        typedef typename DdtrTraits::Config ConfigType;

    public:
        DdtrAlgoFactory(ConfigType const& config)
            : _config(config)
        {}

        void none_selected() const {
            PANDA_LOG_WARN << "ddtr:: no algorithm has been activated. Please set at least one ddtr algorithm to active.";
        }

        template<typename T> T create() const
        {
            return detail::CreateHelper<T, DdtrTraits>::create(*this);
        }

    private:
        ConfigType const& _config;
};

} // namespace detail

template<typename DdtrTraits, template<typename> class... DdtrAlgorithms>
template<typename FactoryT>
struct DdtrModule<DdtrTraits, DdtrAlgorithms...>::FactoryWrap
{
    private:
        template<typename, typename>
        friend struct detail::CreateHelper;
        typedef typename DdtrTraits::Config ConfigType;
        typedef typename BufferType::Plan PlanType;

    public:
        FactoryWrap(ConfigType const& config, FactoryT const& factory, BufferType& buffer, TaskType& task)
            : _config(config)
            , _factory(factory)
            , _buffer(buffer)
            , _task(task)
        {}

        void none_selected() const {
            _factory.none_selected();
        }

        template<typename T> T create() const
        {
            return _factory.template create<T>();
        }

        template<typename Algo>
        bool active() const
        {
            return _config.template config<typename Algo::Config>().active();
        }

        template<typename... Algos>
        void exec()
        {
            _buffer.plan(std::unique_ptr<PlanType>(new ExtendedDedispersionPlan<Algos...>(_config, _task)));
        }

    private:
        ConfigType const& _config;
        FactoryT const& _factory;
        BufferType& _buffer;
        TaskType& _task;
};


template<typename DdtrTraits, template<typename> class... DdtrAlgorithms>
template<typename Handler
        ,typename std::enable_if<!HasAlgoFactoryTypedef<DdtrTraits>::value && !std::is_same<void, Handler>::value, bool>::type
        >
DdtrModule<DdtrTraits, DdtrAlgorithms...>::DdtrModule(ConfigType const& config, Handler&& handler)
    : DdtrModule(config, std::forward<Handler>(handler), AlgoFactoryType(config))
{
}

template<typename DdtrTraits, template<typename> class... DdtrAlgorithms>
template<typename Handler, typename... AggBufferArgs>
DdtrModule<DdtrTraits, DdtrAlgorithms...>::DdtrModule(ConfigType const& config, Handler&& handler
                                                     , AlgoFactoryType const& factory
                                                     , AggBufferArgs&&... agg_buffer_args
                                                     )
    : _task(config.pool(), std::forward<Handler>(handler), _plan_setter)
    , _buffer([this](std::shared_ptr<typename DdtrTraits::BufferType> buffer)
              {
                  //if(buffer.composition().empty())
                  //{
                  //    PANDA_LOG_WARN << "received an empty buffer";
                  //    return;
                  //}
                  // fix up broken buffer metadata - TODO fix this in buffer creation
                  //auto const& tf_obj = *(buffer.composition().front());
                  //buffer.buffer().metadata(tf_obj.metadata());
                  //data::DimensionIndex<data::Time> offset_samples(buffer.offset_first_block()/(tf_obj.number_of_channels() * sizeof(typename DdtrTraits::value_type)));
                  //buffer.buffer().start_time(tf_obj.start_time(offset_samples));
                  _task.submit(buffer);
              }
              , ExtendedDedispersionPlan<>(config, _task)
              , config.dedispersion_samples()
              , std::forward<AggBufferArgs>(agg_buffer_args)...
              )
{
    FactoryWrap<AlgoFactoryType> wrap_factory(config, factory, _buffer, _task);
    utils::TaskConfigurationSetter<DdtrAlgorithms<DdtrTraits>...>::configure(_task, wrap_factory);
}

template<typename DdtrTraits, template<typename> class... DdtrAlgorithms>
template<typename DataT>
void DdtrModule<DdtrTraits, DdtrAlgorithms...>::operator()(DataT&& data)
{
    this->_buffer(std::forward<DataT>(data));
}


} // namespace ddtr
} // namespace modules
} // namespace cheetah
} // namespace ska
