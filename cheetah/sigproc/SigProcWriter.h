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
#ifndef SKA_CHEETAH_SIGPROC_SIGPROCWRITER_H
#define SKA_CHEETAH_SIGPROC_SIGPROCWRITER_H

#include "pss/astrotypes/sigproc/Header.h"
#include "cheetah/sigproc/WriterConfig.h"
#include "cheetah/data/TimeFrequency.h"
#include "cheetah/data/Spectrum.h"
#include <boost/filesystem.hpp>
#include <fstream>

namespace ska {
namespace cheetah {
namespace sigproc {

/**
 * @brief
 *    Writes data types to a sigproc format file
 * @details
 *    Will automatically create files in the provided directory with timestamps as the filename
 */
template<typename HeaderType=pss::astrotypes::sigproc::Header>
class SigProcWriter
{
    public:
        SigProcWriter(WriterConfig const&);
        SigProcWriter(std::string const& directory);
        SigProcWriter(boost::filesystem::path const& directory);
        virtual ~SigProcWriter();

        /**
         * @brief writes out the samples in the data to the currently open sigproc file
         * @details if the sigproc file is not open, or the data parameters do not match
         * that of the currently open sigproc file, then a new sigproc file will be created
         * with a suitable header.
         */
        template<typename Arch, typename NumericalRep>
        SigProcWriter& operator<<(data::TimeFrequency<Arch, NumericalRep> const& tf);

        template<typename DerivedType, typename NumericalRep, typename Alloc>
        SigProcWriter& operator<<(data::SpectrumBase<DerivedType, NumericalRep, Alloc> const& spectrum_data);

        /**
         * @brief close any existing data files
         */
        void flush();

        /**
         * @brief fill the sigproc header object with meta data taken from the TimeFrequency object
         */
        template<typename Arch, typename NumericalRep>
        static void fill_header(HeaderType& header, data::TimeFrequency<Arch, NumericalRep> const& tf);

        /**
         * @brief write the data out to file using the meta data from the header provided
         */
        template<typename T>
        typename std::enable_if<pss::astrotypes::has_exact_dimensions<T, data::Time, data::Frequency>::value, SigProcWriter&>::type
        write(HeaderType const&, T const& t);


    private:
        void set_dir(boost::filesystem::path const& path);
        std::string next_file(data::TimeFrequency<Cpu, uint8_t>::TimePointType const&);

        void file_header(HeaderType const&);

        template<typename T>
        typename std::enable_if<pss::astrotypes::has_exact_dimensions<T, data::Time, data::Frequency>::value, SigProcWriter&>::type
        file_write(HeaderType const&, T const& t, std::size_t spectrum=0);

        template<typename T>
        typename std::enable_if<pss::astrotypes::has_exact_dimensions<T, data::Frequency>::value, SigProcWriter&>::type
        file_write(HeaderType const&, T const& t);

    private:
        boost::filesystem::path _dir;
        HeaderType _header;
        std::ofstream _file_stream;
        std::string _extension;
        std::size_t _sample_count;
        std::size_t _max_samples_per_file;
};

} // namespace sigproc
} // namespace cheetah
} // namespace ska
#include "cheetah/sigproc/detail/SigProcWriter.cpp"

#endif // SKA_CHEETAH_SIGPROC_SIGPROCWRITER_H
