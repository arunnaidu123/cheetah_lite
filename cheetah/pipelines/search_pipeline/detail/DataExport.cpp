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
#include "cheetah/data/TimeFrequency.h"
#include "cheetah/pipelines/search_pipeline/DataExport.h"
#include "cheetah/sigproc/SigProcWriter.h"
#include "cheetah/io/exporters/OcldFileStreamer.h"
#include "cheetah/io/exporters/SclFileStreamer.h"
#include "cheetah/io/exporters/SpCandidateDataStreamer.h"
#include "cheetah/io/exporters/SclCandidateDataStreamer.h"
#include "cheetah/io/exporters/SpCclFileStreamer.h"
#include "cheetah/io/exporters/SpCclSpeadStreamer.h"
#include "cheetah/io/exporters/SpCclSigProc.h"
#include <panda/Log.h>
#include <map>
#include <memory>
#include <iostream>
#include <functional>
#include <utility>

namespace ska {
namespace cheetah {
namespace pipelines {
namespace search_pipeline {


template<typename T>
struct SimpleStreamer {
    void operator()(T const&) {
        PANDA_LOG << "streamer called";// << t;
    }
};

/*
 * Use a shared_ptr pimpl idiom for Writers that have a deleted copy/move constructor
 */
template<typename WriterType, typename T>
struct DataExportStreamWrapper {
    public:
        // pass through constructor - only enabled if the first Argument does not match typical signatures for the normal
        // constructors of DataExportStreamWrapper
        template<typename Arg1>
        DataExportStreamWrapper(Arg1&& arg1, typename std::enable_if<!std::is_same<typename std::decay<Arg1>::type, DataExportStreamWrapper>::value, int >::type* = nullptr)
            : _writer(new WriterType(std::forward<Arg1>(arg1))) {}

        DataExportStreamWrapper(DataExportStreamWrapper const&) = default;
        DataExportStreamWrapper(DataExportStreamWrapper&& s) : _writer(std::move(s._writer)) {}

        inline
        void operator()(T const& t) const {
            *_writer << t;
        }

    private:
        std::shared_ptr<WriterType> _writer;
};

template<typename T, typename HeaderType>
struct SigProcStreamer {
    public:
        SigProcStreamer(sigproc::WriterConfig const& config)
            : _writer(new sigproc::SigProcWriter<HeaderType>(config)) {}

        SigProcStreamer(SigProcStreamer const&) = default;
        SigProcStreamer(SigProcStreamer&& s) : _writer(std::move(s._writer)) {}

        inline
        void operator()(T const& t) const {
            *_writer << t;
        }

    private:
        std::shared_ptr<sigproc::SigProcWriter<HeaderType>> _writer;
};

template<typename T>
struct SpsCandidateStreamer : public DataExportStreamWrapper<io::exporters::SpCclFileStreamer<typename T::TimeFrequencyType::DataType>, T>
{
    typedef DataExportStreamWrapper<io::exporters::SpCclFileStreamer<typename T::TimeFrequencyType::DataType>, T> BaseT;

    public:
        SpsCandidateStreamer(io::exporters::SpCclFileStreamerConfig const& config)
            : BaseT(config)
        {
        }
};

template<typename T>
struct SpsCandidateDataStreamer : public DataExportStreamWrapper<io::exporters::SpCandidateDataStreamer<typename T::TimeFrequencyType>, T>
{
    typedef DataExportStreamWrapper<io::exporters::SpCandidateDataStreamer<typename T::TimeFrequencyType>, T> BaseT;

    public:
        SpsCandidateDataStreamer(io::exporters::SpCandidateDataStreamerConfig const& config)
            : BaseT(config)
        {
        }
};

#ifdef ENABLE_SPEAD
template<typename T>
struct SpsCandidateSpeadStreamer
{
        typedef io::exporters::SpCclSpeadStreamer WriterType;

    public:
        SpsCandidateSpeadStreamer(io::exporters::SpCclSpeadStreamerConfig const& config, panda::Engine& engine)
            : _writer(new WriterType(config, engine))
        {
        }

        inline
        void operator()(T const& t) const {
            *_writer << t;
        }


    private:
        std::shared_ptr<WriterType> _writer;
};
#endif  //ENABLE_SPEAD

template<typename T>
struct SclCandidateDataStreamer : public DataExportStreamWrapper<io::exporters::SclCandidateDataStreamer, T>
{
    typedef DataExportStreamWrapper<io::exporters::SclCandidateDataStreamer, T> BaseT;

    public:
        SclCandidateDataStreamer(io::exporters::SclCandidateDataStreamerConfig const& config)
            : BaseT(config)
        {
        }
};

template<typename T>
struct SiftedCandidateStreamer : public DataExportStreamWrapper<io::exporters::SclFileStreamer, T>
{
    typedef DataExportStreamWrapper<io::exporters::SclFileStreamer, T> BaseT;

