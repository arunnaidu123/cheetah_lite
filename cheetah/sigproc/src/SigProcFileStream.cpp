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
#include "cheetah/sigproc/SigProcFileStream.h"
#include "cheetah/data/TimeFrequency.h"

#include "panda/Error.h"
#include <boost/filesystem.hpp>
#include <type_traits>
#include <iostream>
#include <new>


namespace ska {
namespace cheetah {
namespace sigproc {

#define HI4BITS   240
#define LO4BITS   15
void char2ints (unsigned char c, int *i, int *j)
{
  *i =  c & LO4BITS;
  *j = (c & HI4BITS) >> 4;
}

void char4ints (unsigned char c, int *i, int *j, int *k, int *l)
{
  *i =  c & 3;
  *j = (c & 12) >> 2;
  *k = (c & 48) >> 4;
  *l = (c & 192) >> 6;
}

// @returns the number of samples read
static
std::size_t read_block(std::istream& input, int nbits, float *block, unsigned long nread)
{
    int j, k, s1, s2, s3, s4;
    std::size_t i;
    short *shortblock=nullptr;
    char *charblock=nullptr;
    std::size_t iread;

    /* decide how to read the data based on the number of bits per sample */
    try{
    switch(nbits) {

        case 1:
            // read n/8 bytes into character block containing n 1-bit pairs
            charblock = (char *) malloc(nread / 8);
            try {
                input.read(charblock, nread / 8);
            } catch (...)
            {
                if(!input.eof()) {
                    throw;
                }
            }

            k = 0;

            // unpack 1-bit pairs into the datablock
            iread = input.gcount();
            for (std::size_t i = 0; i < iread; ++i)
                for (j = 0; j < 8; ++j) {
                    block[k++] = charblock[i] & 1;
                    charblock[i] >>= 1;
                }

            iread = k; //number of samples read in
            free(charblock);
            break;

        case 2: // NOTE: Handles Parkes Hitrun survey data
            // read n/4 bytes into character block containing n 2-bit pairs
            charblock = (char *) malloc(nread / 4);
            try {
                input.read(charblock, nread / 4);
            } catch (...)
            {
                if(!input.eof()) {
                    throw;
                }
            }

            j = 0;

            iread = input.gcount();
            for(i = 0; i < iread; i++) {
                char4ints(charblock[i], &s1, &s2, &s3, &s4);
                block[j++] = (float) s1;
                block[j++] = (float) s2;
                block[j++] = (float) s3;
                block[j++] = (float) s4;
            }

            iread *= 4;
            free(charblock);
            break;

        case 4:
            // read n/2 bytes into character block containing n 4-bit pairs
            charblock = (char *) malloc(nread / 2);
            try {
                input.read(charblock, nread / 2);
            } catch (...)
            {
                if(!input.eof()) {
                    throw;
                }
            }

            j = 0;

            /* unpack 4-bit pairs into the datablock */
            iread = input.gcount();
            for (i = 0; i < iread; i++) {
                char2ints(charblock[i], &s1, &s2);
                block[j++] = (float) s1;
                block[j++] = (float) s2;
            }

            iread *= 2; /* this is the number of samples read in */
            free(charblock);
            break;

        case 8:
            /* read n bytes into character block containing n 1-byte numbers */
            charblock = (char *) malloc(nread);
            try {
                input.read(charblock, nread);
            } catch (...)
            {
                if(!input.eof()) {
                    throw;
                }
            }
            /* copy numbers into datablock */
            iread = input.gcount();
            for (i = 0; i < iread; i++)
                block[i] = (float) charblock[i];

            free(charblock);
            break;

        case 16:
	    //NOTE: should condsider raising warning if this is used, as
	    //the data::TimeFrequencyDataType is uint_8.
            /* read 2*n bytes into short block containing n 2-byte numbers */
            shortblock = (short *) malloc(2 * nread);
            try {
                input.read(reinterpret_cast<char*>(shortblock), 2 * nread);
            } catch (...)
            {
                if(!input.eof()) {
                    throw;
                }
            }

            /* copy numbers into datablock */
            iread = input.gcount() / 2;
            for (i = 0; i < iread; i++) {
                block[i] = (float) shortblock[i];
            }

            free(shortblock);
            break;

        case 32:
            /* read 4*n bytes into floating block containing n 4-byte numbers */
            try {
                input.read(reinterpret_cast<char*>(block), 4 * nread);
            } catch (...)
            {
                if(!input.eof()) {
                    throw;
                }
            }

            iread = input.gcount() / 4;
            break;

        default:
            throw panda::Error("SigProcStream : read_block - nbits can only be 4, 8, 16 or 32!");
            iread = 0;
    }
    } catch (...) {
        if(shortblock) free(shortblock);
        if(charblock) free(charblock);
        iread = input.gcount() * 8/nbits;
        if(!input.eof()) {
            throw;
        }
    }

    /* return number of samples read */
    return iread;
}

SigProcFileStream::SigProcFileStream(Config const& config)
    : _config(config)
    , _nsamples(0)
    , _filenames(config.sigproc_files())
    , _file_it(_filenames.cbegin())
{
    _header.number_of_bits(config.nbits());
}

SigProcFileStream::~SigProcFileStream()
{
}

void SigProcFileStream::transfer_header_info(data::TimeFrequency<Cpu, uint8_t>& chunk)
{
    if( chunk.number_of_spectra() != 0 ){
        // assume the chunk wants a set number of samples
        chunk.resize( data::DimensionSize<data::Time>(chunk.number_of_spectra())
                    , data::DimensionSize<data::Frequency>(_header.number_of_channels()));
    }
    else if(number_of_data_samples() > 0 ) {
        // we take as many samples os defined in the header
        chunk.resize( data::DimensionSize<data::Time>(number_of_data_samples()/ _header.number_of_channels())
                    , data::DimensionSize<data::Frequency>(_header.number_of_channels()));
    }
    else {
        // make a guess at how many samples to take
        if( _header.number_of_bits() > 0 ) {
            chunk.resize(
                    data::DimensionSize<data::Time>(sizeof(float)*8/_header.number_of_bits()),
                    data::DimensionSize<data::Frequency>(_header.number_of_channels()));
        }
        else {
            throw panda::Error("unable to determine number of sigproc samples required");
        }
    }
    chunk.sample_interval(_header.sample_interval());
    if(chunk.channel_frequencies().size() > 1) {
        chunk.set_channel_frequencies(_header.frequency_channels().begin(), _header.frequency_channels().end());
    }
    else {
        chunk.set_channel_frequencies_const_width(static_cast<typename data::TimeFrequency<Cpu, uint8_t>::FrequencyType>(*_header.fch1())
                                                 ,static_cast<typename data::TimeFrequency<Cpu, uint8_t>::FrequencyType>(*_header.foff()));
    }
}

std::size_t SigProcFileStream::number_of_data_samples() const
{
    if( _nsamples == 0) {
        _nsamples = (boost::filesystem::file_size(*(_file_it -1)) - _header.size() ) * (8/_header.number_of_bits());
    }
    return _nsamples;
}

bool SigProcFileStream::process()
{
    if(!_file_stream.is_open() || _file_stream.eof() || _file_stream.peek() == decltype(_file_stream)::traits_type::eof()) {
        if(!next_file()) return true; // mark end of data
    }

    typedef data::TimeFrequency<Cpu, uint8_t> TimeFrequencyType;
    //typedef std::decay<decltype(std::declval<TimeFrequencyType>().data())>::type TimeFrequencyDataType;

    auto chunk = get_chunk<TimeFrequencyType>(data::DimensionSize<data::Time>(_config.chunk_samples()), data::DimensionSize<data::Frequency>(0));
    assert(chunk);

    transfer_header_info(*chunk);
    chunk->start_time(_start_time);
    auto it = chunk->begin();
    return fill_chunk(*chunk, 0, it);
}

bool SigProcFileStream::fill_chunk(data::TimeFrequency<Cpu, uint8_t>& chunk, std::size_t offset, data::TimeFrequency<Cpu, uint8_t>::Iterator& it )
{
    std::size_t data_to_read = chunk.number_of_spectra() * chunk.number_of_channels() - offset;
    // of the read_block function. This is not ideal, but fine for the moment.

    typedef data::TimeFrequency<Cpu, uint8_t>::DataType DataType;
    std::size_t data_read;
    {
        // TODO - read in directly to the iterator instead of expanding to floats and back to the type via read_block
        float* tmp = (float*)malloc(data_to_read * sizeof(float));
        if(!tmp) throw std::bad_alloc();

        try {
            data_read = read_block(_file_stream, _header.number_of_bits(), tmp, data_to_read);
        }
        catch(...) {
            free(tmp);
            throw;
        }
        assert(data_read <= data_to_read);

        // Convert float values back to data::TimeFrequencyDataType
        // Note: This will corrupt data from 16 and 32 bit filterbank
        // files. We should consider disabling the 16 and 32 bit switch
        // cases in read_block so that this does not happen.
        std::copy(tmp,tmp+data_read,it);
        free(tmp);
    }

    // This branch handles the case that we do not get a full chunks
    // worth of data from the read (e.g. when we hit EOF)
    std::size_t valid_samples = data_read / chunk.number_of_channels(); //implicit floor.
    _start_time += std::chrono::duration<double>((double)valid_samples * _header.sample_interval().value());
    if(data_read != data_to_read)
    {
        if( _file_stream.eof()) {
            auto expected_header = _header;
            // try to get missing chunks from the next file
            bool rv=next_file();
            if(!rv || expected_header.tstart() != _start_time || expected_header != _header) {
                // Calculate the number of valid time samples (i.e. full spectra) and
                // resize the chuck appropriately.
                PANDA_LOG << "resizing to " << valid_samples;
                chunk.resize( data::DimensionSize<data::Time>(valid_samples) );
                return !rv;
            }
            else {
                fill_chunk(chunk, offset + data_read, it);
            }
        }
    }

    // Indicate that stream is still producing
    return false;
}

bool SigProcFileStream::next_file()
{
    _nsamples = 0U;

    if(_file_it == _filenames.cend()) return false;

    // open file for input
    _file_stream.exceptions();
    _file_stream.clear();
    _file_stream.open(*_file_it, std::ios::binary);

    if(!_file_stream.is_open()) {
        std::cerr << "could not open file '" << *_file_it << "'" << std::endl; // TODO use logging framework
        return false;
    }
    _file_stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    // parse the header
    try {
        // try catch block to append filename to any message
        try {
            // process header
            _header.read(_file_stream);
        } catch(std::exception& e) {
            panda::Error err(e);
            err << " in file " << *_file_it;
            throw err;
        }
    }
    catch(...) {
        // recover from a bad parse, moving on to the next file
        ++_file_it;
        _file_stream.close();
        throw;
    }
    _start_time = *_header.tstart();

    // increment file iterator for next call
    ++_file_it;

    return true;
}


} // namespace sigproc
} // namespace cheetah
} // namespace ska
