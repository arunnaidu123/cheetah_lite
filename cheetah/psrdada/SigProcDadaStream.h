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
#ifndef SKA_CHEETAH_PSRDADA_SIGPROCDADASTREAM_H
#define SKA_CHEETAH_PSRDADA_SIGPROCDADASTREAM_H

#ifdef ENABLE_PSRDADA
#include "panda/Producer.h"
#include "cheetah/data/TimeFrequency.h"
#include "cheetah/sigproc/SigProcHeader.h"
#include "cheetah/psrdada/DadaReadClient.h"
#include "cheetah/psrdada/Config.h"

namespace ska {
namespace cheetah {
namespace psrdada {
/**
 * @class SigProcDadaStream
 *
 * @detail
 *    Read in a DADA buffer in SigProc format and process a series of data chunks.
 *    Opens a DadaReadClient object to read the DADA header (assumes sigproc header format) and then streams data in open DADA data blocks as Timefrequency chunks.
 *    Also checks whether the data are bad or there are incomplete spectra.
 *    Continues to read in data until there are no more data blocks to read or the stream detects an eod marker.
 */

class SigProcDadaStream : public ska::panda::Producer<SigProcDadaStream, data::TimeFrequency<Cpu, uint8_t>>
{
    private:
        typedef data::TimeFrequency<Cpu, uint8_t> ChunkType;

    public:
        SigProcDadaStream(Config const& config);
        ~SigProcDadaStream();

        /**
         * @returns true if all data has been consumed
         */
        bool process();

        /**
         * @brief: stop the stream
         */
        void stop();
       
        /**
         * @brief:  Start the stream
         */  
        void init();

    private:
        /**
          * @brief transfer header info from the DADA header (assuming SigProc format)
          */
        void transfer_header_info(ChunkType& chunk);
        void handle_new_sequence(std::istream& in,std::exception_ptr eptr);
        void handle_new_sequence(std::istream& in,std::shared_ptr<ChunkType> chunk, ChunkType::Iterator it, std::exception_ptr eptr);
        void new_chunk_process(std::shared_ptr<bool> stopped);
        bool do_process(std::shared_ptr<bool> stopped, std::shared_ptr<ChunkType> chunk,ChunkType::Iterator it);

    private:
        Config const& _config;
        panda::Engine& _engine;
        sigproc::SigProcHeader _header;
        DadaReadClient _client;
        utils::ModifiedJulianClock::time_point _start_time;
        bool _error;
};

} // namespace psrdada
} // namespace cheetah
} // namespace ska

#endif //ENABLE_PSRDADA

#endif // SKA_CHEETAH_PSTDADA_SIGPROCDADASTREAM_H
