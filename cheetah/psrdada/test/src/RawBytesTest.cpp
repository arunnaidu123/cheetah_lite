#include "cheetah/psrdada/test/RawBytesTest.h"

namespace ska {
namespace cheetah {
namespace psrdada {
namespace detail {
namespace test {

RawBytesTest::RawBytesTest()
    : ::testing::Test()
{
}

RawBytesTest::~RawBytesTest()
{
}

void RawBytesTest::SetUp()
{
}

void RawBytesTest::TearDown()
{
}

TEST_F(RawBytesTest, test_total_and_used_bytes)
{
    RawBytes bytes(NULL,80ull,33ull);
    ASSERT_EQ(bytes.total_bytes(), 80ull);
    ASSERT_EQ(bytes.used_bytes(), 33ull);
    bytes.used_bytes(44ull);
    ASSERT_EQ(bytes.used_bytes(), 44ull);
}

TEST_F(RawBytesTest, test_ptr)
{
    char data[80ull];
    RawBytes bytes(data,80ull,33ull);
    ASSERT_EQ(bytes.ptr(), data);
}

} // namespace test
} // namespace detail
} // namespace psrdada
} // namespace cheetah
} // namespace ska
