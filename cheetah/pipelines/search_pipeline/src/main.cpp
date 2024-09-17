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
/** @addtogroup apps
 * @{
 * @section app_launcher Pipeline Launch
 * @brief cheetah_pipeline The Executable to launch a variety of pipelines
 * @details
 *    This is the main cheetah executable and enables you to launch pipelines with a variety of options.
 *    It allows you to select:
 *    - the type of pipeline to run
 *    - the data sources (UDP packet stream data, sigproc files, generated data)
 *    - the data sink
 *
 *    for help on how to use see:
 *    @verbatim
       cheetah_pipeline --help
      @endverbatim
 *
 * @} */ // end group

#include "cheetah/pipelines/search_pipeline/CheetahConfig.h"
#include "cheetah/pipelines/search_pipeline/BeamLauncher.h"
#include "cheetah/utils/TerminateException.h"
#include "cheetah/io/producers/rcpt/SkaUdpStream.h"
#include "cheetah/sigproc/SigProcFileStream.h"
#include "cheetah/psrdada/SigProcDadaStream.h"
#include "cheetah/pipelines/search_pipeline/PipelineHandlerFactory.h"
#include "panda/MixInTimer.h"

using namespace ska::cheetah;

// Helper to launch UDP specific beams
template<io::producers::rcpt::SkaSelector::Stream Stream>
struct SelectUdpBeam : public io::producers::rcpt::SkaUdpStream<Stream>
{
    typedef io::producers::rcpt::SkaUdpStream<Stream> BaseT;

    template<typename NumericalT, typename HandlerFactoryT>
    static inline
    int exec(pipelines::search_pipeline::CheetahConfig<NumericalT> const& config, HandlerFactoryT& runtime_handler_factory)
    {
        int rv;
        pipelines::search_pipeline::BeamLauncher<typename BaseT::UdpStream, NumericalT> beam( config.beams_config()
                                                                          , [&](pipelines::search_pipeline::BeamConfigType<NumericalT> const& config) -> typename BaseT::Config const&
                                                                            { return config.template udp_rcpt_config<Stream>(); }
                                                                          , runtime_handler_factory);
        rv = beam.exec();
        config.pool_manager().wait();
        return rv;
    }
};

int main(int argc, char** argv) {

    int rv = 1; // program return value

    typedef uint8_t NumericalT;

    try {
        // -- configuration setup --
        pipelines::search_pipeline::CheetahConfig<NumericalT> config;
        pipelines::search_pipeline::PipelineHandlerFactory pipeline_factory(config);
        config.set_pipeline_handlers(pipeline_factory.available());

        // -- parse the command line --
        if( (rv=config.parse(argc, argv)) ) return rv;

        // -- create computation unit to run --
        std::function<pipelines::search_pipeline::PipelineHandlerFactory::HandlerType*(pipelines::search_pipeline::BeamConfigType<NumericalT> const&)> runtime_handler_factory;
        if(config.time_handler_invocation()) {
            runtime_handler_factory = [&](pipelines::search_pipeline::BeamConfigType<NumericalT> const& beam_config) {
                 return pipeline_factory.create_timed(config.pipeline_name(), beam_config);
            };
        }
        else {
            runtime_handler_factory = [&](pipelines::search_pipeline::BeamConfigType<NumericalT> const& beam_config) {
                return pipeline_factory.create(config.pipeline_name(), beam_config);
            };
        }

        // -- create and launch a suitable pipeline --

        std::string const& stream_name = config.stream_name();
        if(stream_name == "sigproc") {
            pipelines::search_pipeline::BeamLauncher<sigproc::SigProcFileStream, NumericalT> beam( config.beams_config(), [&](pipelines::search_pipeline::BeamConfigType<NumericalT> const& config) -> sigproc::Config const& { return config.sigproc_config(); }, runtime_handler_factory);
            rv = beam.exec();
            config.pool_manager().wait();
        }
        else {
            rv = io::producers::rcpt::SkaSelector::select<int,SelectUdpBeam>(stream_name, config, runtime_handler_factory);
        }
    }
    catch(utils::TerminateException&) {
        return 0; // terminate with success
    }
    catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return rv;
}