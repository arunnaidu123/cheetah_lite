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
#ifndef SKA_CHEETAH_DATA_SPECTRUM_H
#define SKA_CHEETAH_DATA_SPECTRUM_H

#include "cheetah/data/TimeFrequency.h"
#include "pss/astrotypes/multiarray/MultiArray.h"

namespace ska {
namespace cheetah {
namespace data {

template<typename SliceType>
class SpectrumInterface : public SliceType
{
    public:
        typedef typename SliceType::template OperatorSliceType<data::Frequency>::type Channel;
        typedef typename SliceType::template ConstOperatorSliceType<data::Frequency>::type ConstChannel;

        using SliceType::SliceType;

        SpectrumInterface();
        Channel channel(std::size_t channel_number);
        ConstChannel channel(std::size_t channel_number) const;

        /// @brief return the number of channels in the data structure
        //  @details a synonym for dimension<Frequency>()
        std::size_t number_of_channels() const;
};

/**
 * @brief A single dimension representation of a Spectrum
 * @details
 */

template<typename DerivedType, typename NumericalRep, typename Alloc=std::allocator<NumericalRep>>
class SpectrumBase : public SpectrumInterface<pss::astrotypes::multiarray::MultiArray<Alloc, NumericalRep, SpectrumInterface, data::Frequency>>
                   , public panda::DataChunk<DerivedType>
{
        typedef SpectrumInterface<pss::astrotypes::multiarray::MultiArray<Alloc, NumericalRep, SpectrumInterface, data::Frequency>> BaseT;

    public:
        typedef typename BaseT::Channel Channel;
        typedef typename BaseT::ConstChannel ConstChannel;
        typedef cheetah::utils::ModifiedJulianClock::time_point TimePointType;

    public:
        SpectrumBase(pss::astrotypes::DimensionSize<data::Frequency> size = pss::astrotypes::DimensionSize<data::Frequency>(0));
        ~SpectrumBase();

        /**
         * @returns the absolute time the first time sample corresponds to.
         */
        TimePointType const& start_time() const;

        /**
         * @brief set the absolute time the first time sample corresponds to.
         * @param offset the offset time from the start of the data block to get to the first time sample
         */
        void start_time(TimePointType offset);

    private:
        TimePointType _start_time;
};

template<typename NumericalRep, typename Alloc=std::allocator<NumericalRep>>
class Spectrum : public SpectrumBase<Spectrum<NumericalRep, Alloc>, NumericalRep, Alloc>
{
        typedef SpectrumBase<Spectrum<NumericalRep, Alloc>, NumericalRep, Alloc> BaseT;

    public:
        Spectrum(pss::astrotypes::DimensionSize<data::Frequency> size = pss::astrotypes::DimensionSize<data::Frequency>(0));
        ~Spectrum();
};

} // namespace data
} // namespace cheetah
} // namespace ska

namespace pss {
namespace astrotypes {

template<typename T, typename Alloc>
struct has_exact_dimensions<ska::cheetah::data::Spectrum<T, Alloc>, units::Frequency> : public std::true_type
{
};

template<typename T, typename Alloc, typename DerivedType>
struct has_exact_dimensions<ska::cheetah::data::SpectrumBase<DerivedType, T, Alloc>, units::Frequency> : public std::true_type
{
};

} // namespace pss
} // namespace astrotypes

#include "detail/Spectrum.cpp"

#endif // SKA_CHEETAH_DATA_SPECTRUM_H
