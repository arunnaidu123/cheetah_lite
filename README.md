# Welcome to the Cheetah - the pipeline for the SKA Time Domain Team

## API documentation 

https://ska-telescope.gitlab.io/pss/ska-pss-cheetah/

## Installation
This is a cmake project. Please see CMakeLists.txt file for build instructions

## Source Code
The src can be found under the cheetah directory

## Dependencies
cmake
Panda
Boost
CUDA - only on devices ># 3.5 compute capability

## Contributing
Plesae see doc/developers_guide

## Running cheetah

The cheetah project will generate a number of executables.
You can either install these (e.g. by typing 'make install') and use them in the normal way
or run them directly from your build directory as shown in the examples below. 

All cheetah executables support the --help option.

The main executable for launching streaming pipelines is:

        {your_build_dir}/cheetah/pipelines/cheetah_pipeline

The performace test for the rcpt module can be launched (for example):

        {your_build_dir}/cheetah/pipeline/cheetah_rcpt_performace_test --emulator.generator null --udp.number_of_threads#3

A stand alone emulator (generating UDP packets containing beamformed data):

        {your_build_dir}/cheetah/emulator/cheetah_emulator --emulator.generator gaussian_noise
