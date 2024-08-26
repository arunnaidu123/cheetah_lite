#ifndef SKA_CHEETAH_DATA_DMTIMEBUFFERTEST_H
#define SKA_CHEETAH_DATA_DMTIMEBUFFERTEST_H

#include <gtest/gtest.h>

namespace ska {
namespace cheetah {
namespace data {
namespace test {

template <typename Arch, typename T>
struct DmTimeTestTraits
{
    typedef Arch Architecture;
    typedef T ValueType;
};

template <typename Traits>
class DmTimeTest : public ::testing::Test
{
    protected:
        void SetUp() override;
        void TearDown() override;

    public:
        DmTimeTest();
        ~DmTimeTest();

    private:
};

template <typename Arch, typename ValueType>
struct Tester
{
    static void slice_test(std::size_t,std::size_t,std::size_t);
    static void add_test();
};

} // namespace test
} // namespace data
} // namespace cheetah
} // namespace ska

#include "cheetah/data/test_utils/detail/DmTimeTest.cpp"

#endif // SKA_CHEETAH_DATA_DMTIMEBUFFERTEST_H