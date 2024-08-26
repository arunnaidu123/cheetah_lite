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
#include "cheetah/utils/test/TaskConfigurationSetterTest.h"
#include "cheetah/utils/TaskConfigurationSetter.h"
#include "panda/ConfigurableTask.h"
#include "panda/test/TestHandler.h"


namespace ska {
namespace cheetah {
namespace utils {
namespace test {


TaskConfigurationSetterTest::TaskConfigurationSetterTest()
    : ::testing::Test()
{
}

TaskConfigurationSetterTest::~TaskConfigurationSetterTest()
{
}

void TaskConfigurationSetterTest::SetUp()
{
}

void TaskConfigurationSetterTest::TearDown()
{
}

namespace {
struct Arch1 {};
struct Arch2 {};

struct Algo1 {
    typedef Arch1 Architecture;
};
struct Algo2 {
    typedef Arch2 Architecture;
};

class TaskTestFactory {
    public:
        template<typename T>
        T create() {
            return T();
        }

        template<typename Algo>
        bool active() const
        {
            return true;
        }
};

class DummyPool {
    public:
        typedef std::tuple<> Architectures;

        template<typename... Ts>
        std::shared_ptr<panda::ResourceJob> submit(Ts&&...)
        {
            return std::shared_ptr<panda::ResourceJob>();
        }
};

struct TestData {
};

} //namespace

TEST_F(TaskConfigurationSetterTest, test_one)
{
    typedef panda::ConfigurableTask<DummyPool, panda::test::TestHandler, TestData> Task;
    DummyPool pool;
    panda::test::TestHandler handler;
    Task task(pool, handler);
    TaskTestFactory factory;
    bool rv = utils::TaskConfigurationSetter<Algo1, Algo2>::configure(task, factory);
    ASSERT_TRUE(rv);
}

} // namespace test
} // namespace utils
} // namespace cheetah
} // namespace ska
