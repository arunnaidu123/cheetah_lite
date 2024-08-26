#include "cheetah/data/cuda/detail/CachingAllocatorImpl.cuh"

namespace ska {
namespace cheetah {
namespace data {

template <typename T>
T* CachingAllocatorImpl<cheetah::Cuda,T>::_allocate(std::size_t num_elements)
{
    return thrust::cuda::malloc<T>(num_elements).get();
}

template <typename T>
void CachingAllocatorImpl<cheetah::Cuda,T>::_free(T* ptr)
{
    thrust::cuda::free(thrust::cuda::pointer<T>(ptr));
}

} // namespace data
} // namespace cheetah
} // namespace ska
