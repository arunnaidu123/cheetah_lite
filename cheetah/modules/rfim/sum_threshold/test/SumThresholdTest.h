#ifndef SKA_CHEETAH_MODULES_RFIM_SUM_THRESHOLD_TEST_SUMTHRESHOLDTEST_H
#define SKA_CHEETAH_MODULES_RFIM_SUM_THRESHOLD_TEST_SUMTHRESHOLDTEST_H

#include <gtest/gtest.h>

namespace ska {
namespace cheetah {
namespace modules {
namespace rfim {
namespace sum_threshold {
namespace test {

/**
 * @brief
 *
 * @details
 *
 */

class SumThresholdTest : public ::testing::Test
{
    protected:
        void SetUp() override;
        void TearDown() override;

    public:
        SumThresholdTest();

        ~SumThresholdTest();

    private:
};


} // namespace test
} // namespace sum_threshold
} // namespace rfim
} // namespace modules
} // namespace cheetah
} // namespace ska

#endif // SKA_CHEETAH_MODULES_RFIM_SUM_THRESHOLD_TEST_SUMTHRESHOLDTEST_H