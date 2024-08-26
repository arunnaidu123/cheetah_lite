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
#include "cheetah/pipelines/search_pipeline/test/CheetahConfigTest.h"
#include "cheetah/pipelines/search_pipeline/CheetahConfig.h"

#include "panda/test/TestFile.h"

namespace ska {
namespace cheetah {
namespace pipelines {
namespace search_pipeline {
namespace test {


CheetahConfigTest::CheetahConfigTest()
    : ::testing::Test()
{
}

CheetahConfigTest::~CheetahConfigTest()
{
}

void CheetahConfigTest::SetUp()
{
}

void CheetahConfigTest::TearDown()
{
}

TEST_F(CheetahConfigTest, test_pipeline_handlers_unset)
{
    search_pipeline::CheetahConfig<uint8_t> config;
    ASSERT_EQ("", config.pipeline_name()); // check there is an empty pipeline default

    // test --pipeline option is set
    char cmd[] = "cheetah_test";
    char pipeline_option[] = "--pipeline";
    char option[]="test_a";
    char* argv[3] = { cmd, pipeline_option, option };
    ASSERT_EQ(0, config.parse(3, argv));
    config.parse(3, argv);

}

TEST_F(CheetahConfigTest, test_pipeline_handlers_set)
{
    search_pipeline::CheetahConfig<uint8_t> config;
    std::vector<std::string> handlers( { "test_a", "test_b", "test_c" } );
    config.set_pipeline_handlers(handlers);

    // test --pipeline option
    char cmd[] = "cheetah_test";
    char pipeline_option[] = "--pipeline";
    char option[10];
    char* argv[3] = { cmd, pipeline_option, option };
    for( auto const& handler : handlers ) {
        strcpy(option, handler.c_str());
        ASSERT_EQ(0, config.parse(3, argv));
        ASSERT_EQ(handler, config.pipeline_name()); // check pipeline is set as we expect
    }
}

TEST_F(CheetahConfigTest, test_parse_no_arguments)
{
    char** argv = nullptr;
    search_pipeline::CheetahConfig<uint8_t> config;
    ASSERT_NO_THROW(config.parse(0, argv));

    ASSERT_NE("", config.version()); // check there is a non empty version string
    ASSERT_FALSE(config.time_handler_invocation()); // check timers are off by default
    ASSERT_EQ("", config.pipeline_name()); // check pipeline no set
    ASSERT_NE("", config.stream_name()); // check there is a no stream default
}

TEST_F(CheetahConfigTest, test_specify_non_existing_config_file)
{
    char cmd[] = "cheetah_test";
    char config_option[] = "--config";
    char empty_config[10] ="./rubbish";
    char* argv[3] = { cmd, config_option, empty_config };
    search_pipeline::CheetahConfig<uint8_t> config;
    ASSERT_THROW(config.parse(3, argv), panda::Error);
}

TEST_F(CheetahConfigTest, test_specify_existing_config_file)
{
    char cmd[] = "cheetah_test";
    char config_option[] = "--config";
    panda::test::TestFile config_file("%%%%-%%%%-%%%%-%%%%.xml");

    char config_filename[32];
    strcpy(config_filename, config_file.filename().c_str());
    char* argv[3] = { cmd, config_option, config_filename };
    search_pipeline::CheetahConfig<uint8_t> config;
    std::vector<std::string> handlers( { "test_a", "test_b", "test_c" } );
    config.set_pipeline_handlers(handlers);
    ASSERT_THROW(config.parse(3, argv), panda::Error); // invalid  (empty) config file

    // add some content
    config_file << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
    config_file << "<cheetah>\n";
    config_file << "<pipeline>" << "test_b" << "</pipeline>\n";
    config_file << "</cheetah>\n";
    config_file.flush();
    ASSERT_EQ("", config.pipeline_name()); // check pipeline no set
    config.parse(3, argv);
    ASSERT_EQ("test_b", config.pipeline_name()); // check pipeline no set

    // assert command line options override config file settings
    char pipeline_option[] = "--pipeline";
    char option[]="test_a";
    char* argv_2[5] = { cmd, pipeline_option, option, config_option, config_filename };
    config.parse(3, argv_2);
    ASSERT_EQ("test_a", config.pipeline_name());
}

} // namespace test
} // namespace search_pipeline
} // namespace pipelines
} // namespace cheetah
} // namespace ska
