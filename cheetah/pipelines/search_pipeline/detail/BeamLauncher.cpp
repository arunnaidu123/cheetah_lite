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
#include "cheetah/pipelines/search_pipeline/BeamLauncher.h"
#include "cheetah/pipelines/search_pipeline/Pipeline.h"
#include "cheetah/pipelines/search_pipeline/PipelineHandlerFactory.h"
#include "panda/Thread.h"
#include <algorithm>
#include <type_traits>

namespace ska {
namespace cheetah {
namespace pipelines {
namespace search_pipeline {

namespace detail {
    struct PipelineWrapperBase {
            PipelineWrapperBase(std::string const& id);
            virtual ~PipelineWrapperBase() {}
            virtual int operator()() = 0;
            virtual void stop() = 0;

            std::string const& id() const;

        private:
            std::string _id;
    };

    PipelineWrapperBase::PipelineWrapperBase(std::string const& id)
        : _id(id)
    {
    }

    std::string const& PipelineWrapperBase::id() const {
        return _id;
    }


    template<typename P>
    struct PipelineWrapper final : public PipelineWrapperBase
    {
            PipelineWrapper(P&& p, std::string const& id)
                : PipelineWrapperBase(id)
                , _pipeline(std::move(p))
                , _stop(false)
            {}

            int operator()() override {
                try
                {
                    std::lock_guard<std::mutex> lock(_mutex);
                    if(! _stop)
                    {
                        return _pipeline->exec();
                    }
                }
                catch(std::exception const& e)
                {
                    PANDA_LOG_ERROR << "exception caught:" << e.what();
                    stop();
                }
                catch(...)
                {
                    PANDA_LOG_DEBUG << "unknown exception caught!";
                    stop();
                }
                return 0;
            }

            void stop() override {
                PANDA_LOG << "Stopping Beam: " << id();
                std::unique_lock<std::mutex> lock(_mutex, std::defer_lock);
                if(lock.try_lock())
                {
                    // indicate we don't want to start up after all
                    _stop = true;
                }
                else {
                    // we will only fail to get the lock if the pipeline is running
                    while(!_pipeline->is_running()) {
                        // we may have to wait for a pipeline to fully start up before we can stop it
                        if(lock.try_lock()) {
                            // the pipeline has been stopped by some other process
                            _stop=false;
                            return;
                        }
                    }
                    _pipeline->stop();
                    lock.lock();
                    _stop=false;
                }
            }

        private:
            P _pipeline;
            std::mutex _mutex;
            bool _stop; // workaround for out of order start/stop calls
    };

    struct PipelineHandlerWrapperBase {
            PipelineHandlerWrapperBase() {};
            virtual ~PipelineHandlerWrapperBase() {}
    };

    template<typename P>
    struct PipelineHandlerWrapper final : public PipelineHandlerWrapperBase
    {
            PipelineHandlerWrapper(P* pipeline)
                : _pipeline(pipeline) {}

            PipelineHandlerWrapper(PipelineHandlerWrapper&& p)
                : _pipeline(std::move(p._pipeline)) {}

            P& operator*() { return *_pipeline; }

