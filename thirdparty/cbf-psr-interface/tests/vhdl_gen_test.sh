#!/bin/bash

# we test one input file at a time to avoid problems where 
# CI decides to expand a glob in a different order than developer's PC...

mkdir -p output

for f in cpp/global_ns/*.cpp
do
    outfile="output/"`basename $f cpp`vhd
    python3 ../src/translate/translate.py "$f" "$outfile"
done

for f in cpp/parser_stress_ns/*.cpp
do
    outfile="output/"`basename $f cpp`vhd
    python3 ../src/translate/translate.py --namespace parser_stress "$f" "$outfile"
done
