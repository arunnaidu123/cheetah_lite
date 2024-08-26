/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2023 The SKA organisation
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
#include "cheetah/modules/sps/klotski_common/KlotskiCommon.h"
#include "cheetah/data/SpCandidate.h"
#include "cheetah/data/DmTrialsMetadata.h"
#include "cheetah/data/DmTrials.h"
#include "cheetah/data/SpCcl.h"
#include "cheetah/data/TimeFrequency.h"
#include "cheetah/data/Units.h"
#include "cheetah/data/DedispersionMeasure.h"
#include "panda/Resource.h"
#include "panda/Log.h"
#include "panda/Error.h"
#include <cstdlib>

namespace ska {
namespace cheetah {
namespace modules {
namespace sps {
namespace klotski_bruteforce {

namespace detail {

template<class SpsTraits>
class Sps<SpsTraits> : private klotski_common::KlotskiCommon<SpsTraits, ddtr::klotski_bruteforce::Ddtr<SpsTraits>, Config, sps::Config>
{
    private:
        typedef klotski_common::KlotskiCommon<SpsTraits, ddtr::klotski_bruteforce::Ddtr<SpsTraits>, Config, sps::Config> BaseT;

    public:
        // mark the architecture this algo is designed for
        typedef cheetah::Cpu Architecture;
        typedef ddtr::klotski_bruteforce::DedispersionPlan<SpsTraits> DedispersionPlan;

    public:
        typedef data::TimeFrequency<Cpu, uint8_t> TimeFrequencyType;

    private:
        typedef typename SpsTraits::BufferType BufferType;
        typedef typename SpsTraits::DmTrialsType DmTrialsType;

    public:
        Sps(sps::Config const& config);
        Sps(Sps&&) = default;
        Sps(Sps const&) = delete;

        /**
         * @brief call the dedispersion/sps algorithm using the provided device
         */
        template<typename SpHandler>
        std::shared_ptr<typename SpsTraits::DmTrialsType> operator()(panda::PoolResource<cheetah::Cpu>&, BufferType&, SpHandler const&);

        /**
         * @brief set the dedispersion plan
         */
        void plan(DedispersionPlan const& plan);
};

template<class SpsTraits>
template<typename SpHandler>
std::shared_ptr<typename SpsTraits::DmTrialsType> Sps<SpsTraits>::operator()(panda::PoolResource<panda::Cpu>& cpu
                    , BufferType& agg_buf
                    , SpHandler const& sp
                    )
{
    return static_cast<BaseT&>(*this)(cpu, agg_buf, sp);
}

template<class SpsTraits>
Sps<SpsTraits>::Sps(sps::Config const& config)
    : BaseT(config.klotski_bruteforce_config(), config)
{
}

template<class SpsTraits>
void Sps<SpsTraits>::plan(DedispersionPlan const& plan)
{
    BaseT::plan(plan);
}

} // namespace detail

template<class SpsTraits>
Sps<SpsTraits>::Sps(sps::Config const& config)
    : BaseT(config)
{
}

template<class SpsTraits>
template<typename SpHandler, typename BufferType>
std::shared_ptr<typename SpsTraits::DmTrialsType> Sps<SpsTraits>::operator()(panda::PoolResource<Architecture>& dev, BufferType& buf, SpHandler& sh)
{
    return static_cast<BaseT&>(*this)(dev, buf, sh);
}


} // namespace klotski_bruteforce
} // namespace sps
} // namespace modules
} // namespace cheetah
} // namespace ska