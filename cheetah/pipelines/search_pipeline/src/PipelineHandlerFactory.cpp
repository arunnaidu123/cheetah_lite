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
#include "cheetah/pipelines/search_pipeline/PipelineHandlerFactory.h"
#include "cheetah/pipelines/search_pipeline/CheetahConfig.h"

// included pipelines
#include "cheetah/pipelines/search_pipeline/Empty.h"
#include "cheetah/pipelines/search_pipeline/SinglePulse.h"
#include "cheetah/pipelines/search_pipeline/RfiDetectionPipeline.h"

#include "panda/Error.h"
#include "panda/MixInTimer.h"
#include <chrono>
#include <utility>
#include <type_traits>

namespace ska {
namespace cheetah {
namespace pipelines {
namespace search_pipeline {

template<typename T, typename NumericalT>
static
T* make_handler(CheetahConfig<NumericalT> const& config, BeamConfigType<NumericalT> const& beam_config) {
    return new T(config, beam_config);
}

// wrapper to protect the pipeline from being destroyed before tasks in the pools finish
template<typename NumericalT, typename Base>
class PipelineWrapper : public Base
{
    public:
        template<typename... Args>
        PipelineWrapper(CheetahConfig<NumericalT> const& config, Args&&... args)
            : Base(config, std::forward<Args>(args)...)
            , _config(config)
        {
        }

        PipelineWrapper()
        {
            _config.pool_manager().wait();
        }

    private:
        CheetahConfig<NumericalT> const& _config;
};

// wrapper to implement required virtual function of PipelineHandler
template<typename NumericalT, typename Base>
class TimerWrapper : public PipelineWrapper<NumericalT, panda::MixInTimer<Base>>
{
        typedef PipelineWrapper<NumericalT, panda::MixInTimer<Base>> BaseT;

    public:
        template<typename... Args>
        TimerWrapper(Args&&... args)
            : BaseT(std::forward<Args>(args)...)
        {
        }

};

template<typename NumericalT>
class TimedPipeline : public PipelineHandler<NumericalT>
{
        typedef std::chrono::high_resolution_clock ClockType;
        typedef PipelineHandler<NumericalT> BaseT;

    public:
        TimedPipeline(CheetahConfig<NumericalT> const& config, BeamConfigType<NumericalT> const& beam_config, PipelineHandler<NumericalT>* handler)
            : BaseT(config, beam_config)
            , _handler(handler)
        {
        }

        ~TimedPipeline()
        {
            delete _handler;
        }

        // costs us an extra indirection, but I can't see another way of doing it
        void operator()(data::TimeFrequency<Cpu, NumericalT>& data) override
        {
            auto start=ClockType::now();
            (*_handler)(data);
            auto end=ClockType::now();
            PANDA_LOG << "timing: " << std::chrono::duration_cast<std::chrono::microseconds>(start - end).count() << "microseconds";
        }

    private:
        PipelineHandler<NumericalT>* _handler;
};

#define REGISTER_HANDLER(handler_name) \
    add_type(#handler_name, make_handler<handler_name<NumericalT>, NumericalT>)

PipelineHandlerFactory::PipelineHandlerFactory(CheetahConfig<NumericalT>& config)
    : _config(config)
{
    // add known handlers
    REGISTER_HANDLER(Empty);
    REGISTER_HANDLER(RfiDetectionPipeline);
    REGISTER_HANDLER(SinglePulse);

    config.set_pipeline_handlers(available());
}

PipelineHandlerFactory::~PipelineHandlerFactory()
{
}

template<typename TypeFactory>
void PipelineHandlerFactory::add_factory(std::string const& handler_name, TypeFactory const& factory)
{
    typedef typename std::remove_pointer<decltype(std::declval<TypeFactory>()(_config, std::declval<BeamConfigType<NumericalT> const&>()))>::type Type;
    _map.insert( std::make_pair( handler_name
                               , [factory](CheetahConfig<NumericalT> const& config, BeamConfigType<NumericalT> const& beam_config)
                                 {
                                     return factory(config, beam_config);
                                 }
                               )
               );
    _timed_map.insert( std::make_pair( handler_name
                                     , [factory](CheetahConfig<NumericalT> const& config, BeamConfigType<NumericalT> const& beam_config)
                                       {
                                                auto handler = factory(config, beam_config);
                                                try {
                                                    return new TimedPipeline<NumericalT>(config, beam_config, handler);
                                                } catch(...) {
                                                    delete handler;
                                                    throw;
                                                }
                                       }
                ));
}

template<typename TypeFactory>
void PipelineHandlerFactory::add_type(std::string const& handler_name, TypeFactory /*factory*/)
{
    _types.push_back(handler_name);
    //_map.insert(std::make_pair(handler_name, static_cast<PipelineHandlerFactory::FactoryType>(factory)));
    // add a timer type
    typedef typename std::remove_pointer<decltype(std::declval<TypeFactory>()(_config, std::declval<BeamConfigType<NumericalT> const&>()))>::type Type;
    _map.insert(std::make_pair(handler_name, &make_handler<PipelineWrapper<NumericalT, Type>, NumericalT>));
    _timed_map.insert(std::make_pair(handler_name, &make_handler<TimerWrapper<NumericalT, Type>, NumericalT>));
}

std::vector<std::string> PipelineHandlerFactory::available() const
{
    return _types;
}

PipelineHandlerFactory::HandlerType* PipelineHandlerFactory::create(std::string const& handler_name, BeamConfigType<NumericalT> const& beam_config) const
{
    auto it = _map.find(handler_name);
    if(it == _map.end())
        throw panda::Error("Pipeline handler '" + handler_name + "' unknown");

    return it->second(_config, beam_config);
}

PipelineHandlerFactory::HandlerType* PipelineHandlerFactory::create_timed(std::string const& handler_name, BeamConfigType<NumericalT> const& beam_config) const
{
    auto it = _timed_map.find(handler_name);
    if(it == _timed_map.end())
        throw panda::Error("Pipeline handler '" + handler_name + "' unknown");

    return it->second(_config, beam_config);
}

} // namespace search_pipeline
} // namespace pipelines
} // namespace cheetah
} // namespace ska
