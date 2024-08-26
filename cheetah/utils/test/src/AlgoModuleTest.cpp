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
#include "cheetah/utils/test/AlgoModuleTest.h"
#include "cheetah/utils/AlgoModule.h"
#include "panda/test/TestHandler.h"
#include "panda/test/TestPoolManager.h"
#include "panda/test/TestArch.h"
#include <typeinfo>
#include <typeindex>


namespace ska {
namespace cheetah {
namespace utils {
namespace test {


AlgoModuleTest::AlgoModuleTest()
    : ::testing::Test()
{
}

void AlgoModuleTest::SetUp()
{
}

void AlgoModuleTest::TearDown()
{
}

struct AlgoModuleTestFactory
{
    template<typename Algo>
    Algo create() const
    {
        _called[std::type_index(typeid(Algo))] = true;
        return Algo();
    }

    template<class Algo>
    bool active() const { return true; }

    template<class Algo>
    bool called() const { return _called.find(std::type_index(typeid(Algo))) != _called.end(); }

    private:
        mutable std::map<std::type_index, bool> _called;
};

template<typename Arch>
class TestAlgo : public panda::test::TestHandler
{
        typedef panda::test::TestHandler BaseT;

    public:
        TestAlgo()
            : BaseT(false)
        {
        }

        typedef Arch Architecture;
        template<typename...Ts>
        void operator()(Ts&&...) {
            this->BaseT::operator()();
        }
};

class TestHandler : public panda::test::TestHandler
{
        typedef panda::test::TestHandler BaseT;

    public:
        TestHandler() : BaseT(false)
        {}
};

struct ArchA {};
struct ArchB {};

template<typename... Archs>
struct TestAlgoModuleTraitsBase
{
    typedef AlgoModuleTestFactory AlgoFactory;
    typedef panda::test::TestPoolManager<Archs...> PoolType;
    typedef std::tuple<TestAlgo<Archs>...> Algos;
    typedef TestHandler Handler;
};

template<typename... Archs>
struct TestAlgoModuleTraits : public TestAlgoModuleTraitsBase<Archs...>
{
    typedef std::tuple<int> Signatures;
};

struct TestA {};

template<typename... Archs>
struct TestAlgoModuleTraitsMultiSig : public TestAlgoModuleTraitsBase<Archs...>
{
    typedef std::tuple<panda::SubmitMethod<TestA>, int> Signatures;
};


TEST_F(AlgoModuleTest, test_basic_operation_single_algo)
{
    TestHandler handler;
    panda::test::TestPoolManager<ArchA> pool;
    AlgoModuleTestFactory factory;
    pool.add_resource<ArchA>();

    AlgoModule<TestAlgoModuleTraits<ArchA>> mod(pool, factory, handler);
    int data =100;
    auto job = mod(data);

    handler.wait(std::chrono::seconds(5));
    job->wait();
}

TEST_F(AlgoModuleTest, test_basic_operation_single_algo_multiple_signatures)
{
    TestHandler handler;
    panda::test::TestPoolManager<ArchA> pool;
    AlgoModuleTestFactory factory;
    pool.add_resource<ArchA>();

    AlgoModule<TestAlgoModuleTraitsMultiSig<ArchA>> mod(pool, factory, handler);
    int data =100;
    auto job = mod(data);

    handler.wait(std::chrono::seconds(5));
    handler.reset();

    TestA data_2;
    mod(data_2);
    handler.wait(std::chrono::seconds(5));
}

TEST_F(AlgoModuleTest, test_basic_operation_multi_algo)
{
    TestHandler handler;
    panda::test::TestPoolManager<ArchA, ArchB> pool;
    AlgoModuleTestFactory factory;
    pool.add_resource<ArchA>();
    pool.add_resource<ArchB>();

    AlgoModule<TestAlgoModuleTraits<ArchA, ArchB>> mod(pool, factory, handler);
    int data =100;
    auto job = mod(data);

    handler.wait(std::chrono::seconds(5));
    job->wait();

    ASSERT_TRUE(factory.called<TestAlgo<ArchA>>());
    ASSERT_TRUE(factory.called<TestAlgo<ArchB>>());
}

} // namespace test
} // namespace utils
} // namespace cheetah
} // namespace ska
