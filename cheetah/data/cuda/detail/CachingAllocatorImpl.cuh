#ifndef SKA_CHEETAH_DATA_CUDA_CACHINGALLOCATORIMPL_CUH
#define SKA_CHEETAH_DATA_CUDA_CACHINGALLOCATORIMPL_CUH

#include "cheetah/Configuration.h"
#ifdef SKA_CHEETAH_ENABLE_CUDA

#include "cheetah/data/detail/CachingAllocatorImpl.h"
#include "cheetah/cuda_utils/cuda_thrust.h"

namespace ska {
namespace cheetah {
namespace data {

/**
 * @brief Cuda implementation of the CachingAllocator
 */
template <typename T>
class CachingAllocatorImpl<cheetah::Cuda,T>: public CachingAllocatorImplBase<CachingAllocatorImpl<cheetah::Cuda,T>,T>
{
    private:
        //friend the base class to access the cached blocks
        friend class CachingAllocatorImplBase<CachingAllocatorImpl<cheetah::Cuda,T>,T>;

        //Directly allocate cuda memory
        static T* _allocate(std::size_t num_elements);

        //Directly free cuda memory
        static void _free(T* ptr);
};

} // namespace data
} // namespace cheetah
} // namespace ska

#include "cheetah/data/cuda/detail/CachingAllocatorImpl.cu"

#endif // SKA_CHEETAH_ENABLE_CUDA
#endif // SKA_CHEETAH_DATA_CUDA_CACHINGALLOCATORIMPL_CUH
