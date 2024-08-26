@page gpu_bruteforce_algorithm_guide GPU bruteforce algorithm guide
# Notes for Developers
## Concept of Klotski bruteforce algorithm
Klotski bruteforce is an AVX-512-based dedispersion algorithm where most of the compute-intensive code is written in assembly. Klotski performs dedispersion by dividing data into blocks or Klotskis equal to or less than the size of the L1 cache. This way, we can ensure there are no cache misses.

### Determine the maximum number of channels per Klotski
The algorithm aims to perform DDTR in real-time. Hence Klotski is designed to use the processor's multi-core architectures by dividing the dedispersion task into several small jobs and running each job on a designated thread. The number of operations required for the dedispersion is N_dms * K_chans * N_samps. Where N_dms is the total number of DMs in a dm range, the N_samps are the total number of spectra in the input buffer, and K_chans is the number of channels in Klotski. The time it takes to perform these operations depends on all these values (N_dms, K_chans, N_samps) and the processor's clock speed. N_samps and N_dms are user defined in the config file. The user must also provide the maximum number of channels to be used in a Klotski. This will determine the number of subbands and the number of threads that should be executed to run the Klotski algorithm.

Number of threads = total_number_of_channels/max_number_of_channels_per_klotski

The maximum K_chans value can be determined from the base clock speed of the CPU, the bus speed and the sampling time of the data. In the case of Tengu (machine in Oxford), with a processor of clock speed 2.2 GHz, I have determined it to be 256 channels for 100 us sampling time and 128 channels for 50 us sampling time. 256 is set as default.

Please note: You can provide any value as the max K_chans in the config the algorithm will work. However, it is up to the user to determine the proper value so the algorithm can make use of the CPU resources efficiently.

### Determine the size of the L1 cache

The user should also provide the size of the L1 cache in the config file by default; it is set to 1 MB.


### Determining the number of Klotskis per subband.
The input data are divided into X subbands. where is X = total_number_of_channels/K_chans_max. Each subband may be further divided into several Klotskis. This is particularly true for lower frequencies, as the minimum overlap is inversely proportional to the observing frequency. The typical size of the kloski is k_chans*k_samps. Where k_samps = max_overlap + 32. where 32*2 bytes is the size of the AVX-512 register, which is also equal to the number of samples resulted for each iteration. So, the number of Klotskis per subband is determined by making sure that k_chans*k_samps does not exceed the size of the L1 cache.

### simple workflow of the algorithm.

buffering -> cornerturn -> threaded dedispersion -> integrate -> downsample
