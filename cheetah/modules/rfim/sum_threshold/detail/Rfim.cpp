#include "cheetah/modules/rfim/sum_threshold/Rfim.h"
#include "panda/Buffer.h"
#include "panda/Log.h"
#include "panda/TypeTraits.h"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics.hpp>
#pragma GCC diagnostic pop
#include <cmath>


namespace ska {
namespace cheetah {
namespace modules {
namespace rfim {
namespace sum_threshold {

namespace {
typedef data::TimeFrequencyFlags<Cpu> FlagType;

inline float calcThresholdI(float threshold1, unsigned window, float p)
{
    if (p <= 0.0f) {
        p = 1.5f; // according to Andre's RFI paper, this is a good default value
    }
    return (float) (threshold1 * std::pow(p, std::log2(window)) / window);
}

template<typename IteratorType>
static void calculateNormalStatistics(
    IteratorType&& begin, IteratorType const&& end, float& mean, float& median, float& std_dev)
{
    boost::accumulators::accumulator_set<
        double, boost::accumulators::stats<boost::accumulators::tag::variance,
        boost::accumulators::tag::median>> acc;
    for_each(begin, end, std::bind<void>(std::ref(acc), std::placeholders::_1));

    mean = boost::accumulators::mean(acc);
    std_dev = sqrtf(boost::accumulators::variance(acc));
    median = boost::accumulators::median(acc);
}

template<typename IteratorType>
static void calculateStatistics(IteratorType&& begin, IteratorType const&& end, float& mean, float& median, float& std_dev) {
    // In Rob's code, there is also an option for calculateWinsorizedStatistics. I left it out for now.
    calculateNormalStatistics(std::forward<IteratorType>(begin), std::forward<IteratorType const>(end),  mean, median, std_dev);
}



template<typename DataType>
static unsigned sumThreshold2DHorizontal(DataType const & powers, FlagType &flags, const unsigned window, const float threshold) {
    unsigned extra_flagged = 0;

    for(data::DimensionIndex<data::Frequency> channel(0); channel<powers.number_of_channels(); ++channel) {
        for (unsigned base = 0; base + window < powers.number_of_spectra(); ++base) {
            float sum = 0.0f;
            for(data::DimensionIndex<data::Time> time(base); time < base + window; ++time) {
                if (flags[channel][time]) { // If it was flagged in a previous iteration, replace sample with current threshold
                    sum += threshold;
                } else {
                    sum += powers.spectrum(time)[channel];
                }
            }

            if (sum >= window * threshold) {
                // flag all samples in the sequence!
                for(data::DimensionIndex<data::Time> time(base); time < base + window; ++time) {
                    if(!flags[channel][time]) {
                        ++extra_flagged;
                        flags[channel][time] = true;
                    }
                }
            }
        }
    }

    return extra_flagged;
}

template<typename DataType>
static unsigned sumThreshold2DVertical(DataType const & powers
                                      , FlagType &flags
                                      , const unsigned window
                                      , const float threshold)
{
    unsigned extra_flagged = 0;

    for(data::DimensionIndex<data::Frequency> channel(0); channel<powers.number_of_channels(); ++channel) {
        for (unsigned base = 0; base + window < powers.number_of_spectra(); base++) {
            float sum = 0.0f;

            for (data::DimensionIndex<data::Time> time(base); time < base + window; ++time) {
                if(flags[channel][time]) { // If it was flagged in a previous iteration, replace sample with current threshold
                    sum += threshold;
                } else {
                    sum += powers.spectrum(time)[channel];
                }
            }

            if (sum >= window * threshold) {
                // flag all samples in the sequence!
                for (data::DimensionIndex<data::Time> time(base); time < base + window; ++time) {
                    if(!flags[channel][time]) {
                        ++extra_flagged;
                        flags[channel][time] = true;
                    }
                }
            }
        }
    }

    return extra_flagged;
}
} // namespace

template<typename RfimTraits>
Rfim<RfimTraits>::Rfim(const Config& config)
    : _config(config)
{
}

template<typename RfimTraits>
Rfim<RfimTraits>::~Rfim()
{
}

namespace {
    // Produces a suitable type for recording rfim flags
    // default is to use a new structure
    template<typename DataAdapterType, typename Enable=void>
    struct FlagTypeHelper {
        typedef FlagType ReturnType;
        template<typename DataType>
        FlagType flags(DataAdapterType&, DataType const& data) {
            return FlagType(data.template dimension<data::Time>(), data.template dimension<data::Frequency>());
        }
    };

    // unless a flags structure already exists
    template<typename DataAdapterType>
    struct FlagTypeHelper<DataAdapterType, typename std::enable_if<std::is_convertible<typename panda::is_pointer_wrapper<typename DataAdapterType::ReturnType>::type, FlagType&>::value>::type> {
        typedef FlagType& ReturnType;
        template<typename DataType>
        FlagType& flags(DataAdapterType& adapter, DataType const&) {
            return static_cast<FlagType&>(*adapter.data());
        }
    };

} // namespace

template<typename RfimTraits>
template<typename DataType>
void Rfim<RfimTraits>::operator()(DataType& data, DataAdapter& adapter)
{
    float mean, std_dev, median;
    calculateStatistics(data.begin(), data.end(), mean, median, std_dev);
    float factor;
    if (std_dev == 0.0f) {
      factor = _config.base_sensitivity();
    } else {
      factor = std_dev * _config.base_sensitivity();
    }

    unsigned extra_flagged = 0;

    FlagTypeHelper<DataAdapter> flag_helper;
    typename FlagTypeHelper<DataAdapter>::ReturnType flags = flag_helper.flags(adapter, data);
    for (unsigned window : _config.thresholding_data_sizes()) {
      float thresholdI = median + calcThresholdI(_config.its_cutoff_threshold(), window, 1.5f) * factor;
      extra_flagged += sumThreshold2DHorizontal(data, flags, window, thresholdI);
      extra_flagged += sumThreshold2DVertical(data, flags, window, thresholdI);
    }

    // Getting the FlaggedDataReplacer object from RfimBase
    auto data_replacer = BaseT::data_replacer();
    data_replacer.update_stats(data);

    // Ensure adpater interface is called //TODO this should really happen during rfim discovery rather than as a final step
    auto it=data.begin();
    auto flag_it =flags.begin();
    for ( data::DimensionIndex<data::Time> spectrum_num(0); spectrum_num < data.number_of_spectra(); ++spectrum_num ) {
        std::size_t bad_channel=0;
        for ( data::DimensionIndex<data::Frequency> channel_num(0); channel_num < data.number_of_channels(); ++channel_num ) {
            if(*flag_it) {
                (*it) = *(++data_replacer);
                adapter.mark_bad(*it, channel_num);
                ++bad_channel;
            }
            ++it;
            ++flag_it;
        }
        // we have a good spectrum
        if(bad_channel==0) adapter.mark_good(data.spectrum(spectrum_num));
    }
}


} // namespace sum_threshold
} // namespace rfim
} // namespace modules
} // namespace cheetah
} // namespace ska
