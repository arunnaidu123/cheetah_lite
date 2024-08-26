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
#ifndef SKA_CHEETAH_PIPELINES_SEARCH_PIPELINE_PIPELINEHANDLERFACTORY_H
#define SKA_CHEETAH_PIPELINES_SEARCH_PIPELINE_PIPELINEHANDLERFACTORY_H

#include "cheetah/pipelines/search_pipeline/PipelineHandler.h"
#include <map>
#include <vector>
#include <functional>
#include "cheetah/data/TimeFrequency.h"

namespace ska {
namespace cheetah {
namespace pipelines {
namespace search_pipeline {

template<typename NumericalT>
class CheetahConfig;

template<typename NumericalT>
class BeamConfig;

/**
 * @brief
 *    Generates pipeline handler objects by name
 *
 * @details
 * @throw Error if asked for type does not exist
 */
class PipelineHandlerFactory
{
    public:
        //typedef std::function<void(data::TimeFrequency&)> HandlerType;
        typedef uint8_t NumericalT;
        typedef PipelineHandler<NumericalT> HandlerType;

    private:
        typedef std::function<HandlerType*(CheetahConfig<NumericalT> const&, BeamConfig<NumericalT> const&)> FactoryType;

    public:
        PipelineHandlerFactory(CheetahConfig<NumericalT>& config);
        ~PipelineHandlerFactory();

        /**
         * @brief return a vector of available pipeline
         */
        std::vector<std::string> available() const;

        /**
         * @brief create a handler of the named type
         */
        HandlerType* create(std::string const& type, BeamConfig<NumericalT> const&) const;

        /**
         * @brief create a handler of the named type with a MixInTimer to record the time of each invocation
         */
        HandlerType* create_timed(std::string const& type, BeamConfig<NumericalT> const&) const;

    private:
        /**
         * @brief adds a factory method to be looked up.
         * @details use a template method in order to retain
         *          type information as this is used to generate MixIn factories (e.g. timed pipelines)
         */
        template<typename TypeFactory>
        void add_type(std::string const& handler_name, TypeFactory factory);

        template<typename TypeFactory>
        void add_factory(std::string const& handler_name, TypeFactory const& factory);

    private:
        CheetahConfig<NumericalT> const& _config;
        std::map<std::string, FactoryType> _map;
        std::map<std::string, FactoryType> _timed_map;
        std::vector<std::string> _types;
};

} // namespace search_pipeline
} // namespace pipelines
} // namespace cheetah
} // namespace ska

#endif // SKA_CHEETAH_PIPELINES_SEARCH_PIPELINE_PIPELINEHANDLERFACTORY_H
