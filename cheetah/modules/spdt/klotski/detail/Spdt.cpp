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
#include "cheetah/modules/spdt/klotski_common/KlotskiCommon.h"
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
namespace spdt {
namespace klotski {

namespace detail {

template<class SpdtTraits>
class Spdt<SpdtTraits> : private klotski_common::KlotskiCommon<SpdtTraits, Config, spdt::Config>
{
    private:
        typedef klotski_common::KlotskiCommon<SpdtTraits, Config, spdt::Config> BaseT;

    public:
        // mark the architecture this algo is designed for
        typedef cheetah::Cpu Architecture;
        typedef ddtr::klotski::DedispersionPlan<SpdtTraits> DedispersionPlan;

    public:
        typedef data::TimeFrequency<Cpu, uint8_t> TimeFrequencyType;

    private:
        typedef typename SpdtTraits::BufferType BufferType;
        typedef typename SpdtTraits::DmTrialsType DmTrialsType;

    public:
        Spdt(spdt::Config const& config);
        Spdt(Spdt&&) = default;
        Spdt(Spdt const&) = delete;

        /**
         * @brief call the dedispersion/spdt algorithm using the provided device
         */
        std::shared_ptr<typename SpdtTraits::SpType> operator()(panda::PoolResource<cheetah::Cpu>&, std::shared_ptr<DmTrialsType>);

        /**
         * @brief set the dedispersion plan
         */
        void plan(DedispersionPlan const& plan);
};

template<class SpdtTraits>
std::shared_ptr<typename SpdtTraits::SpType> Spdt<SpdtTraits>::operator()(panda::PoolResource<panda::Cpu>& cpu, std::shared_ptr<DmTrialsType> buf)
{
    return static_cast<BaseT&>(*this)(cpu, buf);
}

template<class SpdtTraits>
Spdt<SpdtTraits>::Spdt(spdt::Config const& config)
    : BaseT(config.klotski_config(), config)
{
}

} // namespace detail

template<class SpdtTraits>
Spdt<SpdtTraits>::Spdt(spdt::Config const& config)
    : BaseT(config)
{

}

template<class SpdtTraits>
std::shared_ptr<typename SpdtTraits::SpType> Spdt<SpdtTraits>::operator()(panda::PoolResource<Architecture>& dev, std::shared_ptr<DmTrialsType> buf)
{
    return static_cast<BaseT&>(*this)(dev, buf);
}


} // namespace klotski
} // namespace spdt
} // namespace modules
} // namespace cheetah
} // namespace ska