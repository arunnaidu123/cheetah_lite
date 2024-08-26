@page corner_turn_developer_guide CornerTurn Developer Guide

# Notes for Developers
## Adding a new Algorithm
### The Algorithm
A standard cheetah module subdirectory should be created to contain the Algorithm.
This should provide a CornerTurn.h file which contains the main API.

In this class create a CornerTurn class inheriting from the AlgoCRTP base class.
This will ensure checks are made on the arguments passed to the \ref copy_corner_turn method.

~~~~{.cpp}
class CornerTurn : corner_turn::AlgoCRTP<CornerTurn, Cpu>
{
    public:
        typedef Cpu Architecture; // specifiy the Architecture on which your algo will run on
                                  // n.b must be the same as that passed to AlgoCRTP's second parameter

    private:
        // implement this function to call your Arch specific algo
        template <typename SourceIteratorT, typename DestinIteratorT>
        static inline
        void copy_corner_turn_random_access(
                         SourceIteratorT&& src_it
                       , DestinIteratorT&& dst_it
                       , std::size_t num_input_innerloop_elements
                       , std::size_t num_input_outerloop_elements);

};
~~~~
### Unit Testing The Algorithm
Use the \ref CornerTurnTester available in the corner_turn/test_utils directory to create a suitable
executable inside your algorithms test directory.

### Integrating Your new Algorithm into the top level API
Add your Algorithm to the details/Algorithms.h file.
You will need to:
- put suitable guards around your `#include` for the type of Architecture at the top of the file
- add your CornerTurn class to the ActiveAlgos tuple, again with suitable guards.
  Note that only one algorithm for each Architecture type can be specified for any given configuration.

Unit tests for the top level API are automatically generated for each type in the ActiveAlgos tuple.
