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
#ifndef SKA_CHEETAH_FREQUENCYTIME_H
#define SKA_CHEETAH_FREQUENCYTIME_H

#include "cheetah/data/Units.h"
#include "cheetah/data/TimeFrequencyCommon.h"
#include "cheetah/utils/Architectures.h"
#include "panda/DataChunk.h"

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

template <class Arch, typename NumericalT> class TimeFrequency;

/**
 * @brief A Frequency/Time data chunk.
 *
 * @details Contiguous block of frequency channels and time samples.
 *
 * This class is based on the TimeFrequency class
 *
 * The frequencies represented in each time sample should be set/referenced from
 * with the channel_frequencies methods.
 *
 * Each time sample is represents an indentical frequency range.
 *
 * @example
 * @code
 * #include "cheetah/data/FrequecyTime.h"
 * #include <algorithm>
 *
 * FrequecyTime<Cpu, uint8_t> ft_block(DimensionSize<Time>(1000), DimensionSize<Frequency>(4096));
 * ...
 * // using the iterator interface
 * for(unsigned i=0; i < ft_block.number_of_spectra(); ++i) {
 *        // iterate over channels
 *       Spectra spectrum = ft_block.spectrum(i);
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
 * ft_block[DimesnionIndex<Time>(2)][DimensionIndex<Frequency>(3)] = 10;
 *
 * @endcode
 * @endexample
 *
 */

template <class Arch, typename NumericalT>
class FrequencyTime : public pss::astrotypes::FrequencyTime<NumericalT> // TODO Arch in base type -> use allocator
                    , public TimeFrequencyCommon<FrequencyTime<Arch, NumericalT>>
{
        typedef pss::astrotypes::FrequencyTime<NumericalT> BaseT;
        typedef TimeFrequencyCommon<FrequencyTime<Arch, NumericalT>> CommonBaseT;

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
        typedef TimeFrequency<Arch, NumericalT> TimeFrequencyType;

        /**
         * @brief Iterator class for accessing each time sample
         * @details note the iterator has its own begin() and end() methods
         * to generate iterators over the frequency channels of the sample
         */
        typedef typename BaseT::Spectra Spectra;
        typedef typename BaseT::ConstSpectra ConstSpectra;
        typedef typename BaseT::Channel Channel;
        typedef typename BaseT::ConstChannel ConstChannel;

    public:
        /**
         * Construct an empty FrequencyTime object
         */
        FrequencyTime();

        /**
         * Construct a defined size FrequencyTime object
         */
        FrequencyTime(DimensionSize<Time> number_of_spectra, DimensionSize<Frequency> number_of_channels);
        FrequencyTime(DimensionSize<Frequency> number_of_channels, DimensionSize<Time> number_of_spectra);

        /**
         * Copy from A FrequencyTime object stored on another device
         */
        template<class OtherArch, typename OtherNumericalT>
        FrequencyTime(FrequencyTime<OtherArch, OtherNumericalT> const&);

        template<typename OtherNumericalT>
        FrequencyTime(TimeFrequency<Cpu, OtherNumericalT> const& tf);

        template<typename OtherArch, typename OtherNumericalT>
        FrequencyTime(TimeFrequency<OtherArch, OtherNumericalT> const& tf);

        FrequencyTime(TimeFrequency<Arch, NumericalT> const& ft);

        /**
         * @brief sets the value of the specified channel to the provided
         * value across all time samples
         */
        void set_channel(unsigned channel_number, DataType const& value);

        /**
         * @brief return true if equivalent data
         */
        bool operator==(FrequencyTime const&) const;
};

/**
 * @brief extract the FrequencyTime data type from compound types
 * @details
 * @code
 * template<DataType> do_domething(DataType const& data)
 * {
 *      auto const& ft_data_ref = ExtractFrequencyTimeDataType<DataType>::extract(data);
 * }
 * @endcode
 */
template<typename T>
struct ExtractFrequencyTimeDataType;

/**
 * @brief Implementation Helper class for the ExtractFrequencyTimeDataType - do not call directly
 * @details Where you want to specialise on type_traits you can specialise this class
 *          to extract the FrequencyTime data types
 */
template<typename T, typename Enable = void>
struct ExtractFrequencyTimeDataTypeHelper;

} // namespace data
} // namespace cheetah
} // namespace ska

namespace pss {
namespace astrotypes {

template<typename Alloc, typename T>
struct has_exact_dimensions<ska::cheetah::data::FrequencyTime<T, Alloc>, units::Frequency, units::Time> : public std::true_type
{
};

} // namespace pss
} // namespace astrotypes

#include "detail/FrequencyTime.cpp"
#include "cuda/FrequencyTime.h"

#endif // SKA_CHEETAH_FREQUENCYTIME_H
