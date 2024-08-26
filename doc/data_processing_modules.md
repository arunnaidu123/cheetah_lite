@page modules Data Processing Modules In Cheetah

# Data Processing Modules

@tableofcontents

## Module Components
Although each module will ahve its own unique requirements, in general we want to maintain a standardised interface
and common standards. We would expect any module therefore to follow as closely as possible the guidelines here.
The table below indicates the minimal interface and layout any user of any module might expect:

| Component     |  Function                                           | Notes
|---------------|-----------------------------------------------------|---------
| @ref module_api "API"        | The Top level User Interface                     | Should always be PascalCase name of the module e.g. MyModule.h
| @ref module_config "Config"  | User configurable options for the module         | Should always be Config.h
| @ref module_algo "Algorithm(s)"  | Code that actually does the data processing      | Will always be in sub-directories of the main module and provide @ref module_algo_api "their own API"
| @ref module_tester "Tester"  | Exercise the API interface and test the results  | Will live in test_utils and should be post-fixed by Tester e.g. MyModuleTester.h

## Design Considerations
Each data processing module in cheetah represents a specific data processing task to be performed.\n
It has a fixed interface that accepts the data type(s) to be processed and a type representing the results
of the processing which are to be passed on to the next stage of the pipeline.
Input data and result data are represented as explicit types.

### Support for multiple algorithms

