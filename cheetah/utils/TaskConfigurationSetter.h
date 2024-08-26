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
#ifndef SKA_CHEETAH_UTILS_TASKCONFIGURATIONSETTER_H
#define SKA_CHEETAH_UTILS_TASKCONFIGURATIONSETTER_H
#include <tuple>

namespace ska {
namespace cheetah {
namespace utils {

/**
 * @brief Generate a suitable call to the configure method of a panda::ConfigurableTask<...> object
 *        based on the contents of a parmaeter pack
 * @details
 *      By adding extra requirements on the algos types we pass to the task onject
 *      we can generate some setup automatically based on the template parameter list alone
 *      This function will generate a Select<Algo>(factory.active<Algo>()) for each type in Algos
 *      parmeter list and pass it down to the task.configure method
 *
 * @tparam Algos are a comma sperated list of algorithms that are seelctable for the chosen task type.
 *         Each algo passed must be compatible with the task type.
 *         The factory frovided must support the requirements fo the Tasks configure method
 *         and in addition must support the method
 *         @code
 *              template<class algo>
 *              bool active<Algo>() const;
 *         @endcode
 *
 * @example
 * @code
 * MyHandlerType handler;
 * panda::ConfigurableTask<PoolType
 *                       , MyHandlerType> task(pool_manager, handler);
 *
 * MyFractory factory(some_config_type);
 * if(! TaskConfigurationSetter<Algo1, Algo2, Algo3>::configure(task, factory)) {
 *    // no algos selected i.e no active<Algo>() method has returned true
 * }
 *
 * @endcode
 */
template<typename... Algos>
class TaskConfigurationSetter;

template<typename Algo, typename... Algos>
class TaskConfigurationSetter<Algo, Algos...> {
    public:
        /**
         * @brief configure the task
         * @tparam FactoryT see requirements for panda:ConfigurableTask::configure
         *         Must aslo support
         *         @code
         *         bool active<AlgoType>() const;
         *         @endcode
         * @tparam Args... will be passed on to the configure method after all generated Select<> args
         */
        template<typename FactoryT, typename TaskT, typename... Args>
        static inline bool configure(TaskT& task, FactoryT& factory, Args&&... args);
};

template<typename Algo, typename... Algos>
class TaskConfigurationSetter<std::tuple<Algo, Algos...>> : public TaskConfigurationSetter<Algo, Algos...>
{
};

} // namespace utils
} // namespace cheetah
} // namespace ska
#include "cheetah/utils/detail/TaskConfigurationSetter.cpp"

#endif // SKA_CHEETAH_UTILS_TASKCONFIGURATIONSETTER_H
