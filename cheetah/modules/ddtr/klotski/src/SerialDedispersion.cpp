/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016-2023 The SKA organisation
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

#include <vector>
#include <array>
#include <thread>
#include <cmath>
#include <iostream>

namespace ska {
namespace cheetah {
namespace modules {
namespace ddtr {
namespace klotski {

/**
 * @brief AVX method which performs dedispersion
 * @param variables all the local variables used
 * @param total_size total_size of the staxk used
 * @param data_in FT data block of the Klotski
 * @param data_out tempory output of the Dedispered data
 * @param counts_array total number of iterations per klotski
 * @param total_shift shift to be applied. data[index][i] = data[base][i] + data[index][i+shift]
 * @param total_index contains the information about the out put channel for each interation
 * @param total_base base channel for each iteration
 * @param dmindex_shifts essentially the DM shifts per klotski
 * @param start_dm_shifts inital DM shifts to be applied before reading in the data
 */
extern "C" void nasm_dedisperse( std::size_t* variables
                           , std::size_t total_size
                           , unsigned short *data_in
                           , int *data_out
                           , unsigned int* counts_array
                           , unsigned int* total_shift
                           , unsigned int* total_index
                           , unsigned int* total_base
                           , unsigned int* dmindex_shifts
                           , const unsigned int* start_dm_shifts
                           );

namespace {

/**
 * @brief method to estimate the variables to be stored in the cache.
 */
inline
void dedisperse_klotski_input_array(
                          std::vector<std::size_t>& variables
                        , std::size_t nchans
                        , std::size_t ksamps
                        , std::size_t tsamps
                        , std::size_t dsamps
                        , std::size_t ndms
                        , std::vector<unsigned int>& total_base
                        , std::vector<unsigned int>& total_index
                        , std::vector<unsigned int>& total_shift
                        , std::vector<unsigned int>& counts_array
                        , std::vector<unsigned int>& dmindex_shifts
                        , int dsamps_output
                       )
{
    std::size_t samps_per_iteration = 256;
    variables[0] = (ksamps+samps_per_iteration)*sizeof(short); //DATA_SIZE
    variables[1] = ksamps*nchans*sizeof(short); //OVERLAP_SIZE
    variables[2] = 4*samps_per_iteration*ndms; //DATAOUT_SIZE
    variables[3] = counts_array.size()*sizeof(int); //COUNTS_ARRAY_SIZE
    variables[4] = total_shift.size()*sizeof(int); //INDEX_SHIFTS_SIZE
    variables[5] = total_index.size()*sizeof(int); //INDEX_SIZE
    variables[6] = total_base.size()*sizeof(int); //BASE_SIZE
    variables[7] = samps_per_iteration*sizeof(int); //TEMP_SIZE
    variables[8] = dmindex_shifts.size()*sizeof(int); //DMINDEX_SHIFTS_SIZE
    variables[9] = nchans*sizeof(int); //START_DM_SHIFTS_SIZE
    variables[10] = tsamps*sizeof(short); //TSAMPS_SIZE
    variables[11] = 512; //DATA_LOCATION
    variables[12] = variables[11] + variables[0]; //OVERLAP_LOCATION
    variables[13] = variables[12] + variables[1]; //DATAOUT_LOCATION
    variables[14] = variables[13] + variables[2]; //COUNTS_ARRAY_LOCATION
    variables[15] = variables[14] + variables[3]; //INDEX_SHIFTS_LOCATION
    variables[16] = variables[15] + variables[4]; //INDEX_LOCATION
    variables[17] = variables[16] + variables[5]; //BASE_LOCATION
    variables[18] = variables[17] + variables[6]; //TEMP_LOCATION
    variables[19] = variables[18] + variables[7]; //DMINDEX_SHIFTS_LOCATION
    variables[20] = variables[19] + variables[8]; // START_DM_SHIFTS_LOCATION
    variables[21] = ndms; //NDMS
    variables[22] = tsamps; //TSAMPS
    variables[23] = dsamps-dsamps%samps_per_iteration; //DSAMPS
    variables[24] = ksamps; //NSAMPS
    variables[25] = 1; //NITER
    variables[26] = nchans; //NCHANS
    variables[27] = samps_per_iteration;
    variables[28] = samps_per_iteration/16;
    variables[29] = dsamps_output*sizeof(int); // DSAMPS_OUTPUT_SIZE
    variables[30] = (dsamps%samps_per_iteration)/32; // EDGE_SAMPLE
    variables[31] = variables[20] + variables[9]; //TOTAL_SIZE

}

} // namespace

/**
 * @brief calls the AVX method to perform dedispersion on a klotski
 * @param ksamps number of samples in the Klotski
 * @param tsamps number of samples in the FT block
 * @param dsamps dedispersed samples per Klotski
 * @param ndms number of Dms in the current dm range
 * @param data_in FT data block of the Klotski
 * @param data_temp tempory output of the Dedispered data
 * @param total_base base channel for each iteration
 * @param total_index contains the information about the out put channel for each interation
 * @param total_shift shift to be applied. data[index][i] = data[base][i] + data[index][i+shift]
 * @param counts_array total number of iterations per klotski
 * @param dmindex_shifts essentially the DM shifts per klotski
 * @param start_dm_shifts inital DM shifts to be applied before reading in the data
 * @param dsamps_output dedispersed samples in the klotski
 */

void dedisperse_klotski( std::size_t nchans
                    , std::size_t ksamps
                    , std::size_t tsamps
                    , std::size_t dsamps
                    , std::size_t ndms
                    , unsigned short* data_in
                    , int* data_temp
                    , std::vector<unsigned int>& total_base
                    , std::vector<unsigned int>& total_index
                    , std::vector<unsigned int>& total_shift
                    , std::vector<unsigned int>& counts_array
                    , std::vector<unsigned int>& dmindex_shifts
                    , const unsigned int* start_dm_shifts
                    , int dsamps_output
                    )
{
    // setup variables to pass to kernel
    std::vector<std::size_t> variables(32);
    dedisperse_klotski_input_array(variables, nchans, ksamps, tsamps, dsamps, ndms, total_base, total_index, total_shift, counts_array, dmindex_shifts, dsamps_output);

    nasm_dedisperse( &*variables.begin()
               , variables[31]
               , data_in
               , data_temp
               , &*counts_array.begin()
               , &*total_shift.begin()
               , &*total_index.begin()
               , &*total_base.begin()
               , &*dmindex_shifts.begin()
               , start_dm_shifts
               );
}


/**
 * @brief Serial AVX dedispersion. This method performs dedispersion of all the Klotskis in a given subband.
 * @param data_out output vector which contains the subbanded dedispersed data
 * @param data_in FT data in the form of std::vector
 * @param dsamps_per_klotski dedispersed samples per Klotski
 * @param nsamps number of samples in the Klotski
 * @param number_of_dms number of Dms in the current dm range
 * @param nchans number of channels in FT data block
 * @param dmindex_shifts essentially the DM shifts per klotski
 * @param total_base base channel for each iteration
 * @param total_index contains the information about the out put channel for each interation
 * @param total_shift shift to be applied. data[index][i] = data[base][i] + data[index][i+shift]
 * @param counts_array total number of iterations per klotski
 * @param start_dm_shifts inital DM shifts to be applied before reading in the data
 * @param channels_offset start channel for the subband
 */

int serial_dedispersion(std::vector<int>& data_out
                           , std::vector<unsigned short>& data_in
                           , std::vector<unsigned int> dsamps_per_klotski
                           , int nsamps
                           , int number_of_dms
                           , int max_channels_per_klotski
                           , int nchans
                           , std::vector<std::vector<unsigned int>> dmindex_shifts
                           , std::vector<std::vector<unsigned int>> total_base
                           , std::vector<std::vector<unsigned int>> total_index
                           , std::vector<std::vector<unsigned int>> total_shift
                           , std::vector<std::vector<unsigned int>> counts_array
                           , std::vector<unsigned int> const& start_dm_shifts
                           , unsigned channels_offset
                           )
{

    int number_of_subbands = nchans/max_channels_per_klotski;
    if(nchans%max_channels_per_klotski!=0) number_of_subbands+=1;
    unsigned int ksamps = 512; // TODO: mostly any efficieny factor need to investigate its relation with the hardware used.
                                //512 is a good ball park number for tengu and sharabha.

    for(int klotski=0; klotski<number_of_subbands; ++klotski)
    {
        int number_of_channels = max_channels_per_klotski;
        if(klotski==number_of_subbands-1 && nchans%max_channels_per_klotski!=0) number_of_channels = nchans%max_channels_per_klotski;


        dedisperse_klotski(number_of_channels
                            , ksamps
                            , nsamps
                            , dsamps_per_klotski[klotski]
                            , number_of_dms
                            , &data_in[klotski*max_channels_per_klotski*nsamps+channels_offset*nsamps]
                            , &*data_out.begin()
                            , total_base[klotski]
                            , total_index[klotski]
                            , total_shift[klotski]
                            , counts_array[klotski]
                            , dmindex_shifts[klotski]
                            , &start_dm_shifts[klotski*max_channels_per_klotski+channels_offset]
                            , (int) dsamps_per_klotski[0]);

    }

    return 0;
}

} // namespace klotski
} // namespace ddtr
} // namespace modules
} // namespace cheetah
} // namespace ska
