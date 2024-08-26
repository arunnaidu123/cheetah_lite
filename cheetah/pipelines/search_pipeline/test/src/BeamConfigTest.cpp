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
#include "cheetah/pipelines/search_pipeline/test/BeamConfigTest.h"
#include "cheetah/pipelines/search_pipeline/BeamConfig.h"
#include  <boost/property_tree/xml_parser.hpp>


namespace ska {
namespace cheetah {
namespace pipelines {
namespace search_pipeline {
namespace test {


BeamConfigTest::BeamConfigTest()
    : ::testing::Test()
{
}

BeamConfigTest::~BeamConfigTest()
{
}

void BeamConfigTest::SetUp()
{
}

void BeamConfigTest::TearDown()
{
}

TEST_F(BeamConfigTest, test_parse)
{
    BeamConfig<uint8_t> config;

    boost::property_tree::ptree pt_sigproc;
    std::string sigproc_filename("sigproc_file.fil");
    pt_sigproc.put<std::string>("file",sigproc_filename);

    boost::property_tree::ptree source;
    source.add_child("sigproc",pt_sigproc);

    boost::property_tree::ptree pt_beam;
    pt_beam.put<std::string>("active", "false");
    pt_beam.add_child("source", source);

    boost::property_tree::xml_parser::write_xml(std::cout, pt_beam);

    boost::program_options::variables_map vm;
    config.parse_property_tree(pt_beam, vm);
    boost::program_options::notify(vm);

    ASSERT_EQ(config.active(), false);
    ASSERT_EQ(1U, config.sigproc_config().sigproc_files().size());
    ASSERT_EQ(config.sigproc_config().sigproc_files()[0], sigproc_filename);
}

} // namespace test
} // namespace search_pipeline
} // namespace pipelines
} // namespace cheetah
} // namespace ska
