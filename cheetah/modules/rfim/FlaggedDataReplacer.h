/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2024 The SKA organisation
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
#ifndef SKA_CHEETAH_MODULES_RFIM_FLAGGEDDATAREPLACER_H
#define SKA_CHEETAH_MODULES_RFIM_FLAGGEDDATAREPLACER_H
#include "cheetah/data/RfimFlaggedData.h"
#include <random>
#include <array>

namespace ska{
namespace cheetah{
namespace modules{
namespace rfim{

/**
 * @brief
 * Module is responsible for generating data points with underlying Gaussian Distribution that can be used to replace RFI affected samples
 * @details
 * An object of the class can be created which can be then incremented and a pointer-operator can be used to point to a new value.
 * For instance, an object of this is created inside RfimBase.h which is used in iqrmcpu and sumthresholding
 */

template<typename TimeFrequencyType>
class FlaggedDataReplacer
{
    private:
        typedef typename std::conditional<std::is_const<TimeFrequencyType>::value
                                         ,typename TimeFrequencyType::const_iterator
                                         ,typename TimeFrequencyType::iterator>::type iterator;

    public:
        typedef typename TimeFrequencyType::NumericalRep NumericalRep;
        typedef typename std::conditional<std::is_const<TimeFrequencyType>::value
                                        ,NumericalRep const&, NumericalRep&>::type reference;
        typedef NumericalRep const& const_reference;

    public:
        FlaggedDataReplacer();
        ~FlaggedDataReplacer();

        const_reference operator*() const;
        void update_stats(TimeFrequencyType& data);     //method used to update stats (_mean and _rms) using TF data passed to it
        FlaggedDataReplacer& operator++();

    private:
        std::array<float, 8192> _noise;		// An array to hold noise samples
        std::size_t _wrapindex;
        iterator _tf_it;
        NumericalRep _value;		// A value holder that is updated everytime ++ operator is used
        NumericalRep _mean;			// Used to store the mean value of the input TimeFrequencyType data
        NumericalRep _rms;			// Used to store the std deviation of the input TimeFrequencyType data
};


} //namespace rfim
} //namespace modules
} //namespace cheetah
} //namespace ska
#include "cheetah/modules/rfim/detail/FlaggedDataReplacer.cpp"

#endif // SKA_CHEETAH_MODULES_RFIM_TEST_FLAGGEDDATAREPLACER_H
