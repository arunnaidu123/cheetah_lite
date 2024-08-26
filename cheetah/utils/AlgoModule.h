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
#ifndef SKA_CHEETAH_UTILS_ALGOMODULE_H
#define SKA_CHEETAH_UTILS_ALGOMODULE_H

#include "panda/ConfigurableTask.h"

namespace ska {
namespace cheetah {
namespace utils {


template<class PoolType, class HandlerT, typename... DataTypes>
struct AlgoModuleTaskType;

template<class PoolType, class HandlerT, typename... DataTypes>
struct AlgoModuleTaskType<PoolType, HandlerT, std::tuple<DataTypes...>>
{
    typedef panda::ConfigurableTask<PoolType, HandlerT, DataTypes...> type;
};

/**
 * @brief Provides a class to manage multiple related algorithms (i.e algorithms that support a common interface)
 * @details
 *   It combines the the panda::ConfigurableTask along with a user provided algorithm factory
 *   in order to provide algorithm selection, lifetime management, and execution of the algorithms.
 *
 * @tparam ModuleTraits
 *    A struct containing the following typedefs
 *    typedef std::tuple<DataTypes...> Signatures; // where DataTypes meet the criteria defined in panda::ConfigurableTask
 *
 */

template<typename ModuleTraits>
class AlgoModule
{
        /**
         * @brief the pool type to submit jobs to
         */
        typedef typename ModuleTraits::PoolType PoolType;

        /**
         * @brief a std::tuple type containing the available Algorithm types
         */
        typedef typename ModuleTraits::Algos Algos;

        /**
         * @brief a functor that can accept all return types from execution of the Algos
         */
        typedef typename ModuleTraits::Handler Handler;

        /**
         * @brief Generator of the Algos objects of the types defined in Algos tuple
         * @details for each Algo in Algos tuple there should be methods:
         * @code
         *      template<class Algo>  bool active() const;
         *      template<class Algo>  Algo create() const;
         * @endcode
         *
         */
        typedef typename ModuleTraits::AlgoFactory AlgoFactory;

        /**
         * @brief The type of ConfigurableTask object that is deployed to meet the ModuleTraits
         */
        typedef typename AlgoModuleTaskType<PoolType
                                          , Handler
                                          , typename ModuleTraits::Signatures>::type TaskType;

    public:
        template<typename... ConfigurableTaskFunctorInitialisers>
        AlgoModule(PoolType pool, AlgoFactory const&, Handler handler, ConfigurableTaskFunctorInitialisers&&...);

        /**
         * @brief asyncronous call to the activated algorithm, submitting a job to the pool for a resource
         */
        template<typename... DataT>
        std::shared_ptr<panda::ResourceJob> operator()(DataT&&... data) const;

        /**
         * @brief asyncronous call to the activated algorithm, submitting a job to the pool for a resource
         */
        template<typename... DataT>
        std::shared_ptr<panda::ResourceJob> submit(DataT&&... data) const;

        /**
         * @brief syncronous call to the activated algorithm suitable for the device
         */
        template<class ArchT, typename... DataT>
        void operator()(panda::PoolResource<ArchT>&, DataT&&... data) const;

    protected:
        /**
         * @brief direct access to the ConfigurableTask object
         */
        TaskType const& configured_task() const;

    private: // members
        mutable TaskType _task;
};


} // namespace utils
} // namespace cheetah
} // namespace ska
#include "detail/AlgoModule.cpp"

#endif // SKA_CHEETAH_UTILS_ALGOMODULE_H
