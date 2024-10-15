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
#ifndef SKA_CHEETAH_MODULES_DDTR_TEST_DDTRTESTER_H
#define SKA_CHEETAH_MODULES_DDTR_TEST_DDTRTESTER_H

#include "cheetah/generators/pulse_profile/ProfileManager.h"
#include "cheetah/modules/ddtr/Ddtr.h"
#include "cheetah/utils/test_utils/AlgorithmTester.h"
#include "cheetah/pipelines/search_pipeline/BeamConfig.h"
#include "panda/ResourcePool.h"
#include "panda/test/TestResourcePool.h"
#include "panda/test/TestHandler.h"
#include "panda/test/TestPoolManager.h"
#include <gtest/gtest.h>
#include <vector>

namespace ska {
namespace cheetah {
namespace modules {
namespace ddtr {
namespace test {

/**
 * @brief
 * Generic functional test for the Ddtr algorithm
 *
 * @detail
 * To use you will first need to create suitable Traits descriptions of the algorithm and the
 * hardware requirements it needs to run.
 *
 *  e.g.
 * @code
 * struct MyAlgoTraits1 : public DdtrTesterTraits<MyAlgoTemplate, NumericalT> {
 *      /// wrapper to execute the algorithm on requested provided device
 *      /// @return A dataype matching the tester requirements to verify the results
 *      ResultsType apply_algorithm(DeviceType&) const;
 * };
 * @endcode
 * Instantiate your algorithm tests by constructing suitable @class AlgorithmTestTraits classes
 * and then instantiate them with the INSTANTIATE_TYPED_TEST_SUITE_P macro
 *
 *  e.g.
 * @code
 * typedef ::testing::Types<MyAlgoTraits1, MyAlgoTraits2> MyTypes;
 * INSTANTIATE_TYPED_TEST_SUITE_P(MyAlgo, DdtrTester, MyTypes);
 * @endcode
 *
 *  n.b. the INSTANTIATE_TYPED_TEST_SUITE_P must be in the same namespace as this class
 *
 */

template<class DdtrAlgo>
struct DdtrTesterTraitsBase : public utils::test::PoolAlgorithmTesterTraits<typename panda::AlgorithmInfo<DdtrAlgo>::Architecture, typename panda::AlgorithmInfo<DdtrAlgo>::ArchitectureCapability>
{
    typedef DdtrAlgo Algo;
};

template<template<typename> class DdtrAlgoT, typename NumericalT>
struct DdtrTesterTraits :
    DdtrTesterTraitsBase<DdtrAlgoT<ddtr::CommonTypes<pipelines::search_pipeline::BeamConfig<NumericalT>, ddtr::Config
                                                   , NumericalT
                                                   >
                                 >
                        >
{
    public:
        typedef DdtrTesterTraitsBase<DdtrAlgoT<ddtr::CommonTypes<pipelines::search_pipeline::BeamConfig<NumericalT>, ddtr::Config
                                                   , NumericalT
                                                   >
                                 >
                        > BaseT;
        typedef typename BaseT::Algo DdtrAlgo;
        typedef typename panda::AlgorithmInfo<DdtrAlgo>::Architecture Arch;
        typedef typename BaseT::PoolType PoolType;
        typedef typename DdtrAlgo::NumericalRep NumericalRep;
        typedef typename ddtr::DedispersionConfig::Dm Dm;
        typedef typename DdtrAlgo::TimeFrequencyType TimeFrequencyType;

    protected:

        struct TestConfig : public ddtr::Config
        {
            typedef typename DdtrTesterTraits<DdtrAlgoT, NumericalRep>::PoolType PoolType;

            public:
                TestConfig() : _pool(nullptr) {}
                PoolType& pool() const { assert(_pool); return *_pool; }
                void set_pool(PoolType& pool) { _pool = &pool; }

            protected:
                PoolType* _pool;
        };

        struct TestBeamConfig : public pipelines::search_pipeline::BeamConfig<NumericalRep>
        {
            typedef typename DdtrTesterTraits<DdtrAlgoT, NumericalRep>::PoolType PoolType;

            public:
                TestBeamConfig() : _pool(nullptr) {}
                PoolType& pool() const { assert(_pool); return *_pool; }
                void set_pool(PoolType& pool) { _pool = &pool; }

            protected:
                PoolType* _pool;
        };


    public:
        typedef ddtr::Ddtr<TestBeamConfig, TestConfig, NumericalRep> Api;
        typedef typename Api::DmTrialsType DmType;
        typedef typename std::vector<std::shared_ptr<DmType>> DmDataContainerType;
        typedef typename Api::BeamConfigType BeamConfigType;
    private:
        struct DdtrHandler : public panda::test::TestHandler
        {
                typedef panda::test::TestHandler BaseT;

            public:
                DdtrHandler(DmDataContainerType& dm_data, std::size_t& dm_call_count)
                    : BaseT(false)
                    , _dm_data(dm_data)
                    , _dm_call_count(dm_call_count)
                {}
                void operator()(std::shared_ptr<DmType> data)
                {
                    PANDA_LOG_DEBUG << "DM handler called";
                    _dm_data.push_back(data);
                    ++_dm_call_count;
                    BaseT::operator()();
                }

            private:
                DmDataContainerType& _dm_data;
                std::size_t& _dm_call_count;
        };

    public:
        DdtrTesterTraits();
        DdtrTesterTraits(DdtrTesterTraits const&) = delete;
        Api& api(PoolType& pool);
        ddtr::Config& config();

        /// return true if the dm_handler has been called
        std::size_t dm_handler_call_count() const;
        generators::ProfileManager const& profile_manager() const { return _manager; }
        DmDataContainerType const& dm_data() const;

    protected:
        virtual void configure(ddtr::Config&) {}

    public:
        /**
        * @brief To check if the algorithm is expected pass for a given TimeFrequency block
        */
        virtual bool expected_to_pass(TimeFrequencyType const& ) { return true;}
        DdtrHandler& handler() {return _handler; }

    protected:
        TestConfig _config;
        TestBeamConfig _beam_config;
        std::size_t _dm_call_count;
        DmDataContainerType _dm_data;
        std::unique_ptr<Api> _api; // must be last member
        generators::ProfileManager _manager;
        DdtrHandler _handler;
};


template <typename TestTraits>
class DdtrTester : public cheetah::utils::test::AlgorithmTester<TestTraits>
{
    protected:
        void SetUp();
        void TearDown();

    public:
        DdtrTester();
        ~DdtrTester();
};

TYPED_TEST_SUITE_P(DdtrTester);

} // namespace test
} // namespace ddtr
} // namespace modules
} // namespace cheetah
} // namespace ska
#include "cheetah/modules/ddtr/test_utils/detail/DdtrTester.cpp"


#endif // SKA_CHEETAH_MODULES_DDTR_TEST_DDTRTESTER_H
