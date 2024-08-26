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
#include "cheetah/sigproc/SigProcWriter.h"
#include <iterator>
#include <boost/units/cmath.hpp>

namespace {

    //should work with any iterator
    template<typename IteratorType, typename IteratorTag>
    struct do_write {
        static void exec(IteratorType begin, IteratorType const end, std::ostream& os) {
            while(begin != end) {
                //_file_stream.write(reinterpret_cast<const char*>(&(*it)), sizeof(data::TimeFrequencyDataType));
                os << *begin;
                ++begin;
            }
        }
    };

    // we can speed things up if the memory is contiguous
    template<typename IteratorType>
    struct do_write<IteratorType, std::random_access_iterator_tag>
    {
        static void exec(IteratorType begin, IteratorType const end, std::ostream& os) {
            os.write(reinterpret_cast<const char*>(&(*begin)), std::distance(begin, end) * sizeof(typename std::iterator_traits<typename std::remove_reference<IteratorType>::type>::value_type));
        }
    };

    // calls the correct specialization
    template<typename IteratorType>
    void write_out(IteratorType begin, IteratorType const end, std::ostream& os) {
        do_write<IteratorType, typename std::iterator_traits<typename std::remove_reference<IteratorType>::type>::iterator_category>::exec(begin, end, os);
    }
}

