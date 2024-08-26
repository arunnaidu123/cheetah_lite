/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 The SKA organisation
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
#include "cheetah/modules/rfim/iqrmcpu/Rfim.h"
#include "panda/Log.h"
#include "panda/TypeTraits.h"
#include <algorithm>
#include <iomanip>

#ifdef NDEBUG
#include <chrono>
#endif


namespace ska {
namespace cheetah {
namespace modules {
namespace rfim {
namespace iqrmcpu {

namespace {

// Produces a suitable type for recording rfim flags
// default is to use a new structure

template<typename DataAdapterType, typename TimeFrequencyType, typename Enable=void>
struct FlaggedDataTypeHelper
{
    typedef data::RfimFlaggedData< const TimeFrequencyType> FlaggedDataType;
    typedef FlaggedDataType ReturnType;
    template<typename DataType>
    ReturnType flagged_data(DataAdapterType& , DataType const& data)
    {
        return FlaggedDataType(data);
    }
};

// unless a flags structure already exists
template<typename DataAdapterType, typename TimeFrequencyType>
struct FlaggedDataTypeHelper<DataAdapterType, TimeFrequencyType, typename std::enable_if<std::is_convertible<typename panda::is_pointer_wrapper<
                                                                                                    typename DataAdapterType::ReturnType>::type
                                                                                                    , data::RfimFlaggedData<TimeFrequencyType>>::value>::type>
{
    typedef data::RfimFlaggedData<TimeFrequencyType> FlaggedDataType;
    typedef FlaggedDataType& ReturnType;
    template<typename DataType>
    ReturnType flagged_data(DataAdapterType& adapter, DataType const&)
    {
        return static_cast<FlaggedDataType&>(*adapter.data());
    }
};

} // namespace


template<typename RfimTraits>
Rfim<RfimTraits>::Rfim(Config const& config)
    : IqrmImpl(config)
{
}

template<typename RfimTraits>
Rfim<RfimTraits>::~Rfim()
{
}

template<typename RfimTraits>
template<typename DataType>
inline void Rfim<RfimTraits>::operator()(DataType& data, DataAdapter& adapter)
{
    const size_t nchan = data.number_of_channels();
    const size_t nsamp = data.number_of_spectra();
    if (nsamp * nchan == 0)
        return;

    #ifdef NDEBUG
    auto clock_stat = std::chrono::high_resolution_clock::now();
    #endif
    // compute per channel std for the TF block
    FlaggedDataTypeHelper<DataAdapter, DataType> flagged_data_helper;
    typename FlaggedDataTypeHelper<DataAdapter, DataType>::ReturnType flagged_data = flagged_data_helper.flagged_data(adapter, data);

    // Collect per-channel standard deviations, that's the IQRM input
    const auto& channel_stats = flagged_data.channel_stats(); // vector of 'Statistics' structs
    using Statistics = decltype(*channel_stats.begin()); // has a 'mean' and a 'variance' member
    std::vector<float> channel_stddevs(nchan, 0.0);
    std::transform(
        channel_stats.begin(), channel_stats.end(), channel_stddevs.begin(),
        [](Statistics const& s) {return s.stddev();}
    );

    #ifdef NDEBUG
    auto clock_iqrm = std::chrono::high_resolution_clock::now();
    #endif

    // Compute IQRM channel mask
    std::vector<bool> mask(nchan, false);
    IqrmImpl::iqrm_mask(channel_stddevs.begin(), channel_stddevs.end(), mask.begin());

    #ifdef NDEBUG
    auto clock_flag = std::chrono::high_resolution_clock::now();
    #endif

    // Getting the FlaggedDataReplacer object from RfimBase
    auto data_replacer = BaseT::data_replacer();
    data_replacer.update_stats(data);  //Updating stats of the FlaggedDataReplacer everytime a new TF-block is processed

    // Write channel flags via the adapter
    size_t count = 0;
    DataType noise_channel(data::DimensionSize<data::Time>(data.number_of_spectra())
                                                            ,data::DimensionSize<data::Frequency>(1));
    std::transform(noise_channel.channel(0).begin(),noise_channel.channel(0).end()
                    ,noise_channel.channel(0).begin(), [&](auto d)
    {
        ++d;
        return *(++data_replacer);
    });
    for(size_t ii=0;ii<nchan;++ii)
    {
        if(mask[ii])
        {
            adapter.mark_bad(data.channel(ii));
            std::copy(noise_channel.channel(0).begin(),noise_channel.channel(0).end(),data.channel(ii).begin());
            ++count;
        }
    }
    #ifdef NDEBUG
    auto clock_end = std::chrono::high_resolution_clock::now();
    double stat_ms = std::chrono::duration<double, std::milli>(clock_iqrm - clock_stat).count();
    double iqrm_ms = std::chrono::duration<double, std::milli>(clock_flag - clock_iqrm).count();
    double flag_ms = std::chrono::duration<double, std::milli>(clock_end  - clock_flag).count();

    PANDA_LOG_DEBUG << "rfim::iqrm : fraction masked: "
        << std::fixed << std::setprecision(6) << count / double(nchan);
    PANDA_LOG_DEBUG << "rfim::iqrm : stat run time: "
        << std::fixed << std::setprecision(2) << stat_ms << " ms";
    PANDA_LOG_DEBUG << "rfim::iqrm : IQRM run time: "
        << std::fixed << std::setprecision(2) << iqrm_ms << " ms";
    PANDA_LOG_DEBUG << "rfim::iqrm : channel flagging run time: "
        << std::fixed << std::setprecision(2) << flag_ms << " ms";
    #endif
}

} // namespace iqrmcpu
} // namespace rfim
} // namespace modules
} // namespace cheetah
} // namespace ska
