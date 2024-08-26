@page ddtr_algorithm_guide DDTR algorithm guide

# Introduction to Dedispersion for Radio Pulsar Search

Pulsars are rapidly rotating neutron stars that are observed as train of pulses of radio waves when the magnetic pole crosses the line-of-sight to the earth as star rotates. These pulses are often dispersed due to the effects of the interstellar medium. Dedispersion is a technique used to correct for this dispersion and recover the true pulse profile.

The basic principle of dedispersion is that pulses with different dispersion measures (DMs, A quantity which describes the extent to which the radio emission is dispersed by the foreground ISM) will arrive at different times as a function of frequency. By shifting the arrival times of the pulses based on their DM in each frequency channel (The EM signal is divided in the several frequency channels and delay is applied to each frequency channel)and summing them together, the dispersed pulses can be "de-dispersed" and the true pulse profile can be recovered.

There are several algorithms that can be used for dedispersion transform (DDTR, dedispersing according to a specific DM in the list of trial value), including the "Brute-Force" and "Tree-Dedispersion" methods. The brute-force method involves creating a dedispersed time series for all the user-specified DMs. The Tree-dedispersion method is faster, where the Dedispersion transform is performed at close to NlogN complexity, significantly reducing the number of computations required compared to the Brute-force method (which uses N^2 complexity).

Dedispersion is an essential step in the pulsar search process as it allows for the recovery of the true pulse profile, which can then be used for further analysis and identification of potential pulsar candidates and a crucial step in the pulsar search process.

# Implementation in CHEETAH
The DM ranges to search are provided by the user in the config along with the algorithm to be used for the dedispersion transform (astro-accelerate, cpu or klotski). Each algorithm will have a DedispersionPlan object which will check if it is feasible to perform dedispersion transfor (DDTR). Once it is estimated that the DDTR can be performed, DedispersionPlan object will estimate the buffer sizes and the accelerator resources required for dedispersion (with the help of DedispersionStrategy object) required for the accelerator (which also inclues AVX) to implement the dedispersion.

## A typical psuedo code for dedispersion is as follows

1. Initialize buffers according the user specified DM ranges and the limitation of the algorithm and accelerator and throw if the DDTR is not feasible on the specified accelerator.

2. Buffer incoming TF (Time-Frequency) data and trigger DDTR once filled.

3. In case of non-CPU accelerator move the data to the onboard memory of the accelerator.

4. Cornerturn the TF block -> FT(Frequency-Time) block (this is not always necessary and depends on the specific algorithm used.)

5. For each channel calculate the time delay for each frequency channel due to dispersion

6. Shift the data in the time domain by the calculated delay in each channel.

7. Combine the shifted data from all frequency channels to form a dedispersed time series.

8. Repeat steps 4-5-6 for all user specified DMs

9. Move the resultant time series data to the TimeSeries object.