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
 * @section app_generator
 * @brief cheetah_generator
 * @details
 *    Will generate a continuous stream of generated data (many options available)
 *    that are streamed to file.
 *
 *    for help on how to use see:
 *    @verbatim
       cheetah_generator --help
      @endverbatim
 *
 * @} */ // end group

#include "cheetah/generators/GeneratorAppConfig.h"
#include "cheetah/generators/GeneratorApp.h"
#include "cheetah/generators/GeneratorFactory.h"
#include "cheetah/data/TimeFrequency.h"
#include "cheetah/utils/Architectures.h"
#include "panda/Log.h"
#include <memory>

int main(int argc, char** argv) {

    ska::cheetah::generators::GeneratorAppConfig app_config("cheetah_generator", "genrate cheetah Beamformed Time-Frequecy data to a file");
    int rv;
    try {
        ska::cheetah::generators::GeneratorFactory<ska::cheetah::data::TimeFrequency<ska::cheetah::Cpu, uint8_t>> generator_factory(app_config.generator_config());
        app_config.set_generator_list(generator_factory.available());

        // parse command line options
        if( (rv=app_config.parse(argc, argv)) ) return rv;

        // setup the app
        ska::cheetah::generators::GeneratorApp<ska::cheetah::data::TimeFrequency<ska::cheetah::Cpu, uint8_t>> app(app_config, generator_factory);
        app.exec();
    }
    catch(std::exception const& e) {
        std::cerr << "Emulator: " << e.what() << std::endl;;
    }
    catch(...) {
        std::cerr << "Emulator: unknown exception caught" << std::endl;
    }
    return 1;
}