namespace ska {
namespace cheetah {
namespace sigproc {

template<typename HeaderType>
SigProcWriter<HeaderType>::SigProcWriter(WriterConfig const& config)
    : _extension(config.extension())
    , _sample_count(0)
    , _max_samples_per_file(config.max_count()) // approx only
{
    set_dir(config.dir());
}

template<typename HeaderType>
SigProcWriter<HeaderType>::SigProcWriter(std::string const& filename)
    : _extension(".fil")
{
    set_dir(filename);
}

template<typename HeaderType>
SigProcWriter<HeaderType>::SigProcWriter(boost::filesystem::path const& filename)
    : _extension(".fil")
{
    set_dir(filename);
}

template<typename HeaderType>
SigProcWriter<HeaderType>::~SigProcWriter()
{
}

template<typename HeaderType>
void SigProcWriter<HeaderType>::flush()
{
    _file_stream.close();
}

template<typename HeaderType>
void SigProcWriter<HeaderType>::set_dir(boost::filesystem::path const& path)
{
    _dir=path;
    if(!boost::filesystem::is_directory(_dir)) {
        panda::Error e("SigProcWriter: directory does not exist:");
        e << _dir;
        throw e;
    }
}

template<typename HeaderType>
std::string SigProcWriter<HeaderType>::next_file(data::TimeFrequency<Cpu, uint8_t>::TimePointType const& time)
{
    std::time_t ttp = std::chrono::system_clock::to_time_t(time);
    char stem[20];
    std::strftime(stem, sizeof(stem), "%Y_%m_%d_%H:%M:%S", std::gmtime(&ttp));
    boost::filesystem::path file = _dir / boost::filesystem::path(std::string(stem) + _extension);
    if(boost::filesystem::exists(file)) {
        unsigned count = 0;
        do {
            file = _dir / boost::filesystem::path(std::string(stem) + "_" + std::to_string(++count) + _extension);
        } while( boost::filesystem::exists(file));
    }
    return file.native();
}

template<typename HeaderType>
template<typename Arch, typename NumericalRep>
SigProcWriter<HeaderType>& SigProcWriter<HeaderType>::operator<<(data::TimeFrequency<Arch, NumericalRep> const& tf)
{
    HeaderType h;
    fill_header(h, tf);
    file_write(h, static_cast<pss::astrotypes::TimeFrequency<NumericalRep> const&>(tf));
    return *this;
}

template<typename HeaderType>
template<typename DerivedType, typename NumericalRep, typename Alloc>
SigProcWriter<HeaderType>& SigProcWriter<HeaderType>::operator<<(data::SpectrumBase<DerivedType, NumericalRep, Alloc> const& spectrum)
{
    HeaderType h;
    h.number_of_channels(spectrum.template dimension<data::Frequency>());
    h.number_of_bits(sizeof(NumericalRep)*8);
    h.number_of_ifs(1);
    h.tstart(spectrum.start_time());
    file_write(h, spectrum);
    return *this;
}

template<typename HeaderType>
void SigProcWriter<HeaderType>::file_header(HeaderType const& h)
{
    if((h.tstart().is_set() && (_header.sample_interval().value() > 0.0)  && (abs(*h.tstart() - *_header.tstart()) >= 0.5 * _header.sample_interval())) || _header != h )
    {
        if(_file_stream.is_open()) {
            PANDA_LOG_WARN << "break in data stream: creating new file with updated parameters: expected:\n" << pss::astrotypes::sigproc::Header::Info() << _header
                           << " got:\n" << pss::astrotypes::sigproc::Header::Info() << h;
            // if the incomming data is incompatible with existing header info
            // we start a new file
            _file_stream.close();
        }
    }
    if(!_file_stream.is_open()) {
        _header = h;
        _sample_count = 0;
        _file_stream.open(next_file(*(h.tstart())), std::ios::binary);
        _header.write(_file_stream);
    }
}

template<typename HeaderType>
template<typename T>
typename std::enable_if<pss::astrotypes::has_exact_dimensions<T, data::Time, data::Frequency>::value, SigProcWriter<HeaderType>&>::type
SigProcWriter<HeaderType>::file_write(HeaderType const& h, T const& tf, std::size_t spectrum)
{
    file_header(h);

    std::size_t n_spectra = tf.template dimension<data::Time>() - spectrum;
    bool break_file = false;
    if(_max_samples_per_file != 0 ) {
        auto remaining_file_space = _max_samples_per_file - _sample_count;
        if(n_spectra > remaining_file_space) {
            n_spectra = remaining_file_space;
            break_file = true;
        }
        else if( n_spectra == remaining_file_space)
        {
            break_file = true;
        }
    }
    for(std::size_t i=spectrum; i < n_spectra + spectrum; ++i) {
        //auto sample = tf.spectrum(i);
        auto sample = tf[data::DimensionIndex<data::Time>(i)];
        write_out(sample.begin(), sample.end(), _file_stream);
    }

    _header.tstart(*_header.tstart() + (_header.sample_interval() * (double)n_spectra));
    if(break_file) {
        _file_stream.close();
        if(tf.template dimension<data::Time>() > n_spectra) {
            // send the remainder to the next file
            return file_write(_header, tf, n_spectra);
        }
    }
    else {
        _sample_count+=n_spectra;
    }

    return *this;
}

template<typename HeaderType>
template<typename T>
typename std::enable_if<pss::astrotypes::has_exact_dimensions<T, data::Frequency>::value, SigProcWriter<HeaderType>&>::type
SigProcWriter<HeaderType>::file_write(HeaderType const& h, T const& sample)
{
    file_header(h);
    write_out(sample.begin(), sample.end(), _file_stream);
    if(_max_samples_per_file != 0 && ++_sample_count >= _max_samples_per_file) {
        _file_stream.close();
    }
    return *this;
}

template<typename HeaderType>
template<typename Arch, typename NumericalRep>
void SigProcWriter<HeaderType>::fill_header(HeaderType& h, data::TimeFrequency<Arch, NumericalRep> const& tf)
{
    h.number_of_channels(tf.number_of_channels());
    h.number_of_bits(sizeof(NumericalRep)*8);
    h.number_of_ifs(1);
    h.tstart(tf.start_time());
    h.sample_interval(tf.sample_interval());
    // Only equi-spacedfrequency channels supported
    //h.frequency_channels(tf.channel_frequencies());
    if(tf.channel_frequencies().size() != 0) {
        h.fch1(tf.channel_frequencies()[0]);
        if(tf.channel_frequencies().size() > 1) {
            h.foff(tf.channel_frequencies()[1] - tf.channel_frequencies()[0]);
        }
    }
}

template<typename HeaderType>
template<typename T>
typename std::enable_if<pss::astrotypes::has_exact_dimensions<T, data::Time, data::Frequency>::value, SigProcWriter<HeaderType>&>::type
SigProcWriter<HeaderType>::write(HeaderType const& header, T const& t)
{
    return file_write(header, t);
}

} // namespace sigproc
} // namespace cheetah
} // namespace ska
