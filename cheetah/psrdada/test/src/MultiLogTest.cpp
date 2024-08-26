#include "cheetah/psrdada/test/MultiLogTest.h"

namespace ska {
namespace cheetah {
namespace psrdada {
namespace detail {
namespace test {

MultiLogTest::MultiLogTest()
    : ::testing::Test()
{
}

MultiLogTest::~MultiLogTest()
{
}

void MultiLogTest::SetUp()
{
}

void MultiLogTest::TearDown()
{
}

TEST_F(MultiLogTest, test_name)
{
    MultiLog logger("TestLogger");
    ASSERT_TRUE(logger.name()=="TestLogger");
}

TEST_F(MultiLogTest, test_write)
{
    MultiLog logger("TestLogger");
    ASSERT_NO_THROW(logger.write(LOG_ERR, "This is test number %d\n",1));
}

TEST_F(MultiLogTest, test_valid_handle)
{
    MultiLog logger("TestLogger");
    ASSERT_FALSE(logger.native_handle()==0);
}

} // namespace test
} // namespace detail
} // namespace psrdada
} // namespace cheetah
} // namespace ska
