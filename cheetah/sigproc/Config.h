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
#ifndef SKA_CHEETAH_SIGPROC_CONFIG_H
#define SKA_CHEETAH_SIGPROC_CONFIG_H


#include "cheetah/utils/Config.h"

namespace ska {
namespace cheetah {
namespace sigproc {

/**
 * @brief
 *   SigProc configuration parameters
 * 
 */
class Config : public cheetah::utils::Config
{
    public:
        Config();
        ~Config();

        /**
         * @brief return the sigproc files to be processed as input data
         */
        std::vector<std::string> const& sigproc_files() const;

        /**
         * @brief return the sigproc files to be processed as input data
         */
        void set_sigproc_files(std::vector<std::string> const& sigproc_files);
        void set_sigproc_files(std::string sigproc_file);

        /**
         * @brief return the default nbits to be used when not specified in sigproc file headers
         */
        int nbits() const;

        /**
         * @brief return the number of time samples in a chunk
         */
        unsigned chunk_samples() const;

        /**
         * @brief setthe number of time samples in a chunk
         */
        void set_chunk_samples(unsigned samples);

    protected:
        void add_options(OptionsDescriptionEasyInit& add_options) override;

    private:
        std::vector<std::string> _sigproc_files;
        int _nbits;
        unsigned _chunk_samples;
};

} // namespace sigproc
} // namespace cheetah
} // namespace ska

#endif // SKA_CHEETAH_SIGPROC_CONFIG_H 