        private:
            std::unique_ptr<P> _pipeline;
    };
} // namespace detail

template<typename InputDataStream, typename NumericalT>
template<typename ConfigFactory, typename PipelineFactory>
BeamLauncher<InputDataStream, NumericalT>::BeamLauncher(MultiBeamConfig<NumericalT> const& multi_beams_config, ConfigFactory const& config_factory, PipelineFactory const& runtime_handler_factory)
    : _multi_beam_config(multi_beams_config)
    , _exit(false)
    , _execution_count(0)
{
    auto it=multi_beams_config.beams();
    PANDA_LOG << "Creating Beams....";
    while(it != multi_beams_config.beams_end())
    {
        auto const& beam_config = *it;
        if(beam_config.active())
        {
            // create the stream
            _streams.emplace_back(std::move(std::unique_ptr<InputDataStream>(new InputDataStream(config_factory(beam_config)))));

            // create the compute section of the pipeline
            typedef decltype(runtime_handler_factory(beam_config)) RuntimeHandlerPtrType;
            typedef typename std::remove_pointer<RuntimeHandlerPtrType>::type RuntimeHandlerType;
            typedef detail::PipelineHandlerWrapper<RuntimeHandlerType> WrapperHandlerType;;
            _runtime_handlers.emplace_back(new WrapperHandlerType(runtime_handler_factory(beam_config)));

            // now init a suitable pipeline
            auto pipeline = create_pipeline<InputDataStream>(*_streams.back(), *static_cast<WrapperHandlerType&>(*_runtime_handlers.back()));
            _pipelines.emplace_back(new detail::PipelineWrapper<decltype(pipeline)>(std::move(pipeline), beam_config.id()));

            _thread_config.push_back(&beam_config.thread_config());
        }
        ++it;
    }
    PANDA_LOG << "Finished creating pipelines";
    if(_streams.size() == 0 ) {
        PANDA_LOG_WARN << "No beams have been defined";
    }
}

template<typename InputDataStream, typename NumericalT>
BeamLauncher<InputDataStream, NumericalT>::~BeamLauncher()
{
    join();
}

template<typename InputDataStream, typename NumericalT>
int BeamLauncher<InputDataStream, NumericalT>::exec()
{
    if(_execution_count != 0) return 0; // don't try to start if its already running
    std::atomic<std::size_t> execution_start;
    execution_start = 0;
    auto beam_thread = [&](detail::PipelineWrapperBase& pipeline, int& rv)
    {
        rv = 0;

        // run the thread
        ++_execution_count;
        ++execution_start;
        if(!_exit)
        {
            try
            {
                // Increment in execution_count and execution_start has to be in this order to avoid race condition in wait
                rv = pipeline();
            }
            catch(std::exception const& e) {
                PANDA_LOG_ERROR << "exception thrown:" << e.what();
            }
            catch(...) {
                PANDA_LOG_ERROR << "unknown exception thrown";
            }
        }
        --_execution_count;
        _wait_cv.notify_one();
    };

    if(_streams.size() == 0 ) return 0;

    std::unique_lock<std::mutex> lock(_mutex);
    std::vector<int> rv(_streams.size());
    { // lock context
        _exit = false;
        // launch all except one beam in threads
        for(std::size_t ii = 0; ii < _pipelines.size(); ++ii)
        {
            PANDA_LOG << "Starting Beam: " << _pipelines[ii]->id();
            _threads.emplace_back(new panda::Thread(_thread_config[ii]->affinities(), [&, ii, this]() { beam_thread(*_pipelines[ii], rv[ii]); } ));
        }
    }
    _wait_cv.wait(lock, [&, this]{
                return (!is_running()) && (execution_start==_pipelines.size());
                });

    for( auto & thread : _threads ) {
        thread->join();
    }
    _threads.clear();

    return std::accumulate(rv.begin(), rv.end(), 0);
}

template<typename InputDataStream, typename NumericalT>
bool BeamLauncher<InputDataStream, NumericalT>::is_running() const
{
    return _pipelines.size() == _execution_count;
}

template<typename InputDataStream, typename NumericalT>
void BeamLauncher<InputDataStream, NumericalT>::join()
{
    { // lock context
        std::lock_guard<std::mutex> lock(_mutex);
        if(!_exit) {
            // stop each pipeline
            _exit = true;
            for( auto const& pipeline : _pipelines )
            {
                pipeline->stop();
            }

            // wait for the threads to finish
            for( auto & thread : _threads )
            {
                thread->join();
            }
            _threads.clear();
            PANDA_LOG << "threads joined";
        }
    } // end lock context
    _wait_cv.notify_one();
}

template<typename InputDataStream, typename NumericalT>
std::vector<std::unique_ptr<InputDataStream>>& BeamLauncher<InputDataStream, NumericalT>::streams()
{
    return _streams;
}

} // namespace search_pipeline
} // namespace pipelines
} // namespace cheetah
} // namespace ska
