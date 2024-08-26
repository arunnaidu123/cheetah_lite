\anchor documentation-conventions
# Documentation Conventions

This page defines the collection of rules and recommendations for documentation of C++ code within the Cheetah code base.

The Cheetah documentation is generated using Doxygen and the below rules and recommendations include the doxygen conventions to be used.

## 1. Code Comments

This section provides rules and recommendations for source code comments.

### 1.1 API Code

Source code comments in API's should be aimed at library users. Library users are those that are interested in using the PSS modules and putting in pipelines.

### 1.2 Non API Code

Source code comments in all non API code should be written such that it explains functionality clearly. Where possible, provide references to algorithms. 

#### 1.2.1 Header files

Every class and struct should be preceded with the following comment structure:

@verbatim
/**
 * @brief Short high level summary
 *
 * @details Description and any relevant background information. This should give the user understanding and context of what this class or struct is.
 */
 @endverbatim

Example

@verbatim
/**
 * @brief This class represents a Fast Fourier transform.
 *
 * @details - A Fast Fourier transform is an algorithm that computes the discrete Fourier transform of a sequence, or its inverse.
 * Fourier analysis converts a signal from its original domain to a representation in the reciprocal (usually frequency) domain and vice versa.
 *
 */
@endverbatim

Every function should be preceded with the following comment structure:

@verbatim
/**
* @brief Short high level summary
* @details Any relevant information.
* @param[in] Name and description of input parameter to the function
* @param[out] Name and description of output parameter to the function
* @tparam Name and description of template parameters for the function
*/
@endverbatim

Example

@verbatim
/**
* @brief - Performs a fast fourier transform from real to complex numbers.
* @details - This call is performed asyncronously.
* @tparam InputArch - Specified architecture
* @tparam InputType - The type of data for performing the fft on.
* @param[in] TimeSeries - The time series data for the specified architecture.
*/
template<class InputArch, typename InputType>
std::shared_ptr<panda::ResourceJob> fft_to_complex(std::shared_ptr<data::TimeSeries<InputArch, InputType>> const&) const;
@endverbatim

#### 1.2.2 Source Code files

-# Source code files should NOT contain comments outside of class, structs or functions.

-# Source code files should NOT contain doxygen style comments, instead they should have ordinary comments preceeding with '//'

-# Clear and descriptive naming of classes, structs, functions and variables is preferable to comments. Comments should be used where necessary to aid understanding of what the code is doing and why in cases where it might not be immediatedly obvious and requires addition information.

Example - The line below does not require a comment. It is clear that this function starts an algorithm running on the given device with an input and output.

@verbatim
  launch_algo(device, *input, output_data);
@endverbatim

Example -The lines below are a good example of using a comment.

@verbatim
  // The input buffer data_in is filled up with normally distributed data using std::mt19937 generator.
  fill_with_random_data(data_in, mean_n, std_dev_n);
@endverbatim
