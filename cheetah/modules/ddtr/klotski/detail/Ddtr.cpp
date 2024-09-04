/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016-2022 The SKA organisation
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


#include "cheetah/modules/ddtr/klotski/DdtrProcessor.h"
#include "cheetah/modules/ddtr/klotski/Ddtr.h"

namespace ska {
namespace cheetah {
namespace modules {
namespace ddtr {
namespace klotski {

template<typename DdtrTraits>
Ddtr<DdtrTraits>::Ddtr(BeamConfigType const& beam_config, ddtr::Config const& config)
    : _plan(std::make_shared<DedispersionPlan>(beam_config, config, 0))
{
}

template<typename DdtrTraits>
Ddtr<DdtrTraits>::Ddtr(Ddtr&& other)
    : _plan(std::move(other._plan))
{
}

template<typename DdtrTraits>
Ddtr<DdtrTraits>::~Ddtr()
{
}

template<typename DdtrTraits>
template<typename CallBackT>
std::shared_ptr<typename Ddtr<DdtrTraits>::DmTrialsType> Ddtr<DdtrTraits>::operator()(panda::PoolResource<cheetah::Cpu>&, std::shared_ptr<BufferType> data, CallBackT const& call_back)
{
    return _worker(data, _plan, call_back);
}

template<typename DdtrTraits>
std::shared_ptr<typename Ddtr<DdtrTraits>::DmTrialsType> Ddtr<DdtrTraits>::operator()(panda::PoolResource<cheetah::Cpu>& cpu, std::shared_ptr<BufferType> data)
{
    return (*this)(cpu, data, [](DmTrialsType const&, std::vector<unsigned int> const&){});
}

template<typename DdtrTraits>
void Ddtr<DdtrTraits>::plan(DedispersionPlan const& plan)
{
    _plan = std::make_shared<DedispersionPlan>(plan);
}

} // namespace klotski
} // namespace ddtr
} // namespace modules
} // namespace cheetah
} // namespace ska
