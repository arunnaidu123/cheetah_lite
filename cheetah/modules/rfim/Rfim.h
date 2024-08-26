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
#ifndef SKA_CHEETAH_MODULES_RFIM_RFIM_H
#define SKA_CHEETAH_MODULES_RFIM_RFIM_H

#include "cheetah/modules/rfim/Config.h"
#include "cheetah/modules/rfim/PolicyInfo.h"
#include "cheetah/modules/rfim/DefaultRfimTraits.h"
#include "panda/ConfigurableTask.h"
#include <memory>
#include <functional>

namespace ska {
namespace cheetah {
namespace modules {
namespace rfim {

/**
 * @brief
 *    Main API for RFIM functionality
 */

template<typename TimeFrequencyType, typename RfimTraits=DefaultRfimTraits<TimeFrequencyType>
       , typename ConfigType=panda::PoolSelector<typename Config::PoolManagerType, Config>>
class Rfim
{
        typedef typename TimeFrequencyType::DataType NumericRep;
        typedef typename RfimTraits::Policy PolicyType;
        typedef typename PolicyInfo<PolicyType>::DataArgumentType ArgumentType;

    public:
        typedef typename PolicyInfo<PolicyType>::ReturnType ReturnType;

    public:
        Rfim(ConfigType const&, typename RfimTraits::RfimHandler&, typename RfimTraits::BandPassHandler&);
        Rfim(Rfim const&) = delete;
        ~Rfim();

        /**
         * @brief detect and remove rfi from the dataset
         */
        void run(ArgumentType data);

    private: // typedef
        typedef panda::ConfigurableTask<typename Config::PoolType, typename RfimTraits::RfimHandler&, std::shared_ptr<TimeFrequencyType>> Task;
        template<typename Algo> using Select = typename Task::template Select<Algo>;

    private: // members
        Task _task;
};


} // namespace rfim
} // namespace modules
} // namespace cheetah
} // namespace ska
#include "cheetah/modules/rfim/detail/Rfim.cpp"

#endif // SKA_CHEETAH_MODULES_RFIM_RFIM_H
