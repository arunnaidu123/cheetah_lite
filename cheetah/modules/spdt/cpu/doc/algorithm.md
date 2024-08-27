@page cpu_sps_algorithm_guide A CPU SPS algorithm guide
# CPU spdt Algorithm
This is a very trivial implementation of a sps search. In this CPU version the mean and standard deviation of the data is obtained from the zero dm time series at the fundamental sampling time. These values are then scaled according to the width of matched filter and the time binning. Just to reduce the duplication of candidates we write out coarse triggers with the window size specified by the user.

The whole algorithm can be summerized in three steps
1) Estimate mean and standard deviation
2) Perform search on each time series and find bins with power higher than the threshold.
3) Write out coarse triggers to the candidate list.
