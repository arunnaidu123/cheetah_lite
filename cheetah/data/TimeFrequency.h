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
#ifndef SKA_CHEETAH_TIMEFREQUENCY_H
#define SKA_CHEETAH_TIMEFREQUENCY_H

#include "cheetah/data/Units.h"
#include "cheetah/data/TimeFrequencyCommon.h"
#include "cheetah/utils/Architectures.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"
#include "pss/astrotypes/types/TimeFrequency.h"
#pragma GCC diagnostic pop

namespace ska {
namespace cheetah {
namespace data {

template <class Arch, typename NumericalT> class FrequencyTime;

/**
 * @brief A Time/Frequency data chunk.
 *
 * @details Contiguous block of time samples and frequency channels.
 *
 * The frequencies represented in each time sample shuld be set/referenced from
 * with the channel_frequencies methods.
 *
 * Each time sample is represents an indentical frequency range.
 *
 * @code
 * #include "cheetah/data/TimeFrequecy.h"
 * #include <algorithm>
 *
 * TimeFrequecy<Cpu, uint8_t> tf_block(DimensionSize<Time>(1000), DimensionSize<Frequency>(4096));
 * ...
 * // using the iterator interface
 * for(unsigned i=0; i < tf_block.number_of_spectra(); ++i) {
 *        // iterate over channels
 *       Spectra spectrum = tf_block.spectrum(i);
 *       auto channel_it = spectrum.begin();
 *       while(channel_it != spectrum.end())
 *       {
 *          // do something with the channel value
 *          ++channel_it;
 *       }
 *       // or use the std::algoritms e.h. fill with zeros
 *       std::fill(spectrum.begin(), spectrum.end(), 0U);
 * }
 *
 * // using the operator[] interface
 * tf_block[DimesnionIndex<Time>(2)][DimensionIndex<Frequency>(3)] = 10;
 *
 * @endcode
 *
 */

template <class Arch, typename NumericalT>
class TimeFrequency : public pss::astrotypes::TimeFrequency<NumericalT> // TODO Arch in base type -> use allocator
                    , public TimeFrequencyCommon<TimeFrequency<Arch, NumericalT>>
{
        typedef pss::astrotypes::TimeFrequency<NumericalT> BaseT;
        typedef TimeFrequencyCommon<TimeFrequency<Arch, NumericalT>> CommonBaseT;

    public:
        /**
         * @brief the underlying data storage type for the amplitude of the signal
         */
        typedef NumericalT DataType;
        typedef NumericalT NumericalRep;
        typedef Arch Architecture;

        typedef typename CommonBaseT::FrequencyType  FrequencyType;
        typedef typename CommonBaseT::TimeType  TimeType;
        typedef typename CommonBaseT::TimePointType TimePointType;

        typedef typename BaseT::iterator Iterator;
        typedef typename BaseT::const_iterator ConstIterator;

        /**
         * @brief Iterator class for accessing each time sample
         * @details note the iterator has its own begin() and end() methods
         * to generate iterators over the frequency channels of the sample
         */
        typedef typename BaseT::Spectra Spectra;
        typedef typename BaseT::ConstSpectra ConstSpectra;
        typedef typename BaseT::Channel Channel;
        typedef typename BaseT::ConstChannel ConstChannel;

        using pss::astrotypes::TimeFrequency<NumericalT>::TimeFrequency;

    public:
        /**
         * Construct an empty TimeFrequency object
         */
        TimeFrequency();

        /**
         * Construct a defined size TimeFrequency object
         */
        TimeFrequency(DimensionSize<Time> number_of_spectra, DimensionSize<Frequency> number_of_channels);
        TimeFrequency(DimensionSize<Frequency> number_of_channels, DimensionSize<Time> number_of_spectra);

        template<class OtherArch, typename OtherNumericalT>
        TimeFrequency(const TimeFrequency<OtherArch, OtherNumericalT>& tf);

        template<typename OtherNumericalT>
        TimeFrequency(FrequencyTime<Cpu, OtherNumericalT> const& ft);

        template<class OtherArch, typename OtherNumericalT>
        TimeFrequency(FrequencyTime<OtherArch, OtherNumericalT> const& ft);

        TimeFrequency(FrequencyTime<Arch, NumericalT> const& ft);

        /**
         * @returns the integer number of channels in the block
         */
        std::size_t number_of_channels() const;

        /**
         * @returns the integer number of samples in the block
         */
        std::size_t number_of_spectra() const;

        /**
         * @brief sets the value of the specified channel to the provided
         * value across all time samples
         */
        void set_channel(unsigned channel_number, DataType const& value);

        /**
         * @brief return true if equivalent data
         */
        bool operator==(TimeFrequency const&) const;
};

/**
 * @brief extract the TimeFrequency data type from compound types
 * @details
 * @code
 * template<DataType> do_domething(DataType const& data)
 * {
 *      auto const& tf_data_ref = ExtractTimeFrequencyDataType<DataType>::extract(data);
 * }
 * @endcode
 */
template<typename T>
struct ExtractTimeFrequencyDataType;

/**
 * @brief Implementation Helper class for the ExtractTimeFrequencyDataType - do not call directly
 * @details Where you want to specialise on type_traits you can specialise this class
 *          to extract the TimeFrequency data types
 */
template<typename T, typename Enable = void>
struct ExtractTimeFrequencyDataTypeHelper;

} // namespace data
} // namespace cheetah
} // namespace ska

namespace pss {
namespace astrotypes {

template<typename Alloc, typename T>
struct has_exact_dimensions<ska::cheetah::data::TimeFrequency<T, Alloc>, units::Time, units::Frequency> : public std::true_type
{
};

} // namespace pss
} // namespace astrotypes

#include "detail/TimeFrequency.cpp"
#include "cuda/TimeFrequency.h"

#endif // SKA_CHEETAH_TIMEFREQUENCY_H
