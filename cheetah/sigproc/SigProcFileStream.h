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
#ifndef SKA_CHEETAH_SIGPROC_SIGPROCFILESTREAM_H
#define SKA_CHEETAH_SIGPROC_SIGPROCFILESTREAM_H

#include "panda/Producer.h"
#include "cheetah/data/TimeFrequency.h"
#include "cheetah/sigproc/SigProcHeader.h"
#include "cheetah/sigproc/Config.h"
#include <fstream>
#include <string>
#include <vector>

namespace ska {
namespace cheetah {
namespace sigproc {
/**
 * @class SigProcFileStream
 * 
 * @brief
 *    Read in a SigProc format input file and generate a series of data chunks
 */

class SigProcFileStream : public ska::panda::Producer<SigProcFileStream, data::TimeFrequency<Cpu, uint8_t>>
{
    public:
        SigProcFileStream(Config const& config);
        ~SigProcFileStream();

        /**
         * @returns true if all data has been consumed
         */
        bool process();

    protected:
        /**
         * @brief return the number of samples (time * channels) in the currently opened file
         * @details actual data samples = this * number of channels.
         */
        std::size_t number_of_data_samples() const;

    private:
        void transfer_header_info(data::TimeFrequency<Cpu, uint8_t>& chunk);
        bool fill_chunk(data::TimeFrequency<Cpu, uint8_t>& chunk, std::size_t offset, data::TimeFrequency<Cpu, uint8_t>::iterator& it );

    private:
        bool next_file();
        std::ifstream _file_stream;

    private:
        Config const& _config;
        SigProcHeader _header;
        utils::ModifiedJulianClock::time_point _start_time;

        mutable std::size_t _nsamples; // calculated number of samples from header info and file size
        std::vector<std::string> _filenames;
        std::vector<std::string>::const_iterator _file_it;
};

} // namespace sigproc
} // namespace cheetah
} // namespace ska

#endif // SKA_CHEETAH_SIGPROC_SIGPROCFILESTREAM_H 
