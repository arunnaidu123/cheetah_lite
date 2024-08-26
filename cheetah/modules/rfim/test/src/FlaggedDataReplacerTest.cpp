/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2024 The SKA organisation
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
#include "cheetah/modules/rfim/test/FlaggedDataReplacerTest.h"
#include "cheetah/modules/rfim/FlaggedDataReplacer.h"
#include "cheetah/data/TimeFrequency.h"
#include "cheetah/data/RfimFlaggedData.h"
#include <algorithm>

namespace ska {
namespace cheetah {
namespace modules {
namespace rfim {
namespace test{

FlaggedDataReplacerTest::FlaggedDataReplacerTest()
{
}

FlaggedDataReplacerTest::~FlaggedDataReplacerTest()
{
}

void FlaggedDataReplacerTest::SetUp()
{
}

void FlaggedDataReplacerTest::TearDown()
{
}

TEST_F(FlaggedDataReplacerTest, test_zero_return)
{
    /*
    * Test if all the values returned from the FlaggedDataReplacer class are zero
    * if the input TimeFrequency Data is zero
    */
    std::shared_ptr<data::TimeFrequency<Cpu, unsigned char>> data_ptr(new data::TimeFrequency<Cpu, unsigned char>(
                                                                        data::DimensionSize<data::Time>(20)
                                                                        , data::DimensionSize<data::Frequency>(10))
                                                                    );
    std::fill(data_ptr->begin(), data_ptr->end(), 0);
    unsigned char n=0;
    FlaggedDataReplacer<data::TimeFrequency<Cpu, unsigned char>> noise;
    noise.update_stats((*data_ptr));
    for(size_t i=0;i<1024;++i)
    {
        ASSERT_EQ(*(++noise),n);
    }
}

TEST_F(FlaggedDataReplacerTest, test_correct_numerical_return_type)
{
    /*
    * Test if the return type from FlaggedDataReplacer class is as expected
    * Performing tests for uint8_t and float
    */
    std::shared_ptr<data::TimeFrequency<Cpu, uint8_t>> data_ptr(new data::TimeFrequency<Cpu, uint8_t>(
                                                                    data::DimensionSize<data::Time>(20)
                                                                    , data::DimensionSize<data::Frequency>(10))
                                                                );
    std::shared_ptr<data::TimeFrequency<Cpu, float>> f_data_ptr(new data::TimeFrequency<Cpu, float>(
                                                                    data::DimensionSize<data::Time>(20)
                                                                    , data::DimensionSize<data::Frequency>(10))
                                                                );
    std::fill(data_ptr->begin(), data_ptr->end(), 0);
    std::fill(f_data_ptr->begin(), f_data_ptr->end(), 0);

    uint8_t n=0;
    FlaggedDataReplacer<data::TimeFrequency<Cpu, unsigned char>> noise;
    noise.update_stats((*data_ptr));

    float f=0;
    FlaggedDataReplacer<data::TimeFrequency<Cpu, float>> f_noise;
    f_noise.update_stats((*f_data_ptr));

    for(size_t i=0;i<1024;++i)
    {
        ASSERT_EQ(typeid(n),typeid(*noise));
        ASSERT_EQ(typeid(f),typeid(*f_noise));
    }
}

TEST_F(FlaggedDataReplacerTest, test_incorrect_numerical_return_type)
{
    /*
    * Testing the return type of the dereferencing operator
    */
    std::shared_ptr<data::TimeFrequency<Cpu, uint8_t>> data_ptr(new data::TimeFrequency<Cpu, uint8_t>(
                                                                    data::DimensionSize<data::Time>(20)
                                                                    , data::DimensionSize<data::Frequency>(10))
                                                                );
    std::fill(data_ptr->begin(), data_ptr->end(), 0);
    float n=0;
    FlaggedDataReplacer<data::TimeFrequency<Cpu, unsigned char>> noise;
    noise.update_stats((*data_ptr));
    for(size_t i=0;i<1024;++i)
    {
        ASSERT_FALSE(typeid(n)==typeid(*noise));
    }
}

TEST_F(FlaggedDataReplacerTest, test_increment_operator)
{
    int _mean = 96;
    int _rms = 24;
    std::shared_ptr<data::TimeFrequency<Cpu, uint8_t>> data_ptr(new data::TimeFrequency<Cpu, uint8_t>(
                                                                    data::DimensionSize<data::Time>(2048)
                                                                    , data::DimensionSize<data::Frequency>(1024))
                                                                );
    static std::random_device rd;
    std::seed_seq seed {  rd(), rd(), rd(), rd(), rd() };
    auto e2 = std::mt19937( seed );
    auto distribution = std::normal_distribution<float>(_mean,_rms);
    std::generate((*data_ptr).begin(),(*data_ptr).end(), std::bind(distribution, e2));

    FlaggedDataReplacer<data::TimeFrequency<Cpu, unsigned char>> noise;
    noise.update_stats((*data_ptr));

    //copying all the values of the vector
    std::vector<uint8_t> noise_buffer;
    noise_buffer.resize(8192);
    std::vector<uint8_t>::iterator it;
    for(it=noise_buffer.begin(); it!=noise_buffer.end(); ++it)
    {
        *it = *(++noise);
    }

    for(it=noise_buffer.begin();it!=noise_buffer.end(); ++it)
    {
        ASSERT_EQ(*it,*(++noise));
    }
}

TEST_F(FlaggedDataReplacerTest, test_dereferencing_operator)
{
    /*
    * Testing the dereference operator from FlaggedDataReplacer class
    */
    std::shared_ptr<data::TimeFrequency<Cpu, uint8_t>> data_ptr(new data::TimeFrequency<Cpu, uint8_t>(
                                                                    data::DimensionSize<data::Time>(2048)
                                                                    , data::DimensionSize<data::Frequency>(1024))
                                                                );
    std::fill(data_ptr->begin(), data_ptr->end(), 0);

    FlaggedDataReplacer<data::TimeFrequency<Cpu, unsigned char>> noise;
    noise.update_stats((*data_ptr));
    ASSERT_NO_THROW(*noise);
}

TEST_F(FlaggedDataReplacerTest, test_stats_of_generated_noise)
{
    /*
    * Generating gaussian noise samples from a fixed Mean and Standard Deviation.
    * Using generated TimeFrequency block as input to the FlaggedDataReplacer class
    * and checking the statistics of a set of number of samples
    */
    float _mean = 96;
    float _rms = 24;
    std::shared_ptr<data::TimeFrequency<Cpu, uint8_t>> data_ptr(new data::TimeFrequency<Cpu, uint8_t>(
                                                                    data::DimensionSize<data::Time>(2048)
                                                                    , data::DimensionSize<data::Frequency>(1024))
                                                                );
    static std::random_device rd;
    std::seed_seq seed {  rd(), rd(), rd(), rd(), rd() };
    auto e2 = std::mt19937( seed );
    auto distribution = std::normal_distribution<float>(_mean,_rms);
    std::generate((*data_ptr).begin(),(*data_ptr).end(), std::bind(distribution, e2));

    FlaggedDataReplacer<data::TimeFrequency<Cpu, unsigned char>> noise;
    noise.update_stats((*data_ptr));
    float calc_mean = 0;
    float calc_rms = 0;
    size_t n = 2048;
    for(size_t i=0;i<n;++i)
    {
        calc_mean += ((float)*(++noise))/n;
    }
    for(size_t i=0;i<n;++i)
    {
        calc_rms += std::pow((float)*(++noise)-calc_mean,2);
    }
    calc_rms = std::sqrt(calc_rms/n);

    bool flag_mean = false;
    if(calc_mean>(float)_mean*0.95 && calc_mean<(float)_mean*1.05)
    {
        flag_mean = true;
    }
    ASSERT_TRUE(flag_mean);

    bool flag_rms = false;
    if(calc_rms>(float)_rms*0.90 && calc_rms<(float)_rms*1.10)
    {
        flag_rms = true;
    }
    ASSERT_TRUE(flag_rms);
}

} // namespace test
} // namespace rfim
} // namespace modules
} // namespace cheetah
} // namespace ska
