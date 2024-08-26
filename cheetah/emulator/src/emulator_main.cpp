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
 * @section app_emulator UDP packet emulation
 * @brief cheetah_emulator A stand alone UDP packet stream generator
 * @details
 *    Will generate a continuous stream of generated data (many options available)
 *    that are streamed over UDP.
 *
 *    for help on how to use see:
 *    @verbatim
       cheetah_emulator --help
      @endverbatim
 *
 * @} */ // end group

#include "cheetah/io/producers/rcpt/SkaPacketStreamFactory.h"
#include "cheetah/emulator/EmulatorAppConfig.h"
#include "panda/Log.h"
#include <ostream>

int main(int argc, char** argv)
{
    ska::cheetah::io::producers::rcpt::SkaPacketStreamFactory ska_stream_factory;
    typedef ska::cheetah::emulator::EmulatorAppConfig EmulatorAppConfig;
    EmulatorAppConfig app_config("cheetah_emulator"
                                , "UDP emulation for cheetah Beamformed Time-Frequency data"
                                , ska_stream_factory
                                );
    int rv;
    try {
        // parse command line options
        if( (rv=app_config.parse(argc, argv)) ) return rv;
        ska::cheetah::emulator::Config& config = app_config.emulator_config();
        auto stream_type=app_config.stream_type();
        rv = ska_stream_factory.exec(stream_type, config);

        return rv;
    }
    catch(std::exception const& e) {
        std::cerr << "Emulator: " << e.what() << std::endl;;
    }
    catch(...) {
        std::cerr << "Emulator: unknown exception caught" << std::endl;
    }
    return 1;
}
