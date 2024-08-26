#ifndef SKA_CHEETAH_DATA_CUDA_SERIES_H
#define SKA_CHEETAH_DATA_CUDA_SERIES_H

#include "cheetah/Configuration.h"

#ifdef SKA_CHEETAH_ENABLE_CUDA

#include "cheetah/data/Series.h"
#include "cheetah/cuda_utils/cuda_thrust.h"
#include <panda/arch/nvidia/DeviceCopy.h>

namespace ska {
namespace cheetah {
namespace data {

/**
 * @brief Specialisation of the Series class for Cuda architectures
 * @details note that the implementation is not included here. Instead
 *          the class has explicit instantiations made available in the library
 *          via the src/Series.cu file. If you are getting link errors relating
 *          to this class you should verify that the appropriate types and
 *          methods are being generated there.
 */
template <typename ValueType, typename Alloc>
class Series<cheetah::Cuda, ValueType, Alloc>: public VectorLike<thrust::device_vector<ValueType,Alloc>>
{
        typedef VectorLike<thrust::device_vector<ValueType,Alloc>> BaseT;

    public:
        typedef cheetah::Cuda Architecture;
        typedef Alloc Allocator;
        typedef ValueType value_type;

    public:
        Series(std::size_t size);

        Series(std::size_t size, Allocator const& allocator);

        Series(Allocator const& allocator);

        /**
         * @brief copy constructor. Series data transfered from Series data on a device
         */
        template<typename OtherArch, typename OtherValueT, typename OtherAlloc>
        Series(Series<OtherArch, OtherValueT, OtherAlloc> const& copy, Alloc const& allocator=Alloc());
        ~Series();

        /**
         * @brief assignment operator
         */
        template<typename OtherArch, typename OtherValueT, typename OtherAlloc>
        Series& operator=(Series<OtherArch, OtherValueT, OtherAlloc> const& copy);

};

} // namespace data
} // namespace cheetah
} // namespace ska
//#include "detail/Series.cu"

#endif //SKA_CHEETAH_ENABLE_CUDA
#endif // SKA_CHEETAH_DATA_CUDA_SERIES_H
