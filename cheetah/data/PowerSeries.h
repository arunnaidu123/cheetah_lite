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
#ifndef SKA_CHEETAH_DATA_POWERSERIES_H
#define SKA_CHEETAH_DATA_POWERSERIES_H

#include "cheetah/data/FrequencySeries.h"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"
#include <boost/math/distributions/chi_squared.hpp>
#include <boost/math/distributions/normal.hpp>
#pragma GCC diagnostic pop

namespace ska {
namespace cheetah {
namespace data {


/**
 * @brief      Class for power series (detected FrequencySeries).
 *
 * @details     This class provides and extension to the FrequencySeries class to specifically
 *             deal with data that has been converted to spectral power measurements that obey
 *             a chi2 distribution with k degrees of freedom. The PowerSeries allows the change
 *             in the degrees of freedom of the data to be tracked as the data goes through the
 *             Tdas pipeline (where harmonic summing increases the effected degrees of freedom).
 *             No checks are performed to determine that the data follow a chi2 distribution with
 *             the specified degrees of freedom.
 *
 * @tparam     Arch       The architecture on which the memory for the series is allocated
 * @tparam     ValueType  The underlying value type of the (should be Real type)
 * @tparam     num_spectra totla number of contiguos spectra in the object (default = 1)
 * @tparam     Alloc      The allocator used to provide allocate and deallocate memory
 */
template <typename Arch, typename ValueType, unsigned num_spectra = 1, typename Alloc=typename DefaultAllocator<Arch,ValueType>::type>
class PowerSeries
    : public FrequencySeries<Arch, ValueType, Alloc>
    , public panda::DataChunk<PowerSeries<Arch, ValueType, num_spectra, Alloc>>
{
    private:
        typedef FrequencySeries<Arch, ValueType, Alloc> BaseT;

    public:
        typedef Alloc Allocator;

    public:
        typedef boost::math::chi_squared_distribution<double> ChiSquaredType;
        typedef boost::math::normal_distribution<double> GaussianType;

    public:
        /**
         * @brief      Create a new default PowerSeries instance
         */
        PowerSeries(std::size_t size=0, Allocator const& allocator = Allocator());

        /**
         * @brief      Create a new PowerSeries instance
         *
         * @param      df                  The frequency step between neighbouring bins (in Hz)
         * @param[in]  degrees_of_freedom  The degrees of freedom of the data distribution
         *                                 (assumed to be chi2)
         */
        PowerSeries(FourierFrequencyType const& df, double degrees_of_freedom=2.0f
                   , std::size_t=0, Allocator const& allocator=Allocator());
        ~PowerSeries();

        /**
         * @brief      Retreive the (assumed) degrees of freedom of the data distribution
         *
         * @return     The degrees of freedom of the data distribution
         */
        double degrees_of_freedom() const;

        /**
         * @brief      Set the degrees of freedom of the data distribution
         *
         * @param[in]  dof   The degree of freedom
         */
        void degrees_of_freedom(double dof);

        /**
         * @brief      Return the statistical probability of a given power level occurring
         *
         * @details     Here we are assuming the statistical probability of a single draw from
         *             a chi2 distribution and do not take into account the number of points (trials)
         *             in the PowerSeries instance.
         *
         * @param[in]  power  A power value
         *
         * @return     The probability that the input power value would occur by chance
         */
        double pvalue(float power) const;

        /**
         * @brief      For a given power level compute the Gaussian equivalent sigma
         *
         * @param[in]  power  The power
         *
         * @return     The Gaussian equivalent sigma
         */
        float power_to_equiv_sigma(float power) const;

        /**
         * @brief      Compute the power that corresponds to a Gaussian equivalent sigma
         *
         * @param[in]  sigma  The Gaussian equivalent sigma
         *
         * @return     The corresponding power level
         */
        float equiv_sigma_to_power(float sigma) const;

    private:
        double _dof;
        ChiSquaredType _distribution;
        GaussianType _gaussian;
};


} // namespace data
} // namespace cheetah
} // namespace ska

#include "cheetah/data/detail/PowerSeries.cpp"

#endif // SKA_CHEETAH_DATA_POWERSERIES_H
