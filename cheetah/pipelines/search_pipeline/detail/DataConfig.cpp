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
#include "cheetah/pipelines/search_pipeline/DataConfig.h"
#include "cheetah/pipelines/search_pipeline/DataExport.h"
#include "panda/ChannelId.h"
#include <iostream>

namespace ska {
namespace cheetah {
namespace pipelines {
namespace search_pipeline {


template<typename NumericalRep>
DataConfig<NumericalRep>::DataConfig()
    : BaseT("sinks")
{
}

template<typename NumericalRep>
DataConfig<NumericalRep>::~DataConfig()
{
}

template<typename NumericalRep>
void DataConfig<NumericalRep>::add_options(OptionsDescriptionEasyInit& add_options)
{
    add_options
        ("list-output-streams", boost::program_options::bool_switch()->default_value(false)->notifier(std::bind(&DataConfig<NumericalRep>::list_export_types, this, std::placeholders::_1)), "print to stdout a list of output streamers available");
    BaseT::add_options(add_options);
}

template<typename NumericalRep>
void DataConfig<NumericalRep>::list_export_types(bool b) const
{
    if(b) {
        // temporarily create a data export instance so we can query the available types
        search_pipeline::DataExport<NumericalRep> exporter(*this);
        for(auto const& type : exporter.available()) {
            std::cout << type << "\n";
        }
        terminate();
    }
}

template<typename NumericalRep>
DataExport<NumericalRep>& DataConfig<NumericalRep>::data_exporter() const
{
    if(!_out) {
        _out.reset(new DataExport<NumericalRep>(*this));
    }
    return *_out;
}

} // namespace search_pipeline
} // namespace pipelines
} // namespace cheetah
} // namespace ska
