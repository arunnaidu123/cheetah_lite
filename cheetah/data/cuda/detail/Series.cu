
#include "cheetah/data/cuda/Series.h"

#ifdef SKA_CHEETAH_ENABLE_CUDA

#include "cheetah/cuda_utils/cuda_thrust.h"
#include <panda/arch/nvidia/DeviceCopy.h>

namespace ska {
namespace cheetah {
namespace data {

#ifdef __CUDACC__
#if __CUDACC_VER_MAJOR__ > 10 || (__CUDACC_VER_MAJOR__ == 10 && __CUDACC_VER_MINOR__ >= 2)
#define __NVCC_THRUST_SUPPORTS_ALLOCATOR_CONSTRUCTORS__
#endif

template <typename ValueType, typename Alloc>
Series<cheetah::Cuda, ValueType, Alloc>::Series(std::size_t size)
    : BaseT(size)
{
}

template <typename ValueType, typename Alloc>
Series<cheetah::Cuda, ValueType, Alloc>::Series(std::size_t size, Allocator const& allocator)
#ifdef __NVCC_THRUST_SUPPORTS_ALLOCATOR_CONSTRUCTORS__
    : BaseT(size, allocator)
{
#else // __NVCC_THRUST_SUPPORTS_ALLOCATOR_CONSTRUCTORS__
    : BaseT(size)
{
    (void) allocator;
#endif // __NVCC_THRUST_SUPPORTS_ALLOCATOR_CONSTRUCTORS__
}

template <typename ValueType, typename Alloc>
Series<cheetah::Cuda, ValueType, Alloc>::Series(Allocator const& allocator)
#ifdef __NVCC_THRUST_SUPPORTS_ALLOCATOR_CONSTRUCTORS__
    : BaseT(allocator)
{
#else // __NVCC_THRUST_SUPPORTS_ALLOCATOR_CONSTRUCTORS__
{
    (void) allocator;
#endif // __NVCC_THRUST_SUPPORTS_ALLOCATOR_CONSTRUCTORS__
}

template<typename ValueType, typename Alloc>
template<typename OtherArch, typename OtherValueT, typename OtherAlloc>
Series<cheetah::Cuda, ValueType, Alloc>::Series(Series<OtherArch, OtherValueT, OtherAlloc> const& copy, Alloc const& allocator)
#ifdef __NVCC_THRUST_SUPPORTS_ALLOCATOR_CONSTRUCTORS__
    : BaseT(copy.size(), allocator)
{
#else // __NVCC_THRUST_SUPPORTS_ALLOCATOR_CONSTRUCTORS__
    : BaseT(copy.size())
{
    (void) allocator;
#endif // __NVCC_THRUST_SUPPORTS_ALLOCATOR_CONSTRUCTORS__
    panda::copy(copy.begin(), copy.end(), this->begin());
}

template <typename ValueType, typename Alloc>
Series<cheetah::Cuda, ValueType, Alloc>::~Series()
{
}

template<typename ValueType, typename Alloc>
template<typename OtherArch, typename OtherValueT, typename OtherAlloc>
Series<cheetah::Cuda, ValueType, Alloc>& Series<cheetah::Cuda, ValueType, Alloc>::operator=(Series<OtherArch, OtherValueT, OtherAlloc> const& copy)
{
    this->resize(copy.size());
    panda::copy(copy.begin(), copy.end(), this->begin());
    return *this;
}

#endif // __CUDACC__

} // namespace data
} // namespace cheetah
} // namespace ska

#endif //SKA_CHEETAH_ENABLE_CUDA
