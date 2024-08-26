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
#ifndef SKA_CHEETAH_PIPELINES_SEARCH_PIPELINE_CONFIG_H
#define SKA_CHEETAH_PIPELINES_SEARCH_PIPELINE_CONFIG_H

#include "cheetah/utils/Config.h"
#include "cheetah/io/exporters/DataExportConfig.h"
#include "cheetah/pipelines/search_pipeline/DataExport.h"
#include <memory>

namespace ska {
namespace cheetah {
namespace pipelines {
namespace search_pipeline {

/**
 * @brief
 *    Data Input Configuration Parameters
 */

template<typename NumericalRep>
class DataConfig : public io::exporters::DataExportConfig
{
        typedef io::exporters::DataExportConfig BaseT;

    public:
        DataConfig();
        ~DataConfig();

        /**
         * @brief return the data exporter module
         * @details result not valid until after parse() has been called
         */
        DataExport<NumericalRep>& data_exporter() const;

    protected:
        void add_options(OptionsDescriptionEasyInit& add_options) override;
        /**
         * @brief print to std::cout the export streamer types available
         */
        void list_export_types(bool) const;

    private:
        mutable std::unique_ptr<DataExport<NumericalRep>> _out;

};

} // namespace search_pipeline
} // namespace pipelines
} // namespace cheetah
} // namespace ska
#include "detail/DataConfig.cpp"

#endif // SKA_CHEETAH_PIPELINES_SEARCH_PIPELINE_CONFIG_H
