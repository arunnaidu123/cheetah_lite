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
#ifndef SKA_CHEETAH_PSRDADA_CONFIG_H
#define SKA_CHEETAH_PSRDADA_CONFIG_H

#include "cheetah/utils/Config.h"
#include <panda/ProcessingEngine.h>
#include <panda/ProcessingEngineConfig.h>
#include <sys/types.h> // key_t

namespace ska {
namespace cheetah {
namespace psrdada {

/**
 * @brief
 *   SigProcDadaStream configuration parameters
 *
 */
#ifdef ENABLE_PSRDADA
class Config : public cheetah::utils::Config
{
    public:
        typedef panda::ProcessingEngine Engine;
        Config();
        ~Config();

        /**
         * @brief return key to the dada buffer.
         */
        key_t dada_key() const;

        /**
         * @brief  number of samples to read
         */
        int number_of_samples() const;

        /**
         * @brief set the dada key
         */
        void dada_key(key_t const& key);

        /**
         * @brief set the number of samples required in each block
         */
        void number_of_samples(int const& samples);

        /**
         *  @brief : returns a configured panda::engine to control the thread pool
         */
        Engine& engine() const;

        /**
         *  @ brief: return number of threads in the engine
         */
        unsigned number_of_threads() const;

    protected:
        void add_options(OptionsDescriptionEasyInit& add_options) override;

    private:
        mutable std::unique_ptr<Engine> _engine;
        panda::ProcessingEngineConfig _engine_config;
        key_t _dada_key;
        int _number_of_samples;
};
#else // ENABLE_PSRDADA
// a dummy Config
class Config : public cheetah::utils::Config
{
    public:
        Config(): cheetah::utils::Config("") {}
        ~Config() {};

    protected:
        void add_options(OptionsDescriptionEasyInit&) override {};
};

#endif // ENABLE_PSRDADA


} // namespace psrdada
} // namespace cheetah
} // namespace ska

#endif // SKA_CHEETAH_PSRDADA_CONFIG_H
