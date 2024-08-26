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
#include "cheetah/pipelines/search_pipeline/test/PipelineHandlerFactoryTest.h"
#include "cheetah/pipelines/search_pipeline/PipelineHandlerFactory.h"
#include "cheetah/pipelines/search_pipeline/CheetahConfig.h"
#include "cheetah/pipelines/search_pipeline/BeamConfig.h"

namespace ska {
namespace cheetah {
namespace pipelines {
namespace search_pipeline {
namespace test {


PipelineHandlerFactoryTest::PipelineHandlerFactoryTest()
    : ::testing::Test()
{
}

PipelineHandlerFactoryTest::~PipelineHandlerFactoryTest()
{
}

void PipelineHandlerFactoryTest::SetUp()
{
}

void PipelineHandlerFactoryTest::TearDown()
{
}

TEST_F(PipelineHandlerFactoryTest, test_create)
{
    search_pipeline::CheetahConfig<uint8_t> config;
    BeamConfig<uint8_t> beam_config;
    search_pipeline::PipelineHandlerFactory factory(config);
    for(auto const& handler : factory.available() ) {
        std::cout << "testing handler '" << handler << "'" << std::endl;
        try {
            auto type = factory.create(handler, beam_config);
            ASSERT_NE( nullptr, type ) << handler;
            delete type;
        } catch (panda::Error &) {}

        std::cout << "testing timed handler '" << handler << "'" << std::endl;
        try {
            auto timed_type = factory.create_timed(handler, beam_config);
            ASSERT_NE( nullptr, timed_type ) << handler;
            delete timed_type;
        } catch (panda::Error &) {}
    }
}

TEST_F(PipelineHandlerFactoryTest, test_tdas_config)
{
    search_pipeline::CheetahConfig<uint8_t> config;
    BeamConfig<uint8_t> beam_config;
    search_pipeline::PipelineHandlerFactory factory(config);

    // Add DM ranges for the dedispersion
    modules::ddtr::Config& ddtr_config = config.ddtr_config();
    typedef typename modules::ddtr::DedispersionConfig::Dm Dm;

    ddtr_config.add_dm_range(Dm(0.0 * data::parsec_per_cube_cm)
                            ,Dm(500.0 * data::parsec_per_cube_cm)
                            ,Dm(10.0 * data::parsec_per_cube_cm));

    search_pipeline::PipelineHandlerFactory newFactory(config);
}

} // namespace test
} // namespace search_pipeline
} // namespace pipelines
} // namespace cheetah
} // namespace ska
