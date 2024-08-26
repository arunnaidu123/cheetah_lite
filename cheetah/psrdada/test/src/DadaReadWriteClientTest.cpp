#include "cheetah/psrdada/test/DadaReadWriteClientTest.h"
#include "cheetah/psrdada/DadaWriteClient.h"
#include "cheetah/psrdada/DadaReadClient.h"
#include "cheetah/psrdada/test_utils/TestDadaDB.h"

#include <sstream>

namespace ska {
namespace cheetah {
namespace psrdada {
namespace test {

DadaReadWriteClientTest::DadaReadWriteClientTest()
    : ::testing::Test()
    , _engine(1)
{
}

DadaReadWriteClientTest::~DadaReadWriteClientTest()
{
}

void DadaReadWriteClientTest::SetUp()
{
}

void DadaReadWriteClientTest::TearDown()
{
}

TEST_F(DadaReadWriteClientTest, test_default_params)
{
    test_utils::TestDadaDB test_db(4, 4096, 4, 4096);
    test_db.create();
    {
        DadaWriteClient writer(test_db.key(), [](std::ostream&){});
        ASSERT_TRUE(writer.data_buffer_size()==test_db.data_buffer_size());
        ASSERT_TRUE(writer.data_buffer_count()==test_db.num_data_buffers());
        ASSERT_TRUE(writer.header_buffer_size()==test_db.header_buffer_size());
        ASSERT_TRUE(writer.header_buffer_count()==test_db.num_header_buffers());

        DadaReadClient reader(test_db.key(), _engine, [](std::istream&, std::exception_ptr){});
        ASSERT_TRUE(reader.data_buffer_size()==test_db.data_buffer_size());
        ASSERT_TRUE(reader.data_buffer_count()==test_db.num_data_buffers());
        ASSERT_TRUE(reader.header_buffer_size()==test_db.header_buffer_size());
        ASSERT_TRUE(reader.header_buffer_count()==test_db.num_header_buffers());
    }
    test_db.destroy();
}


TEST_F(DadaReadWriteClientTest, test_multiblock_readwrite)
{
    test_utils::TestDadaDB test_db(4, 4096, 4, 4096);
    test_db.create();
    std::string write_header("This is some header data!");
    {
        std::vector<char> input_data(8*1024+222);
        {
            DadaWriteClient writer(test_db.key(), [&](std::ostream& out){
                out << write_header;
            });
            auto it = input_data.begin();
            writer.write(it, input_data.end());
        }

        std::string read_header;
        DadaReadClient reader(test_db.key(), _engine, [&](std::istream& in, std::exception_ptr ){
            in >> read_header;
        });

        std::vector<char> output_data(8*1024+222);
        auto it = output_data.begin();
        reader.read(it, output_data.end());
        for (std::size_t jj=0; jj<input_data.size(); ++jj)
        {
            ASSERT_EQ(output_data[jj],input_data[jj]);
        }
    }
    test_db.destroy();
}

TEST_F(DadaReadWriteClientTest, test_header_overflow)
{
    test_utils::TestDadaDB test_db(1, 1, 1, 1);
    test_db.create();
    {
        std::string write_header("This is some header data!");
        ASSERT_ANY_THROW(DadaWriteClient writer(test_db.key(), [&](std::ostream& out){out << write_header;}));
    }
    test_db.destroy();
}

TEST_F(DadaReadWriteClientTest, test_multi_sequence_readwrite)
{
    test_utils::TestDadaDB test_db(16, 4096, 4, 4096);
    test_db.create();
    {
        std::size_t ii;
        std::size_t observation_id = 0;
        auto new_sequence_callback = [&](std::ostream& out)
        {
            out << "This is the header for observation ID " << observation_id << "\n";
            ++observation_id;
        };

        { // This is the writer scope
            DadaWriteClient writer(test_db.key(), new_sequence_callback);
            //Write one and a half data blocks and mark the EOD
            std::vector<float> data(4096+2048);
            for (ii=0; ii<data.size(); ++ii)
            {
                data[ii] = (float) (ii%255);
            }
            auto it = data.begin();
            writer.write(it,data.end());
            writer.new_sequence();
            it = data.begin();
            writer.write(it,data.end());
        }

        auto next_sequence_callback = [&](std::istream& stream, std::exception_ptr)
        {
            std::string tmp_string;
            while (!stream.eof())
            {
                stream >> tmp_string;
            }
        };

        { // This is the reader scope
            DadaReadClient reader(test_db.key(), _engine, next_sequence_callback);
            //Read partial data from first observation
            std::vector<float> partial_data(2048);
            auto it = partial_data.begin();
            reader.read(it, partial_data.end());
            for (ii=0; ii<partial_data.size(); ++ii)
            {
                ASSERT_EQ(partial_data[ii],(float) (ii%255));
            }
            reader.next_sequence();

            it = partial_data.begin();
            reader.read(it, partial_data.end());
            for (ii=0; ii<partial_data.size(); ++ii)
            {
                ASSERT_EQ(partial_data[ii],(float) (ii%255));
            }
        }

    }
    test_db.destroy();
}

} // namespace test
} // namespace psrdada
} // namespace cheetah
} // namespace ska
