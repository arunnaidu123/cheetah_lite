@page build Build Guide


Cheetah uses cmake to generate the makefiles or similar for your required tool chain.
First create a directory for your build (you can have more than one, for each type of build such as optimised or debug, with or without accelerator support built in etc). Then use cmake with suitable options to generate the build you require.
```bash
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release -DENABLE_CUDA=true -DPANDA_INSTALL_DIR=<path_to_panda> <path_to_cheetah_src>
```
If all is well cmake will generate the necessary files for your build system (by default on linux systems this will be makefiles).
Once the files are generated you can use your build tool (```make``` on linux) to build the libraries and executables
e.g.
```
make -j
make install
```

## some cmake options
-DBOOST_ROOT=<path_to_boost>        : location of your boost version (see /usr/share/cmake/Modules/FindBoost.cmake for more options)
-DCMAKE_INSTALL_PREFIX              : Root directory used to install files when calling 'make install'. The default for this is usually /usr/local.
-DCMAKE_BUILD_TYPE={debug|release|profile) :  The type of build

-DENABLE_CUDA=true                  : link against the CUDA libs for GPU support
-DENABLE_PSRDADA=true              : generate psrdada functions linking against the psrdada library
-DENABLE_ASTROACCELERATE=true       : use functions dependent on the astroaccelerate libs

If you are having trouble with some specific package, the cmake modules can be checked for documentation.
They will reside in either cheetahs cmake directory, or the default
cmake modules can be found in /usr/share/cmake/Modules.


## Optimized Release Build

Releases builds can be optimized based on profiling information generated when running the test suite. This is a 3 stage process which involves building the libraries with profiling instrumentation, running the test suite to generate profiling information and finally re-building the libraries using the profiling files.

### 1/ Build with training instrumentation

The first step is to build the code with training instrumentation. To do this, create a build directory, and from within this run cmake with the option `-DCMAKE_BUILD_TYPE=pgotrain`.

If the training build directory is called `training` and is placed at the same level as the final build directory, step 3 is easier.

```bash
mkdir training
cd training
cmake -DCMAKE_BUILD_TYPE=pgotrain -DBOOST_ROOT=<PATH TO BOOST> -DPANDA_INSTALL_DIR=<PATH TO PANDA> <PATH TO CHEETAH SOURCE>
make
```

### 2/ Generate the profiling information

Step 2 involves running the test suite, which, due to the training instrumentation built into the code will generate coverage and profiling information used in the next step.

```bash
make test
```

### 3/ Generate final optimized libraries

You will need to create another build directory from which you can run cmake with the build optimization option. cmake needs to know where to look for the generated profiling files, so you supply a `-DPGO_TRAINING_DIR` pointing at the directory you created in step 1 above. You must also specify `-DCMAKE_BUILD_TYPE=pgobuild` so that it uses the profiling information to generate optimized code.

The recommendation is to place the build directory at the same level as the training directory, so that it is easier to find. If the training build directory was called `training` you do not need to supply the -DPGO_TRAINING_DIR option.

```bash
cd ..
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=pgobuild -DBOOST_ROOT=<PATH TO BOOST> -DPANDA_INSTALL_DIR=<PATH TO PANDA> -DPGO_TRAINING_DIR=../training <PATH TO SOURCE>
make
```

### 3a/ Recompiling when source files change

You should be able to just run `make` in the final build area and make will step back into the training area and run `make all` followed by `make test` before making all targets in the final build area.

## Generating the Profiling information

Currently the profiling information used to optimize the final build is derived from running the test suite. Generally the profiling information should be generated when running the required executables in the way they would typically be run, as this will give the best optimization.

If this is changed in the future then the 'run test suite' phase should be changed to run whatever process has been devised to generate the profiling data back in the correct directory.
