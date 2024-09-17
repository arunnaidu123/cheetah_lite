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
#include "cheetah/pipelines/search_pipeline/test/MultiBeamConfigTest.h"
#include "cheetah/pipelines/search_pipeline/MultiBeamConfig.h"
#include  <boost/property_tree/xml_parser.hpp>

namespace ska {
namespace cheetah {
namespace pipelines {
namespace search_pipeline {
namespace test {


MultiBeamConfigTest::MultiBeamConfigTest()
    : ::testing::Test()
{
}

MultiBeamConfigTest::~MultiBeamConfigTest()
{
}

void MultiBeamConfigTest::SetUp()
{
}

void MultiBeamConfigTest::TearDown()
{
}

TEST_F(MultiBeamConfigTest, test_multiple_beam_config_parse)
{
    /*
    MultiBeamConfig<uint8_t> mb_config;
    std::vector<std::string> sigproc_filenames = { "sigproc_1.fil", "sigproc_2.fil" };

    // Make a property tree
    boost::property_tree::ptree pt_root;
    boost::property_tree::ptree pt_beams;

    boost::property_tree::ptree pt_beam_1;
    boost::property_tree::ptree pt_beam_2;
    boost::property_tree::ptree source_1;
    boost::property_tree::ptree source_2;
    boost::property_tree::ptree source_3;
    source_3.put<std::string>("id","source_3_id");

    // Sigproc files

    boost::property_tree::ptree pt_sigproc_1;
    pt_sigproc_1.put<std::string>("file",sigproc_filenames[0]);
    pt_sigproc_1.put<std::string>("id","sigproc_1_id");
    boost::property_tree::ptree pt_sigproc_2;
    pt_sigproc_2.put<std::string>("file",sigproc_filenames[1]);
    pt_sigproc_2.put<std::string>("id","sigproc_2_id");
    source_1.add_child("sigproc",pt_sigproc_1);
    source_2.add_child("sigproc", pt_sigproc_2);

    pt_beam_1.put<std::string>("active", "true");
    pt_beam_2.put<std::string>("active", "true");
    pt_beam_1.add_child("source", source_1);
    pt_beam_2.add_child("source", source_2);
    pt_beams.add_child("beam", pt_beam_1);
    pt_beams.add_child("beam", pt_beam_2);
    pt_root.put_child("beams", pt_beams);

    //boost::property_tree::xml_parser::write_xml(std::cout, pt_root);

    boost::program_options::variables_map vm;
    mb_config.parse_property_tree(pt_beams, vm);
    boost::program_options::notify(vm);

    auto it = mb_config.beams();
    unsigned count=0;
    while (it != mb_config.beams_end())
    {
        auto const& beam_config = *it;
        ASSERT_EQ(1U, beam_config.sigproc_config().sigproc_files().size());
        ASSERT_EQ(beam_config.sigproc_config().sigproc_files()[0], sigproc_filenames[count]);
        ++it;
        ++count;
    }
    ASSERT_EQ(count, 2U);
    */
}

} // namespace test
} // namespace search_pipeline
} // namespace pipelines
} // namespace cheetah
} // namespace ska
