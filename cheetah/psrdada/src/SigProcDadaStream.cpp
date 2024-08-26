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
#include "cheetah/psrdada/SigProcDadaStream.h"
#include "cheetah/sigproc/SigProcHeader.h"
#include "cheetah/data/TimeFrequency.h"
#include "cheetah/psrdada/Config.h"
#include "cheetah/utils/ModifiedJulianClock.h"

#include "panda/Error.h"
#include <type_traits>
#include <iostream>
#include <new>
#include <cstdlib>
#include <cmath>

namespace ska {
namespace cheetah {
namespace psrdada {


/**
 * @brief initializing the object
 */
#ifdef ENABLE_PSRDADA
SigProcDadaStream::SigProcDadaStream(Config const& config)
    : _config(config)
    , _engine(config.engine())
    , _client(_config.dada_key()
        , _engine
        , [&](std::istream& in, std::exception_ptr eptr)
          {
              handle_new_sequence(in, eptr);
          })
    , _start_time(std::chrono::milliseconds(0))
    , _error(false)
{
}

void SigProcDadaStream::init()
{
    _client.start();
}

SigProcDadaStream::~SigProcDadaStream()
{
}


void SigProcDadaStream::handle_new_sequence(std::istream& in, std::exception_ptr eptr)
{
    if(eptr)
    {
        PANDA_LOG_ERROR << "Error with reading header\n";
        _error = true;
    }
    else
    {
        std::shared_ptr<bool> stopped = _client.stopped();
        PANDA_LOG_DEBUG << "Reading first header";
        _header.read(in);
        _start_time = _header.start_time();
        _engine.post([this,stopped]() { new_chunk_process(stopped); });
    }
}

void SigProcDadaStream::handle_new_sequence(std::istream& in, std::shared_ptr<ChunkType> current_chunk, ChunkType::Iterator it, std::exception_ptr eptr)
{
    try
    {
        PANDA_LOG_DEBUG << "Reading new header";
        _header.read(in);
        std::shared_ptr<bool> stopped = _client.stopped();
        if( _header.start_time() > _start_time || (unsigned) _header.number_of_channels() != current_chunk->number_of_channels() )
        {
            _start_time = _header.start_time();
            auto start = current_chunk->begin();
            std::size_t new_size = std::distance(start,it)/current_chunk->number_of_channels();
            current_chunk->resize(data::DimensionSize<data::Time>(new_size));
            _engine.post([this, stopped](){ new_chunk_process(stopped); });
        }
        else if( _header.start_time() < _start_time && _start_time - _header.start_time() >= _header.sample_interval())
        {
            throw panda::Error("Incorrect timestamp. Data stream broken!!\n");
            return;
        }
        else
        {
            PANDA_LOG_DEBUG << "Time consistent!";
            _engine.post([&,stopped,current_chunk,it](){do_process(stopped,current_chunk,it);});
        }
    }
    catch (std::exception& e)
    {
        eptr = std::current_exception();
        PANDA_LOG_WARN << "Exception caught: " << e.what();
        _error=true;
    }
    catch (...)
    {
        PANDA_LOG_WARN << "Unknown exception caught";
        _error=true;
    }
}

void SigProcDadaStream::transfer_header_info(ChunkType& chunk)
{
    if (_config.number_of_samples() > 0)
    {
        chunk.resize( data::DimensionSize<data::Time>(_config.number_of_samples()),data::DimensionSize<data::Frequency>(_header.number_of_channels()));
    }
    else
    {
        throw panda::Error("unable to determine number of sigproc samples required");
    }

    chunk.sample_interval(_header.sample_interval());
    if(chunk.channel_frequencies().size() > 1)
    {
        chunk.set_channel_frequencies( _header.frequency_channels().begin()
                                     , _header.frequency_channels().end());
    }
    else
    {
        chunk.set_channel_frequencies_const_width(static_cast<typename data::TimeFrequency<Cpu, uint8_t>::FrequencyType>(*_header.fch1())
                                                 ,static_cast<typename data::TimeFrequency<Cpu, uint8_t>::FrequencyType>(*_header.foff()));
    }
}



bool SigProcDadaStream::process()
{
    if(_error) return _error;

    if(_config.number_of_threads() == 0)
    {
        _engine.poll_one();
    }
    return false;

}


void SigProcDadaStream::new_chunk_process(std::shared_ptr<bool> stopped)
{
    try
    {
        auto current_chunk = get_chunk<ChunkType>(data::DimensionSize<data::Time>(_config.number_of_samples()), data::DimensionSize<data::Frequency>(0));
        assert(current_chunk);
        transfer_header_info(*current_chunk);
        current_chunk->start_time(_start_time);
        auto it = current_chunk->begin();
        do_process(stopped, current_chunk, it);
    }
    catch (std::exception& e)
    {
        PANDA_LOG_ERROR << "DadaStream: exception caught." << e.what();
        _error=true;
    }
    catch (...)
    {
        PANDA_LOG_ERROR << "exception caught!";
        _error=true;
    }
}

bool SigProcDadaStream::do_process( std::shared_ptr<bool> stopped
                                  , std::shared_ptr<ChunkType> current_chunk
                                  , ChunkType::Iterator it)
{
    static constexpr unsigned number_of_bits = sizeof(ChunkType::value_type) * 8;
    try
    {
        typedef data::TimeFrequency<Cpu, uint8_t>::DataType DataType;
        auto begin = it;
        // Raise error if nbits not equal to 8
        if(_header.number_of_bits() != number_of_bits)
        {
            panda::Error e("Expecting ");
            e << number_of_bits << " bits in header. Got " << _header.number_of_bits();
            throw e;
        }

        // Checks for a partial read (due to eod) and resizes the chunk accordingly and stops reading data.
        if(_client.read(it,current_chunk->end()) != current_chunk->end())
        {
            PANDA_LOG_DEBUG << "Partial read of stream, resizing and invoking next_sequence";
            std::size_t elements_read = std::distance(begin,it);
            std::lldiv_t dv{0,0};
            dv = std::div(elements_read,static_cast<long long>(_header.number_of_channels()));
            if (dv.rem !=0)
            {
                int const new_size = dv.quot;
                PANDA_LOG_ERROR << "Partial read not a multiple of TF chunk. Incomplete spectra read";
                throw panda::Error("Incorrect Data. Data have incomplete spectrum");
                current_chunk->resize(data::DimensionSize<data::Time>(new_size - 1),data::DimensionSize<data::Frequency>(_header.number_of_channels()));
                return true;
            }
            _start_time += std::chrono::duration<double>((int)(elements_read/_header.number_of_channels()) * _header.sample_interval().value());
            _client.next_sequence([&,current_chunk, it](std::istream& in, std::exception_ptr eptr) { handle_new_sequence(in, current_chunk, it, eptr); } );
            return false;
        }
        else
        {
            std::size_t elements_read = std::distance(begin,current_chunk->end());
            _start_time += std::chrono::duration<double>((int)(elements_read/_header.number_of_channels()) * _header.sample_interval().value());
            PANDA_LOG_DEBUG << "Successful read of the buffer ";
        }
        _engine.post([this,stopped]() { new_chunk_process(stopped); } );
        return false;
    }
    catch (...)
    {
        PANDA_LOG_DEBUG << "Exception caught!";
        _error = true;
        return false;
    }

}

void SigProcDadaStream::stop()
{
    _client.stop();
}

#else // ENABLE_PSRDADA

SigProcDadaStream::SigProcDadaStream(Config const& config)
    : _config(config)
{
}

SigProcDadaStream::~SigprocDadaStream()
{
}

#endif //ENABLE_PSRDADA

} // namespace psrdada
} // namespace cheetah
} // namespace ska