    SiftedCandidateStreamer(io::exporters::SclFileStreamerConfig const& config)
        : BaseT(config)
    {
    }
};

template<typename T>
struct SpCclSigProcStreamer : public DataExportStreamWrapper<io::exporters::SpCclSigProc<typename T::TimeFrequencyType::DataType>, T>
{
    typedef DataExportStreamWrapper<io::exporters::SpCclSigProc<typename T::TimeFrequencyType::DataType>, T> BaseT;

    SpCclSigProcStreamer(io::exporters::SpCclSigProcConfig const& config)
        : BaseT(config)
    {
    }
};

template<typename NumRep, typename T>
struct OptimisedCandidateStreamer : public DataExportStreamWrapper<io::exporters::OcldFileStreamer<NumRep>, T>
{
    typedef DataExportStreamWrapper<io::exporters::OcldFileStreamer<NumRep>, T> BaseT;

    OptimisedCandidateStreamer(io::exporters::OcldFileStreamerConfig const& config)
        : BaseT(config)
    {
    }
};

template<typename NumRep, typename ExportTraits>
DataExport<NumRep, ExportTraits>::DataExport(io::exporters::DataExportConfig const& config)
    : BaseT(config)
{
    // n.b for each factory that takes a configuration add the configuration to pipeline/DataConfig.cpp
    typedef data::TimeFrequency<Cpu, NumRep> TimeFrequencyType;
    this->template set_factory<TimeFrequencyType>(io::exporters::ExporterType("cout"),
        [](io::exporters::DataExportStreamConfig const&) { return SimpleStreamer<data::TimeFrequency<Cpu, NumRep>>(); } );
    this->template set_factory<data::TimeFrequency<Cpu, NumRep>>(io::exporters::ExporterType("sigproc"), [](io::exporters::DataExportStreamConfig const& c)
        {
           return SigProcStreamer<TimeFrequencyType, typename ExportTraits::SigProcHeader>(static_cast<sigproc::WriterConfig const&>(c.sink_config()));
        }
    );
    this->template set_factory<modules::rfim::ampp::Spectrum<NumRep>>(io::exporters::ExporterType("sigproc"), [](io::exporters::DataExportStreamConfig const& c)
        {
           return SigProcStreamer<modules::rfim::ampp::Spectrum<NumRep>, typename ExportTraits::SigProcHeader>(static_cast<sigproc::WriterConfig const&>(c.sink_config()));
        }
    );
    this->template set_factory<data::SpCcl<NumRep>>(io::exporters::ExporterType("spccl_files"),
        [](io::exporters::DataExportStreamConfig const& c)
        {
            return SpsCandidateStreamer<data::SpCcl<NumRep>>(static_cast<io::exporters::SpCclFileStreamerConfig const&>(c.sink_config()));
        }
    );
#ifdef ENABLE_SPEAD
    this->template set_factory<data::SpCcl<NumRep>>(io::exporters::ExporterType("spccl_spead"),
        [](io::exporters::DataExportStreamConfig const& c)
        {
            return SpsCandidateSpeadStreamer<data::SpCcl<NumRep>>(static_cast<io::exporters::SpCclSpeadStreamerConfig const&>(c.sink_config()), c.engine());
        }
    );
#endif // ENABLE_SPEAD
    this->template set_factory<data::Scl>(io::exporters::ExporterType("scl_candidate_data"),
        [](io::exporters::DataExportStreamConfig const& c)
        {
            return SclCandidateDataStreamer<data::Scl>(static_cast<io::exporters::SclCandidateDataStreamerConfig const&>(c.sink_config()));
        }
    );
    this->template set_factory<data::SpCandidateData<TimeFrequencyType>>(io::exporters::ExporterType("sp_candidate_data"),
        [](io::exporters::DataExportStreamConfig const& c)
        {
            return SpsCandidateDataStreamer<data::SpCandidateData<TimeFrequencyType>>(static_cast<io::exporters::SpCandidateDataStreamerConfig const&>(c.sink_config()));
        }
    );
    this->template set_factory<data::Ocld<NumRep>>(io::exporters::ExporterType("ocld_files"),
        [](io::exporters::DataExportStreamConfig const& c)
        {
            return OptimisedCandidateStreamer<NumRep, data::Ocld<NumRep>>(static_cast<io::exporters::OcldFileStreamerConfig const&>(c.sink_config()));
        }
    );
    this->template set_factory<data::Scl>(io::exporters::ExporterType("scl_files"),
        [](io::exporters::DataExportStreamConfig const& c)
        {
            return SiftedCandidateStreamer<data::Scl>(static_cast<io::exporters::SclFileStreamerConfig const&>(c.sink_config()));
        }
    );
    this->template set_factory<data::SpCcl<NumRep>>(io::exporters::ExporterType("spccl_sigproc_files"),
        [](io::exporters::DataExportStreamConfig const& c)
        {
            return SpCclSigProcStreamer<data::SpCcl<NumRep>>(static_cast<io::exporters::SpCclSigProcConfig const&>(c.sink_config()));
        }
    );

    this->init();
}

template<typename NumRep, typename ExportTraits>
DataExport<NumRep, ExportTraits>::~DataExport()
{
}

} // namespace search_pipeline
} // namespace pipelines
} // namespace cheetah
} // namespace ska
