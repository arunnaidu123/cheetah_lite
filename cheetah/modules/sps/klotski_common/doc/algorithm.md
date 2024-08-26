@page klotski_common_sps_algorithm_guide A KLOTSKI SPS algorithm guide
# KLOTSKI spdt Algorithm

This is a simple box car search algorithm which written in assembly using predominently AVX intructions. Instead of performing search of each width at a time. The data is compied in small chunks to the cache and search for all widths is performed at the same time. This will reduce the number of reads of the DMTrials data.

search logic

1) SUM[sample] = SUM[sample-1] + VALUE[sample] - VALUE[sample-width]
2) SNR[sample]  = (SUM[sample]/width)/(STDDEV);
3) if(SNR[sample]>_threshold) save the canditate info.

The SUM information for all the widths and the overlap need for the next chunk is saved to be used for the next set of DMTrials data.