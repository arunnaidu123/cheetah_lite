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
#ifndef SKA_CHEETAH_IO_EXPORTERS_SPCCLSIGPROCCONFIG_H
#define SKA_CHEETAH_IO_EXPORTERS_SPCCLSIGPROCCONFIG_H


#include "cheetah/sigproc/WriterConfig.h"
#include "cheetah/data/CandidateWindowConfig.h"

namespace ska {
namespace cheetah {
namespace io {
namespace exporters {

/**
 * @brief A configuration class for the dynamic spectrum of single pulse candidates
 * @details
 */

class SpCclSigProcConfig : public sigproc::WriterConfig
{
    typedef sigproc::WriterConfig BaseT;

    public:
        SpCclSigProcConfig();

        ~SpCclSigProcConfig();

        /**
         * @brief: Getter for window config
         */
        data::CandidateWindowConfig const& window_config() const;

        /**
         * @brief: Setter for the candidate window config
         */
        void window_config(data::CandidateWindowConfig& window_config);

    protected:
        void add_options(OptionsDescriptionEasyInit& add_options) override;


    private:
        data::CandidateWindowConfig _window_config;
};


} // namespace exporters
} // namespace io
} // namespace cheetah
} // namespace ska

#endif // SKA_CHEETAH_IO_EXPORTERS_SPCCLSIGPROCCONFIG_H
