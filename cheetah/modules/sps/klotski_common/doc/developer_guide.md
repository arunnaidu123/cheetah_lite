@page klotski_common_sps_developer_guide klotski_common_sps developerguide
# Notes for Developers
The algorithm is very efficient. _max_width value is used for estimating the overlap to be save. It must be a multiple of 1024 this is a restriction from the AVX processing. All the widths which are need to be searched should be less than _max_width value. Other than that there is no restriction on the number of widths. The computational cost actually very minimal for each additional width value but it will add up.

Klotski SPS and klotski_bruteforce SPS use klotski DDTR and Klotski_bruteforce DDTR repectively. The rest of the SPDT is same.