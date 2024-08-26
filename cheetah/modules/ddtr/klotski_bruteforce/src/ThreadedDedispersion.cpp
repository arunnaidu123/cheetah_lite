#include <iostream>
#include <vector>
#include <array>
#include <thread>
#include <cmath>

namespace ska {
namespace cheetah {
namespace modules {
namespace ddtr {
namespace klotski_bruteforce {

/**
 * @brief The main dedispersion AVX implementation which performs brutefroce dedispersion.
 * @param data_in pointer the the input data to be dedispersed.
 * @param shifts dm shifts per Dm per channel
 * @param data_out output data (subanded Dm trials object)
 * @param start_dm_shifts initial shifts to be applied before reading in the data
 * @param variables pointer to the variables to be stored in the stack
 * @param total_size total size of the stack used for dedispersion.
 */
extern "C" void nasm_dedisperse_klotski_bruteforce( void *data_in
                           , const void *shifts
                           , void *data_out
                           , const unsigned int* start_dm_shifts
                           , std::size_t* variables
                           , std::size_t total_size);


/**
 * @brief AVX implementation of integration of subbands.
 * @param data_out output data.
 * @param array_pointers all the pointers to the subbands
 * @param number_of_pointers number of elements in the array_pointers (essentially number of subbands).
 * @param number_of_samples number of samples dedispersed.
 * @param number_of_channels number of channels in the Klotski.
 * @param total_size_of_stack Total size of the stack used.
 */
extern "C" void nasm_integrate_klotski_bruteforce( float *data_out
                           , std::size_t *array_pointers
                           , std::size_t number_of_pointers
                           , std::size_t number_of_samples
                           , std::size_t number_of_channels
                           , std::size_t total_size_of_stack
                           );

/**
 * @brief AVX implementation of downsampling or binning of data.
 * @param data input FT block
 * @param number_of_elements total number of elements in the data block.
 */
extern "C" void nasm_downsample_klotski_bruteforce(unsigned short *data
                                , unsigned int number_of_elements
                                );
/**
 * @brief integrates the timeseries from all the subbands and write it to dmtrials object
 * @param data_out pointer to single DmTrial location in DmTrials object
 * @param data_temp 2D vector which contains the subbanded DM trials
 * @param number_of_channels number of channels
 * @param number_of_elements total number of elements (channels*spectra)
 * @param dm_index dm index of the dm trial.
 */

void integrate_klotski_bruteforce( float* data_out
                      , std::vector<std::vector<int>>& data_temp
                      , std::size_t number_of_channels
                      , std::size_t number_of_elements
                      , std::size_t dm_index
                      )
{
    std::vector<std::size_t*> pointers;
    for(std::size_t i=0; i<data_temp.size(); ++i)
    {
        pointers.push_back(reinterpret_cast<std::size_t*>(&*(data_temp[i].begin()+dm_index*number_of_elements)));
    }

    nasm_integrate_klotski_bruteforce(data_out
               , reinterpret_cast<std::size_t*>(&*pointers.begin())
               , pointers.size()
               , number_of_elements
               , number_of_channels
               , pointers.size()*sizeof(std::size_t));
}

namespace {

/**
 * @brief This method estimates all the variables need for the Klotski_bruteforce which will be stored in the cache.
 */

inline
void dedisperse_klotski_bruteforce_input_array(
                         std::array<std::size_t, 15>& variables
                       , std::size_t nchans
                       , std::size_t ksamps
                       , std::size_t tsamps
                       , std::size_t dsamps
                       , std::size_t ndms
                       , std::size_t flag
                       )
{
    // setup variables to pass to kernel
    variables[0] = (tsamps)*sizeof(short);            // TSAMPS_SIZE
    variables[1] = (ksamps+32)*sizeof(short);         // TSTRIDE_SIZE
    variables[2] = (ksamps+32)/32;                    // NBLOCKS_PER_CHANNEL
    variables[3] = nchans;                            // NCHANS
    variables[4] = (ksamps+32)*sizeof(short)*nchans;  // DATA_SIZE
    variables[5] = nchans*sizeof(float);              // SHIFTS_SIZE
    variables[6] = nchans*sizeof(int);                // START_SHIFTS_SIZE
    variables[7] = variables[4] + variables[5] + 256; // START_SHIFTS_LOCATION
    variables[8] = variables[7] + variables[6];       // CURRENT_SHIFTS_LOCATION
    variables[9] = ksamps*sizeof(short);              // NSAMPS_SIZE
    variables[10] = dsamps;                            // DSAMPS
    variables[11] = ksamps;                            // NSAMPS
    variables[12] = ndms;                             // NDMS
    variables[13] = flag;                             // FLAG
    variables[14] = (nchans*(ksamps+32))*sizeof(short) + 3*nchans*sizeof(float) + 256; // TOTAL_SIZE
}

} // namespace

/**
 * @brief call the nasm_dedisperse_klotski_bruteforce
 *        The data in copied to data_temp
 * @details the arguments to the kernel are sent as a vector and this function generates that vector and
 * passes it to the appropriate kernel
 * @param nchans number of channels
 * @param ksamps number of samples per klotski_bruteforce
 * @param tsamps number of samples in the data_in buffer
 * @param dsamps dedispersed samples
 * @param ndms number of dm in the current dm range
 * @param dm_shifts shift per dm for each channel
 * @param start_dm_shifts initial shifts to be applied before reading in the data
 * @param data_in input vector
 * @param data_temp vector which holds the intermediate data products
 */

void dedisperse_klotski_bruteforce( std::size_t nchans
                       , std::size_t ksamps
                       , std::size_t tsamps
                       , std::size_t dsamps
                       , std::size_t ndms
                       , const float* dm_shifts
                       , const unsigned int* start_dm_shifts
                       , unsigned short* data_in
                       , int* data_temp
                       , std::size_t flag
                       )
{
    // setup variables to pass to kernel
    std::array<std::size_t, 15> variables;
    dedisperse_klotski_bruteforce_input_array(variables, nchans, ksamps, tsamps, dsamps, ndms, flag);

    nasm_dedisperse_klotski_bruteforce(reinterpret_cast<void*>(&data_in[0])
                , reinterpret_cast<const void*>(&dm_shifts[0])
                , reinterpret_cast<void*>(&data_temp[0])
                , start_dm_shifts
                , &*variables.begin()
                , variables[14]);
}


/**
 * @brief performs the subband dedispersion.
 * @details call the dedisperse_klotski_bruteforce function with the appropriate flags.
 * The first klotski_bruteforce of the subband should be called with 0 flag and the rest with flag 1
 * @param channels_per_subband vector containing information about the number channels in a given band
 * @param nsubbands number of subbands per band
 * @param tsamps number of samples in the data_in buffer
 * @param dsamps dedispersed samples (samples per dm index)
 * @param ndms number of dm in the current dm range
 * @param dm_shifts shift per dm for each channel
 * @param data_in input FT data (TF data converturnded to std::vector)
 * @param data_temp vector which holds the intermediate data products
 */

void subanded_dedisperse(const unsigned int* channels_per_subband
                       , std::size_t nsubbands
                       , std::size_t tsamps
                       , std::size_t dsamps
                       , std::size_t ndms
                       , const float *dm_shifts
                       , const unsigned int* start_dm_shifts
                       , unsigned short *data_in
                       , int* data_temp)
{
    std::size_t subband = 0;
    std::size_t flag = 0;
    int nchans_total = 0;

    std::size_t ksamps = std::ceil(dm_shifts[channels_per_subband[subband]-1]*ndms/1024.0)*1024;;
    if(ksamps%32!=0) ksamps += 32-ksamps%32; //making sure ksamps is a multiple of 32.

    while(subband<nsubbands)
    {

        if(subband != 0) flag=1;

        dedisperse_klotski_bruteforce( channels_per_subband[subband]
                       , ksamps
                       , tsamps
                       , dsamps
                       , ndms
                       , &dm_shifts[nchans_total]
                       , &start_dm_shifts[nchans_total]
                       , data_in
                       , data_temp
                       , flag
                       );

        nchans_total += channels_per_subband[subband];
        ++subband;
    }
}

/**
 * @brief Spawns multiple threads one for each subband to perform dedispersion.
 * @param channels_per_band is two dimentional vector conating information about the number channels in a given band
 * @param nsamps number of samples in the data_in buffer
 * @param dsamps dedispersed samples (samples per dm index)
 * @param ndms number of Dms in the current dm range
 * @param dm_shifts shift per dm for each channel
 * @param start_dm_shifts initial shifts applied to the data to offset the start DM of range.
 * @param data_in input FT data (TF data converturnded to std::vector)
 * @param data_temp vector which holds the intermediate data products
 */
void threaded_dedispersion(std::vector<std::vector<unsigned int>> const& channels_per_band
                       , std::size_t nsamps
                       , std::size_t dsamps
                       , std::size_t ndms
                       , std::vector<float> const& dm_shifts
                       , std::vector<unsigned int> const& start_dm_shifts
                       , std::vector<unsigned short>& data_in
                       , std::vector<std::vector<int>>& data_temp
                       )
{
    std::vector<std::thread> ddtr_threads;
    int sum_channels=0;

    for(std::size_t i=0;i<channels_per_band.size();++i)
    {

        ddtr_threads.push_back(std::thread(subanded_dedisperse
                       , &*channels_per_band[i].begin()
                       , channels_per_band[i].size()
                       , nsamps
                       , dsamps
                       , ndms
                       , &dm_shifts[sum_channels]
                       , &start_dm_shifts[sum_channels]
                       , &data_in[sum_channels*nsamps]
                       , &*data_temp[i].begin()
                       ));

        for(unsigned j=0; j<channels_per_band[i].size(); ++j)
        {
            sum_channels += channels_per_band[i][j];
        }
    }

    for(unsigned i=0;i<ddtr_threads.size();++i) ddtr_threads[i].join();

    nasm_downsample_klotski_bruteforce(&*data_in.begin(), data_in.size());
}

} // namespace klotski_bruteforce
} // namespace ddtr
} // namespace modules
} // namespace cheetah
} // namespace ska