Each module should allow for the possibility that multiple algorithms will be available to perform its processing task.
e.g. you may have different algorithms:
- to utilise different architectures (e.g Cpu, FPGA, or GPU's)
- optimised to work in specific input data ranges
- to try out different calculation techniques for prototyping, comparison & benchmarking

In general we would expect the user to be able to select which algorithm (or set of algorithms) to use
at runtime through the settings of its Config object.

@anchor module_algo_api_compat
Allowing this runtime flexibility, however, comes with a runtime cost (at a minimum you should expect a virtual lookup).
As a C++ programmer you, very reasonably, expect to not have to pay any runtme cost unless you actively want the functionality thus provided.
If you are writing a pipeline and you wish to hardcode a very specifc algorithm, taking away the users choice, you should be able to
do so and reap any performance benefits. To this end we therefore aim to provide compatible interfaces at
the modules top level API and with each algorithm that supports that API. This then allows the module user to switch modes with very
little effort. For example consider a pipeline dedicated to performing the well know Foo and Bar modules
~~~~{.cpp}
template<class FooT, class BarT>
class FooBarPipeline {
    public:
        // constructors etc, calls foo then bar
        void operator()(InputData const& data);

    private:
        FooT _foo:
        BarT _bar;
};
~~~~

Given such a templated pipeline it should be very easy to generate a fully flexible version, allowing the user to experiment with
different algos and optimisation stratergies:
~~~~{.cpp}
FooBarPipeline<modules::Foo, modules::Bar> foo_bar_pipeline;
~~~~

Perhaps after using such a flexible pipeline we have determined an optimal configuration and now
wish to generate highly optimised production code that squeezes out every overhead, particularly in the FooBar processing which has a
very high duty cycle.
With well defined compatible interfaces we should be able to to simply change the types provided in the template e.g.

~~~~{.cpp}
// Inflexible streamlined pipeline
// using a specific algorithm "foo_algorithm_a" provided by Foo
// and "bar_algorithm_b" provided by Bar

FooBarPipeline<modules::Foo::foo_algorithm_a::Foo, modules::Bar::bar_algorithm_b::Bar> foo_bar_pipeline;

~~~~

In practice, the development of existing modules has not focused on this aspect of design and so not all modules are compliant.

### Supporting Multiple Hardware Devices - Memory
As well as having specialist algorithms tailored to specific hardware inside our module we also need to worry
about the data being passed to, and being exported from it. The data can be stored in memory
in one of 3 places:
- on the host machine (i.e. normal memory we are used to dealing with in cpu code)
- on the accelerator device we have been provided with to process our task
- on a different accelerator device (perhaps as a result from some previous stage of the operation)

To simplify this complexity, data types in cheetah are designed to be aware of which device the memory is on and
to be easily convertible, via specialist copy constructors, between the device specific representations of that data type.
Cheetah uses architecture tags to achieve this.

@anchor module_data_conversion
As the complexity of data transfers is handled by the data objects themselves, writing your modules API interface that can handle all these options can be done with minimal effort.

#### Handling Input Data
e.g. for some member function passed to convert TimeFrequency data from a cuda device to the host you can:
~~~~{.cpp}
class MyModule {
    public:
        typedef cheetah::Cuda Architecture; // this is a Cuda specific Algo of MyModule

        // generic sync method can take in any time-freq data type and converts to the type we need
        // using th copy operator
        template<class ArchT>
        std::shared_ptr<ResultsT> do_something( panda::PoolResource<Architecture>& device, TimeFrequency<ArchT, uint8_t> const& data )
        {
            return do_something_impl(device, TimeFrequency<Architecture, uint8_t>( data, device ));
        }

        // data is already in the format we need - dont waste resources by copying
        std::shared_ptr<ResultsT> do_something( panda::PoolResource<Architecture>& device, TimeFrequency<Architecture, uint8_t> const& data )
        {
            return do_something_impl(device, data);
        }
};
~~~~
Such conversions could be relegated to a CRTP base class template to alieviate developers of algorithms in your module from
having to implement themselves.

#### Handling Return Data
How you return the data should also be carefully considered. Do you wish to return the results in memory on the device, on the host, or let the user decide (e.g. by providing template methods that take an Architecture tag for the return type). Returing on the host is the simplest solution, but may result in expensive, unnessasry copies if the next stage in the pipeline happens to use the same device.

In any case, any new data type that is destined to be returned or passed to an async handler must be created as a std::shared_ptr,
(or perhaps a std::unique_ptr) of the appropriate type.

### A Common Testing Framework
The purpose of the module is to provide a consistent interface that can be used to execute
many potential algorithms conforming to a common interface. Each of these algorithms require unit testing and the module
should provide a suitable common testing framework to exercise its API ensuring consistency and coverage.

### Asyncronous nature of modules
Most modules will support asynchronous interfaces to perform the task.
This means that the caller of the module does not block whilst waiting for the module
to aquire the resources it needs and execute its processing task.
Instead an async module will be initialised with a callback handler that is to be executed
with the results of the processing.

Where possible, modules should in addition provide syncronous interfaces.
Such interfaces will generally require you to provide the compute resource as a function argument.

For more on the top level module API see @ref module_api "here".

## Module Implemantation Details and Conventions

In general each module has its own subdirectory under the \e cheetah directory. The structure of this directory
must conform to the standard set out in the @ref dev-guide "developers guide".
Depending on the module there will be addition requirements as described.

Each module must have an API class with the same name (or rather the PascalCase version of it) as the module itself.
In the majority of cases each module will have its own user configurable options. These shall be represented by a
configuration module class Config in the Config.h file.

e.g. the expansion of the my_module directory tree should look something like this:
@verbatim
   cheetah -+
            +- modules -+
                        +- my_module +                                           <-- Directory name for module always in snake_case
                                 +- CMakeLists.txt
                                 +- MyModule.h                                   <-- API for the module
                                 +- Config.h                                     <-- A panda::ConfigModule compatible node
                                 +- src +
                                        +- MyModule.cpp                          <-- implementation details for MyModule Api
                                        +- Config.cpp                            <-- implementation details for Config
                                 +- detail +
                                        +- MyModule.cpp                          <-- template implementation details live in the detail folder
                                 +- test_utils +
                                               +- MyModuleTester.h               <-- Generic algorithm testing framework for the module and the
                                                                                     (optional) provision of mock types and other convenience functions
                                                                                     that may be helpful to users of the module when they write
                                                                                     their own unit tests
                                               +- details +
                                                  +- MyModuleTester.cpp
                         +- test                                                 <-- usual test directory structure common to all cheetah subpacakges
@endverbatim

### Module Namespaces
All C++ code within the module should be in its own namespace within the cheetah namespace hierarchy.
The namespace should be the same as the directory name for the module.
~~~~{.cpp}
namespace ska {
namespace cheetah {
namespace modules {
namespace my_module {

    // MyModule code here

} // namespace my_module
} // namespace modules
} // namespace cheetah
} // namespace ska
~~~~


@anchor module_api
### The Module API
The API is the primary interface to the module for the end user, and defines the interface for its use as a data processing element.
The user of the module should only need to
~~~~{.cpp}
#include "cheetah/my_module/MyModule.h"
~~~~
to be able to use the my_module data processing features in their code.

e.g. A top level module API interface may look something like this:
~~~~{.cpp}
/**
 * @tparam HandlerT the callback handler which must define a method:
 *                     void operator()(std::shared_ptr<ResultsT>);
 */
template<class HandlerT>
class MyModule
{
    public:
        MyModule(Config const&, HandlerT handler);

        /**
         * @brief async interface that will call the handler provided in the constructor with the results
         */
        void operator()(InputData const& input_data) const;

        /**
         * @brief sync interface that returns the results (as a shared_ptr)
         * @param resource the compute resource to be used to perform the code
         */
        template<class ArchT>
        std::shared_ptr<ResultsT> operator()(panda::Resource<ArchT>& resource, InputData const& input_data) const;
};
~~~~

To implement the async call the module has to:
- select the set of useable algorithms that it has, taking into account user preferences, available hardware etc
- request a suitable resource from the assigned resource pool (the pool to use should be provided through the Config object in the constructor)
- ensure the resource provided by the pool is used and the correct algorithm is called (usually by calling the sync interface)
- pass the results of the execution to the handler

The sync call is easier in that it just has to call the correct algorithm as the resource has already been aquired elsewhere.

Note also the callback handler for the async call is part of the constructor rather than being passed to the async call itself.
This is a little less convenient than allowing you to pass a handler to the call directly, but it does offer the scope of more
compiler optimisations inside the implementation itself, and imposes no requirements to use virtual functions.

There are templated helper frameworks available to fascilitate the implemntation of this
top level API, particularly the resource aquisition and runtime algorithm selection aspects. They are still in refinement and subject to
change.
@see cheetah::utils::AlgoModule for more details.

@anchor module_config
### The Config
Most asyncronous. multi-algorithm modules should inherit from @ref ska::cheetah::utils::ConfigModule, to ensure a consistent
interfacei and ease of use. By doing so you your config will have a pool() method that will return the allocated pool to which
you can submit async jobs. See the  @ref ska::cheetah::utils::ConfigModule "ConfigModule" doc for more details.

Using ConfigModule is not a hard requirement, so if this doesn't fit your needs then you can build your
own from the @ref ska::cheetah::utils::Config, panda::PoolSelector, and panda::ConfigModule components.
At a minimum it must inherit from @ref ska::panda::ConfigModule.

An intance of such a Config can then be added as a node in the pipeline applications configuration tree allowing the
user to set any parameters either in their configuration file or via options on the command line.
The config will allow the user to select the algorithms required for use and provide the pool object
from which the module can request resources.
There is a tutorial available in the panda documentation that goes into
detail about how this works.

@anchor module_tester
### The Module Tester
A generic unit testing framework shall be provided to exercise the modules API. This framework shall be
implemented using the googletest TYPED_PARAM_TEST framework, where the TypeParam will be either the Algorithm type
(e.g. my_module::my_algo::MyModule) or some specific TestTraits object that specifies the algo type and other parameters required to adapt a specific algo to the tester (e.g. some algorithms might have different tolerances for the results).
This Traits type should be well documented.

This tester should be the PascalCase'd module name postfixed with the word Tester and be placed in the test_utils directory of the module.
Each individual algorithm is expected to use this tester as part of their own unit testing suite.

@anchor module_algo
### Adding an algorithm to a module
Each algorithm should have its own dedicated @ref subpackage-structure "subpackage structure" in a subdirectory
of the module. Like the top level module directory it should also have an API described in the PascalCased module name file
(e.g. MyModule.h) and, where appropriate, a Config.h to describe configuration specific to that algorithm.\n

e.g.
@verbatim
   cheetah -+
            +- modules +
                       +- my_module +                            <-- Directory name for module always in snake_case
                                    +- my_algo +
                                               +- CMakeLists.txt
                                               +- MyModule.h     <-- API for the algo (same name as API in the module directory)
                                               +- Config.h       <-- A panda::ConfigModule compatible node (optional)
                                               +- src
                                               +- detail
                                               +- test           <-- usual unit test directory structure

@endverbatim

The algorithm should be in its own subnamespace of the modules namespace.
e.g.
~~~~{.cpp}
namespace ska {
namespace cheetah {
namespace modules {
namespace my_module {
namespace my_algo {

    // my_algo::MyModule  code here

} // namespace my_algo
} // namespace my_module
} // namespace modules
} // namespace cheetah
} // namespace ska
~~~~

@module module_algo_api
#### The Algorithm API
The algorithm should provide an API class with a method that performs the task syncronously.
It will be used as a panda::Task and should conform to the requirements, i.e.:
- you must specify the Architecture type on which your algorithm runs
    - this is done by adding a public typedef called Architecture to your API class
    - The available architecture types are defined in cheetah/utils/Architectures.h
- optionaly you can also specify an ArchitectureCapabilities typedef for further refinement of device capabilities
  e.g. minimum memory requirements, or runtime binary interfaces that need to be supported
  - CUDA options can be found in panda/arch/nvidia/DeviceCapability.h

With the Architecture defined the API should then provide a sync interface that takes a
```panda::PoolResource<Architecture>&``` as its first argument, and returning the results as a shared_ptr of the type
representing the results.

In the interests of providing compatible interfaces between the
module API and the algoirthms API as described @ref module_algo_api_compat "above" you may wish to consider
providing a CRTP base class template to handle async calls,
@ref module_data_conversion "auto data conversions" etc.

#### The Algorithm Config
If there are any configurable parameters specific to your algorithm you can add these to the algorithms Config class.
Each algorithm Config should be a subtree of the modules Config object.
