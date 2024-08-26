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
#ifndef SKA_CHEETAH_MODULES_RFIM_AMPP_SPECTRUM_H
#define SKA_CHEETAH_MODULES_RFIM_AMPP_SPECTRUM_H


#include "cheetah/data/Spectrum.h"
#include "pss/astrotypes/multiarray/MultiArray.h"

namespace ska {
namespace cheetah {
namespace modules {
namespace rfim {
namespace ampp {

/**
 * @brief A single dimension representation of the bandPass
 * @details
 */

template<typename NumericalRep, typename Alloc=std::allocator<NumericalRep>>
class Spectrum : public data::SpectrumBase<Spectrum<NumericalRep, Alloc>, NumericalRep, Alloc>
{
        typedef data::SpectrumBase<Spectrum<NumericalRep, Alloc>, NumericalRep, Alloc> BaseT;

    public:
        typedef typename BaseT::Channel Channel;
        typedef typename BaseT::ConstChannel ConstChannel;

    public:
        Spectrum(pss::astrotypes::DimensionSize<data::Frequency> size = pss::astrotypes::DimensionSize<data::Frequency>(0));
        ~Spectrum();

        /**
         * @brief adjust each channel by the provided amount
         */
        void adjust_mean(NumericalRep);

        /**
         * @brief return the calculated mean
         */
        float mean() const;
        float calculate_mean();

    private:
        float _mean;

};


} // namespace ampp
} // namespace rfim
} // namespace modules
} // namespace cheetah
} // namespace ska

namespace pss {
namespace astrotypes {

template<typename T, typename Alloc>
struct has_exact_dimensions<ska::cheetah::modules::rfim::ampp::Spectrum<T, Alloc>, units::Frequency> : public std::true_type
{
};

} // namespace pss
} // namespace astrotypes

#include "detail/Spectrum.cpp"

#endif // SKA_CHEETAH_MODULES_RFIM_AMPP_SPECTRUM_H