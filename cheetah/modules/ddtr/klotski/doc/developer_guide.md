@page klotski_developer_guide AVX512 based bruteforce developerguide
# Notes for Developers
## AVX Kernels
All the AVX-512 kernels are written in NASM.
This module provides four AVX-512 kernels:
- DDTR kernel which performs Dedispersion on each klotski (Klotski performs dedispersion by dividing data into blocks or klotskis of size equal to or less than the size of the L1 cache)
- DDTR accumulate kernel which performs Dedispersion on each klotski and adds to the previous klotskis DDTR output.
- Integrate Kernel which simply adds all the dedispersed timeseries of all the subbands and write it to dm trials object.
- Downsample kernel which downsamples the FT data in the temporary working area.

All the kernels can be found in the src/kernels directory.

## AVX-512 capability
The algorithm works on all the CPUs which have AVX-512 registers. Most of the Intel 3rd Generation Xeon scalable processors and the 4th generation AMD Epyc processors should be able to support the kernels. But it is advised to check if the processors have the capability before compiling the code with -DENABLE_NASM.

## Constants used
The overlap required for the kloski algorithm should be multiple of 1024. The value is given in the detail/KlotskiConstraits.h.
