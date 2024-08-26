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
#include "cheetah/io/exporters/test/DataExportConfigTest.h"
#include "cheetah/io/exporters/DataExportConfig.h"
#include <boost/property_tree/xml_parser.hpp>

namespace ska {
namespace cheetah {
namespace io {
namespace exporters {
namespace test {


DataExportConfigTest::DataExportConfigTest()
    : ::testing::Test()
{
}

DataExportConfigTest::~DataExportConfigTest()
{
}

void DataExportConfigTest::SetUp()
{
}

void DataExportConfigTest::TearDown()
{
}

class Sink_1_Config : public cheetah::utils::Config {
  public:
    Sink_1_Config() : cheetah::utils::Config("sink_1") {}
  protected:
    void add_options(OptionsDescriptionEasyInit& ) override {}
};

struct Sink_1 {
    typedef Sink_1_Config ConfigType;
};

class Sink_2_Config : public cheetah::utils::Config {
  public:
    Sink_2_Config() : cheetah::utils::Config("sink_2") {}
  protected:
    void add_options(OptionsDescriptionEasyInit& ) override {};
};

struct Sink_2 {
    typedef Sink_2_Config ConfigType;
};

TEST_F(DataExportConfigTest, test_parse_tree_no_registered)
{
    boost::property_tree::ptree pt_root;
    boost::property_tree::ptree pt_exporter_1;
    boost::property_tree::ptree pt_exporter_2_a;
    boost::property_tree::ptree pt_exporter_2_b;

    pt_root.put_child("exporter_1", pt_exporter_1);
    pt_root.add_child("exporter_2", pt_exporter_2_a);
    pt_root.add_child("exporter_2", pt_exporter_2_b);

    exporters::DataExportConfig c;
    boost::program_options::variables_map vm;
    c.parse_property_tree(pt_root, vm);
    boost::program_options::notify(vm);

    ASSERT_EQ(0U, c.exporters().size()); // no data exporters registered

}

TEST_F(DataExportConfigTest, test_parse_tree)
{
    boost::property_tree::ptree pt_root;
    boost::property_tree::ptree pt_channels;
    //boost::property_tree::ptree pt_sinks;
    boost::property_tree::ptree pt_sink_configs;

    boost::property_tree::ptree pt_channel_1;
    boost::property_tree::ptree pt_channel_2;
    boost::property_tree::ptree sink_1;
    sink_1.put<std::string>("id","sink_1_id");
    boost::property_tree::ptree sink_2;
    sink_2.put<std::string>("id","sink_2_id");
    boost::property_tree::ptree sink_3;
    sink_3.put<std::string>("id","sink_3_id");
    boost::property_tree::ptree sink_4; // empty

    pt_channel_1.put<std::string>("active", "true");
    pt_channel_1.add_child("sink", sink_1);
    pt_channel_1.add_child("sink", sink_2);
    pt_channel_2.add_child("sink", sink_3);
    pt_channel_2.add_child("sink", sink_4);
    pt_channels.add_child("channel_1", pt_channel_1);
    pt_channels.add_child("channel_2", pt_channel_2);
    pt_root.put_child("channels", pt_channels);

    // add sink configurations
    boost::property_tree::ptree sink_1_config;
    boost::property_tree::ptree sink_1_with_id_config;
    sink_1_with_id_config.put<std::string>("id","sink_1_id");
    boost::property_tree::ptree sink_2_config;
    boost::property_tree::ptree sink_2_with_id_config;
    sink_2_with_id_config.put<std::string>("id","sink_2_id");
    //pt_sinks.add_child("sink_2", sink_2_config);
    //pt_sinks.add_child("sink_2", sink_2_config);
    //pt_root.put_child("sinks", pt_sinks);
    pt_sink_configs.add_child("sink_1", sink_1_config);
    pt_sink_configs.add_child("sink_1", sink_1_with_id_config);
    pt_sink_configs.add_child("sink_2", sink_2_config);
    pt_sink_configs.add_child("sink_2", sink_2_with_id_config);
    pt_root.put_child("sink_configs", pt_sink_configs);

    boost::property_tree::xml_parser::write_xml(std::cout, pt_root);

    exporters::DataExportConfig c;
    c.add_sink("sink_1", []() { return new Sink_1_Config(); });
    c.add_sink("sink_2", []() { return new Sink_2_Config(); });

    boost::program_options::variables_map vm;
    c.parse_property_tree(pt_root, vm);
    boost::program_options::notify(vm);

    // register some exporters
    ASSERT_EQ(2U, c.exporters().size());
    Sink_1_Config const& c1 = dynamic_cast<Sink_1_Config const&>(c.exporters()[0].sink_config());
    ASSERT_EQ("sink_1",c1.name());
    Sink_2_Config const& c2 = dynamic_cast<Sink_2_Config const&>(c.exporters()[1].sink_config());
    ASSERT_EQ("sink_2",c2.name());
}

} // namespace test
} // namespace exporters
} // namespace io
} // namespace cheetah
} // namespace ska
