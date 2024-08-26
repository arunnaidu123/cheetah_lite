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
#include "cheetah/producers/Factory.h"
#include "cheetah/producers/SpCclSpeadProducer.h"
#include "panda/Pipeline.h"
#include "panda/Log.h"


namespace ska {
namespace cheetah {
namespace producers {

template<typename DataType>
template<typename ComputeModule>
int Factory<DataType>::exec(std::string const&, ComputeModule&)
{
    static_assert(std::is_same<DataType, std::true_type>::value, "unrecognised datatype");
    return 0;
}

template<typename NumericalT>
Factory<data::SpCandidateData<data::TimeFrequency<Cpu, NumericalT>>>::Factory(ConfigType const& producers_config)
    : _config(producers_config)
{
}

template<typename NumericalT>
std::vector<std::string> Factory<data::SpCandidateData<data::TimeFrequency<Cpu, NumericalT>>>::available() const
{
    return { "spead" };
}


template<typename NumericalT>
template<typename ComputeModule>
int Factory<data::SpCandidateData<data::TimeFrequency<Cpu, NumericalT>>>::exec(std::string const& stream_name, ComputeModule& pipeline)
{
    int rv = 1;
    if(stream_name == "spead")
    {
#ifdef ENABLE_SPEAD
        SpCclSpeadProducer<TimeFrequencyType> data_stream(_config.spead_config());
        rv=panda::Pipeline<SpCclSpeadProducer<TimeFrequencyType>>(data_stream, [&](DataType& data) { pipeline(data); } ).exec();
#else // ENABLE_SPEAD
        (void) pipeline;
        PANDA_LOG_ERROR << "spead producer has not been compiled in. Recompile with the -DENABLE_SPEAD=truei option set in cmake";
#endif // ENABLE_SPEAD
    }
    else {
        PANDA_LOG_ERROR << "unknown source stream: " << stream_name;
    }
    return rv;
}

} // namespace producers
} // namespace cheetah
} // namespace ska
