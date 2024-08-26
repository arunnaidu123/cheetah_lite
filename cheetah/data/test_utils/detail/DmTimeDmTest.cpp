#include "cheetah/data/test_utils/DmTimeDmTest.h"
#include "cheetah/data/test_utils/DmTrialsGeneratorUtil.h"
#include "cheetah/data/DmTime.h"
#include "cheetah/data/DmTrialsMetadata.h"
#include "cheetah/data/DmTrials.h"
#include "cheetah/data/TimeSeries.h"
#include "panda/Log.h"

namespace ska {
namespace cheetah {
namespace data {
namespace test {

DmTimeDmTest::DmTimeDmTest()
    : ::testing::Test()
{
}

DmTimeDmTest::~DmTimeDmTest()
{
}


void DmTimeDmTest::SetUp()
{
}

void DmTimeDmTest::TearDown()
{
}

template <typename Arch>
struct CopyTest
{
    static void test(std::size_t nblocks, std::size_t samps_per_block, std::size_t timeseries_length)
    {
        typedef DmTrials<Cpu,float> DmTrialsType;
        typedef typename DmTrialsType::TimeType TimeType;
        typedef typename DmTrialsType::DmType Dm;
        auto buffer = DmTime<DmTrialsType>::make_shared();
        data::TimeSeries<Arch,float> timeseries(timeseries_length);
        DmTrialsGeneratorUtil<DmTrialsType> trials_generator;
        for (int block=0; block<nblocks; ++block)
        {
            buffer->add(trials_generator.generate(TimeType(0.000064*data::seconds),samps_per_block,3));
        }
        ASSERT_EQ(buffer->blocks().size(),nblocks);
        auto slice = *(buffer->begin(2));
        auto const& dm = *(slice->begin());
        auto end = dm.copy_to(timeseries);
        std::size_t diff = std::distance(timeseries.begin(),end);
        ASSERT_EQ(diff,std::min(nblocks*samps_per_block,timeseries.size()));
    }
};

} // namespace test
} // namespace data
} // namespace cheetah
} // namespace ska
