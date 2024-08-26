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
#include "cheetah/utils/TaskConfigurationSetter.h"

namespace ska {
namespace cheetah {
namespace utils {


template<typename ModuleTraits>
template<typename... ConfigurableTaskFunctorInitialisers>
AlgoModule<ModuleTraits>::AlgoModule(PoolType pool, AlgoFactory const& factory, Handler handler, ConfigurableTaskFunctorInitialisers&&... args)
    : _task(pool, handler, std::forward<ConfigurableTaskFunctorInitialisers>(args)...)
{
    utils::TaskConfigurationSetter<Algos>::configure(_task, factory);
}

template<typename ModuleTraits>
template<typename... DataT>
inline
std::shared_ptr<panda::ResourceJob> AlgoModule<ModuleTraits>::operator()(DataT&&... data) const
{
    return _task.submit(std::forward<DataT>(data)...);
}

template<typename ModuleTraits>
template<typename... DataT>
inline
std::shared_ptr<panda::ResourceJob> AlgoModule<ModuleTraits>::submit(DataT&&... data) const
{
    return _task.submit(std::forward<DataT>(data)...);
}

template<typename ModuleTraits>
template<class ArchT, typename... DataT>
inline
void AlgoModule<ModuleTraits>::operator()(panda::PoolResource<ArchT>& resource, DataT&&... data) const
{
    _task(resource, std::forward<DataT>(data)...);
}

template<typename ModuleTraits>
typename AlgoModule<ModuleTraits>::TaskType const& AlgoModule<ModuleTraits>::configured_task() const
{
    return _task;
}

} // namespace utils
} // namespace cheetah
} // namespace ska
