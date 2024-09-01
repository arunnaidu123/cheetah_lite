@page klotski_algorithm_guide Klotski algorithm guide
# Notes for Developers
## Concept of Klotski algorithm
Klotski is an AVX-512-based tree dedispersion algorithm where most of the compute-intensive code is written in assembly. Klotski performs dedispersion by dividing data into blocks or Klotskis of size equal to or less than the size of the L1 cache. This way, we can ensure there are no cache misses. The concept  is similar to Klotski-bruteforce except that Klotski uses a tree-dedispersion algorithm.

## The AVX code logic
The assembly part of the code will perform a very simple task.

for(int iter=0; iter<count; iter++)
{
    for(int sample=0; sample<dsamps; sample++)
    {
        data_out[index[iter]][sample] = data_out[base[iter]][sample] + data[index[channel]][sample+shift[iter]];
    }
}

The index and base arrays will contain the specific channel for a given iteration. The shift array simply contains the shift to be applied for a given iteration.

The base, index and shift arrays are precomputed for the given range, subbands, klotski and iteration. The precision required and all the ajustments need for the accuracy can be done in the strategy object.

## Dedispersion strategy of the klotski algorithm
Before performing the dedispersion the Klotski algorithm will pre calculate all the steps need to perform dedispersion. The AVX code simply follow the steps in the strategy object.

## Determine the maximum number of channels per subband
The algorithm aims to perform DDTR in real-time. Hence, Klotski is designed to use the multi-core architectures of the processor by dividing the dedispersion task into several small jobs and running each job on a designated thread. Klotski performs dedispersion in small block and the these block are executed in sequence in a thread. Each subband has designated thread to perform dedispersion and the max_number_of_channels_per_subband will determine the number of threads. It is up to the user to determine the number of threads needed by adjusting the max_number_of_channels_per_subband.

### Determine the size of the L1 cache

The user should also provide the size of the L1 cache in the config file; by default it is set to 2 MB.

## simple work flow of the algorithm.

buffering -> cornerturn -> threaded dedispersion -> integrate -> downsample

the last three steps are run in loop to cover all the dm ranges.