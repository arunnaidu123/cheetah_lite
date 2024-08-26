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
#ifndef SKA_CHEETAH_MODULES_RFIM_IQRMCPU_RFIM_H
#define SKA_CHEETAH_MODULES_RFIM_IQRMCPU_RFIM_H

#include "cheetah/modules/rfim/iqrmcpu/Config.h"
#include "cheetah/modules/rfim/iqrmcpu/detail/IqrmImpl.h"
#include "cheetah/data/TimeFrequency.h"
#include "cheetah/modules/rfim/RfimBase.h"
#include "cheetah/modules/rfim/PolicyInfo.h"
#include "cheetah/modules/rfim/policy/Policy.h"
#include "cheetah/utils/Architectures.h"
#include "cheetah/data/TimeFrequency.h"
#include "cheetah/data/RfimFlaggedData.h"

namespace ska {
namespace cheetah {
namespace modules {
namespace rfim {
namespace iqrmcpu {

/**
 * @brief
 *    A CPU implementation of the IQRM algorithm, which is explained in detail in
 *    https://arxiv.org/abs/2108.12434
 *
 * @details
 *    This implementation has one additional config parameter called "edge_channels", which denotes
 *    the number of channels to forcibly mask on each edge on the band. It is zero by default,
 *    in which case the algorithm's behaviour is exactly as described in the paper.
 */
template<typename RfimTraits>
class Rfim :
    public RfimBase<Rfim<RfimTraits>, typename RfimTraits::Policy>,
    private IqrmImpl
{
    typedef RfimBase<Rfim<RfimTraits>, typename RfimTraits::Policy> BaseT;
    friend BaseT;

    typedef typename RfimTraits::Policy Policy;
    typedef typename PolicyInfo<Policy>::AdapterType DataAdapter;

    public:
        typedef typename PolicyInfo<Policy>::ReturnType ReturnType;
        typedef cheetah::Cpu Architecture;

    public:
        Rfim(Config const& config);
        ~Rfim();

        using BaseT::operator();

    protected:
        template<typename DataType>
        void operator()(DataType& data, DataAdapter& adpater);
};


} // namespace iqrmcpu
} // namespace rfim
} // namespace modules
} // namespace cheetah
} // namespace ska
#include "detail/Rfim.cpp"

#endif // SKA_CHEETAH_MODULES_RFIM_IQRMCPU_RFIM_H
