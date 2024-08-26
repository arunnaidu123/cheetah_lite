/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2022 The SKA organisation
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
#ifndef SKA_CHEETAH_MODULES_DDTR_RFIEXCISION_H
#define SKA_CHEETAH_MODULES_DDTR_RFIEXCISION_H
#include "cheetah/data/TimeFrequencyFlags.h"
#include "cheetah/data/TimeFrequency.h"
#include "cheetah/data/RfimFlaggedData.h"
#include <random>

namespace ska {
namespace cheetah {
namespace modules {
namespace ddtr {

/**
 * @brief
 * @details
 */
template<typename TimeFrequencyType, typename RfiFlagDataType>
class RfiExcision
{
    private:
        typedef typename std::conditional<std::is_const<TimeFrequencyType>::value
                                         ,typename TimeFrequencyType::const_iterator
                                         ,typename TimeFrequencyType::iterator>::type iterator;

        typedef typename std::conditional<std::is_const<RfiFlagDataType>::value
                                       ,typename RfiFlagDataType::TimeFrequencyFlagsType::const_iterator
                                       ,typename RfiFlagDataType::TimeFrequencyFlagsType::iterator>::type FlaggedIterator;


    public:
        typedef typename TimeFrequencyType::NumericalRep NumericalRep;

        typedef typename std::conditional<std::is_const<TimeFrequencyType>::value
                                         , NumericalRep const&, NumericalRep&>::type reference;

        typedef NumericalRep const& const_reference;

    public:
        RfiExcision(RfiFlagDataType& data, size_t offset, float ideal_rms, float threshold, bool active);
        ~RfiExcision();

        bool operator==(RfiExcision const& iterator) const;
        bool operator!=(RfiExcision const& iterator) const;

        RfiExcision& operator++();

        const_reference operator*() const;

    private:
        FlaggedIterator _flag_it;
        iterator _tf_it;
        std::size_t _wrapindex;
        NumericalRep _value;
        NumericalRep _replacement_value;
        float _ideal_rms;
        float _threshold;
        bool _active;
        typename TimeFrequencyType::const_iterator _tf_end_it;
        std::vector<float> _noise;
};

} // namespace ddtr
} // namespace modules
} // namespace cheetah
} // namespace ska
#include "cheetah/modules/ddtr/detail/RfiExcision.cpp"
#endif // SKA_CHEETAH_MODULES_DDTR_RFIEXCISION_H
