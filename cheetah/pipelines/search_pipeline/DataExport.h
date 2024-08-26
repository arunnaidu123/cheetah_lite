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
#ifndef SKA_CHEETAH_PIPELINES_SEARCH_PIPELINE_DATAEXPORT_H
#define SKA_CHEETAH_PIPELINES_SEARCH_PIPELINE_DATAEXPORT_H


#include "cheetah/io/exporters/DataExport.h"
#include "cheetah/io/exporters/DataExportConfig.h"
#include "cheetah/data/TimeFrequency.h"
#include "cheetah/data/SpCcl.h"
#include "cheetah/data/SpCandidateData.h"
#include "cheetah/data/Scl.h"
#include "cheetah/data/Ocld.h"
#include "cheetah/modules/rfim/ampp/Spectrum.h"
#include "pss/astrotypes/sigproc/Header.h"

namespace ska {
namespace cheetah {
namespace pipelines {
namespace search_pipeline {

/**
 * @brief
 *   A panda DataSwitch configured with all the known output streams in this project
 *
 * @details
 *   To add a new streamer, add a set_factory() call to the initialiser
 *   To add a new data type, add the type to the CheetahDataTypes define
 */

#define CheetahDataTypes const data::TimeFrequency<Cpu, NumRep>, const data::SpCcl<NumRep>, const data::SpCandidateData<data::TimeFrequency<Cpu, NumRep>>, const data::Ocld<NumRep>, const data::Scl, const modules::rfim::ampp::Spectrum<NumRep>

struct DefaultExportTraits
{
    typedef pss::astrotypes::sigproc::Header SigProcHeader;
};

template<typename NumRep, typename ExportTraits=DefaultExportTraits>
class DataExport : public io::exporters::DataExport<CheetahDataTypes>
{
        typedef io::exporters::DataExport<CheetahDataTypes> BaseT;

    public:
        DataExport(io::exporters::DataExportConfig const&);
        virtual ~DataExport();

};

} // namespace search_pipeline
} // namespace pipelines
} // namespace cheetah
} // namespace ska
#include "detail/DataExport.cpp"

#endif // SKA_CHEETAH_PIPELINES_SEARCH_PIPELINE_DATAEXPORT_H
