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
#ifndef SKA_CHEETAH_PIPELINES_SEARCH_PIPELINE_BEAMLAUNCHER_H
#define SKA_CHEETAH_PIPELINES_SEARCH_PIPELINE_BEAMLAUNCHER_H

#include "cheetah/pipelines/search_pipeline/MultiBeamConfig.h"
#include "cheetah/pipelines/search_pipeline/BeamConfig.h"
#include "cheetah/pipelines/search_pipeline/PipelineHandler.h"
#include <vector>
#include <functional>
#include <condition_variable>
#include <mutex>

namespace ska {
namespace cheetah {
namespace pipelines {
namespace search_pipeline {

/**
 *  @brief An object for launching multiple pipeline instances withina single
 *  cheetah pipeline process
 *
 */

namespace detail {
    struct PipelineWrapperBase;
    struct PipelineHandlerWrapperBase;
}

template<typename StreamType, typename NumericalT>
class BeamLauncher
{
    public:

        template<typename StreamConfigFactory, typename PipelineFactory>
        BeamLauncher(MultiBeamConfig<NumericalT> const& mb_config, StreamConfigFactory const& config_factory, PipelineFactory const& pipeline_factory);
        ~BeamLauncher();

        /** @brief launch the beam pipelines
         * @param PipelineFactory a functor that will return a suitable runtime handler of the incoming data
         * @details this function will block until join() is called, or the pipelines terminate in some other way
         */
        int exec();

        /// Wait for threads to finish
        void join();

        /// return true if all activated beams are running
        bool is_running() const;

        /// return the vector of unique pointers to all the streams
        std::vector<std::unique_ptr<StreamType>>& streams();

    private:
        MultiBeamConfig<NumericalT> const& _multi_beam_config;
        bool _exit;
        std::mutex _mutex;
        std::condition_variable _wait_cv;
        std::vector<std::unique_ptr<panda::Thread>> _threads;
        std::vector<panda::ThreadConfig const*> _thread_config;
        std::vector<std::unique_ptr<StreamType>> _streams;
        std::vector<std::unique_ptr<detail::PipelineHandlerWrapperBase>> _runtime_handlers;
        std::vector<std::unique_ptr<detail::PipelineWrapperBase>> _pipelines;
        std::atomic<std::size_t> _execution_count;
};

} // namespace search_pipeline
} // namespace pipelines
} // namespace cheetah
} // namespace ska

#include "cheetah/pipelines/search_pipeline/detail/BeamLauncher.cpp"

#endif // SKA_CHEETAH_PIPELINES_SEARCH_PIPELINE_BEAMLAUNCHER_H
