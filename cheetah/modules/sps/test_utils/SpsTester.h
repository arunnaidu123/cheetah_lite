/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2022 The SKA organisation
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
#ifndef SKA_CHEETAH_MODULES_SPS_TEST_SPSTESTER_H
#define SKA_CHEETAH_MODULES_SPS_TEST_SPSTESTER_H

#include "cheetah/modules/sps/Sps.h"
#include "cheetah/utils/test_utils/AlgorithmTester.h"
#include "panda/ResourcePool.h"
#include "panda/test/TestResourcePool.h"
#include <gtest/gtest.h>
#include <deque>
#include <mutex>
#include <condition_variable>

namespace ska {
namespace cheetah {
namespace modules {
namespace sps {
namespace test {

/**
 * @brief
 * Generic functional test for the Sps algorithm
 *
 * @details
 * To use you will first need to create suitable Traits descriptions of the algorithm and the
 * hardware requiremnts it needs to run.
 *
 *  e.g.
 * @code
 * struct MyAlgoTraits1 : public SpsTesterTraits<ResourceCapabilities, Architecture> {
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
 * INSTANTIATE_TYPED_TEST_SUITE_P(MyAlgo, SpsTester, MyTypes);
 * @endcode
 *
 *  n.b. the INSTANTIATE_TYPED_TEST_SUITE_P must be in the same namespace as this class
 *
 */


template<typename ArchitectureTag, typename ArchitectureCapability>
struct SpsTesterTraits : public utils::test::PoolAlgorithmTesterTraits<ArchitectureTag, ArchitectureCapability>
{
        typedef utils::test::PoolAlgorithmTesterTraits<ArchitectureTag, ArchitectureCapability> BaseT;

    public:
        typedef ArchitectureTag Arch;
        typedef typename BaseT::PoolType PoolType;

    private:
        struct TestConfig : public sps::Config
        {
            typedef sps::Config BaseT;
            typedef typename SpsTesterTraits<ArchitectureTag, ArchitectureCapability>::PoolType PoolType;

            public:
                TestConfig()
                    : BaseT(_ddtr_config)
                    , _pool(nullptr) {}
                PoolType& pool() const { assert(_pool); return *_pool; }
                void set_pool(PoolType& pool) { _pool = &pool; }

            protected:
                PoolType* _pool;
                modules::ddtr::Config _ddtr_config;
        };

    public:
        typedef sps::Sps<TestConfig, uint8_t> Api;
        typedef typename Api::DmTrialType DmType;
        typedef typename Api::SpType SpType;

    public:
        SpsTesterTraits();
        Api& api(PoolType& pool);
        sps::Config& config();

        /// return true if the dm_handler has been called
        bool dm_handler_called() const;

        /// return true if the sp_handler has been called
        bool sp_handler_called() const;

        /// return true if the sp_handler has been called
        //  blocks until the sp_handler is called or cancelled
        //  returns true if called, false if cancelled due to time out
        bool wait_sp_handler_called() const;

        /// return true if the sp_handler has been called
        std::size_t sp_handler_call_count() const;

        std::shared_ptr<SpType> sp_data() const;

    protected:
        // override this function to set up specfic configuration parameters
        virtual void configure(sps::Config&) {}

    private:
        bool _dm_called;
        bool _sp_called;

    protected:
        TestConfig _config;

        std::shared_ptr<DmType> _dm_data;
        mutable std::deque<std::shared_ptr<SpType>> _sp_data;
        mutable std::mutex _sp_data_mutex;
        mutable std::condition_variable _sp_wait;
        std::size_t _sp_call_count;

        std::unique_ptr<Api> _api; // must be last member
};

template <typename TestTraits>
class SpsTester : public cheetah::utils::test::AlgorithmTester<TestTraits>
{
    protected:
        //typename sps::Sps<sps::Config, uint8_t>::BufferFillerType::AggregationBufferType BufferDataType;
        typename sps::Sps<sps::Config, uint8_t>::TimeFrequencyType DataType;

    protected:
        void SetUp();
        void TearDown();

     public:
        SpsTester();
        ~SpsTester();
};

TYPED_TEST_SUITE_P(SpsTester);

} // namespace test
} // namespace sps
} // namespace modules
} // namespace cheetah
} // namespace ska
#include "cheetah/modules/sps/test_utils/detail/SpsTester.cpp"


#endif // SKA_CHEETAH_MODULES_SPS_TEST_SPSTESTER_H
