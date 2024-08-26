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
#ifndef SKA_CHEETAH_MODULES_RFIM_FLAGPOLICY_H
#define SKA_CHEETAH_MODULES_RFIM_FLAGPOLICY_H

#include "cheetah/data/RfimFlaggedData.h"
#include "cheetah/data/TimeFrequency.h"
#include <type_traits>

namespace ska {
namespace cheetah {
namespace modules {
namespace rfim {

/**
 * @brief Does not change the data, but prduces a set of boolena flags to indicate suspected interference
 * @details
 */
template<typename TimeFrequencyType>
class FlagPolicyAdapter
{
        typedef typename TimeFrequencyType::DataType NumericalRep;
        typedef data::RfimFlaggedData<TimeFrequencyType> FlaggedDataType;
        typedef typename TimeFrequencyType::Channel Channel;

    public:
        typedef std::shared_ptr<FlaggedDataType> ReturnType;

        FlagPolicyAdapter(TimeFrequencyType&);
        ~FlagPolicyAdapter();
        ReturnType data();

        template<typename SliceT>
        typename std::enable_if<pss::astrotypes::is_slice<SliceT>::value, void>::type
        mark_good(SliceT const&);
        void mark_good(NumericalRep const&, data::DimensionIndex<data::Frequency> channel_number);

        template<typename SliceT>
        typename std::enable_if<pss::astrotypes::is_slice<SliceT>::value, void>::type
        mark_bad(SliceT const& slice);

        void mark_bad(NumericalRep const&, data::DimensionIndex<data::Frequency>);

     private:
        ReturnType _data;
        data::TimeFrequencyFlags<Cpu>& _flag_data;
};

template<typename TimeFrequencyType>
class FlagPolicy
{
    public:
        typedef FlagPolicyAdapter<TimeFrequencyType> Adapter;
        typedef typename Adapter::ReturnType ReturnType;

    public:
        FlagPolicy();
        ~FlagPolicy();
        Adapter adapter(TimeFrequencyType& data) const;
};

} // namespace rfim
} // namespace modules
} // namespace cheetah
} // namespace ska:w

#include "detail/FlagPolicy.cpp"

#endif // SKA_CHEETAH_MODULES_RFIM_FLAGPOLICY_H
