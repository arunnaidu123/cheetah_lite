
#include "cheetah/data/cuda/detail/Series.cu"
#include "cheetah/data/DefaultAllocator.h"
#include "cheetah/data/ComplexTypeTraits.h"
#include "cheetah/utils/System.h"

namespace ska {
namespace cheetah {
namespace data {

// The purpose of this file is to generate explcit instantiations of the cuda Series template to add to our library
// so that it may be used without the cuda compiler
//
// These functins are template classes with template base classes and getting C++ to ensure these imlementations
// make it into the library is tricky. We cannot use the standard explcit instantiation notation
//    (e.g. template class ClassT<double>;) for several reasons:
//  1) C++ will only instantiate the top level class and not a templated base class, working on the assumption that
//     this class will be generated eleswhere.
//  2) There are a number of templated methods on each class, each of which needs explicit instantiation.
//  3) C++11 restricts such notation to namesapce context (i.e we can't specify it inside a class context).
//     This makes it imposssible to auto-generate the architecture dependent methods we need automatically
//     based on the Systems architecture types. Note this restriction is removed in C++14.
//  To solve this problem we create a dummy object that obliges the compiler to generate all the layers so
//  that they are available in our library. This is a bit ugly as we have to have this dummy object in our
//  library too (and the associated singleton method to ensure it get's included) even though it will never
//  be used. If we move to C++14 we should be able to remove, or at least reduce the size of, this dummy.

// some traits for passing types between our dummy class layers
template<typename ValueT, typename AllocatorT>
struct CudaSeriesTraits
{
    typedef Series<cheetah::Cuda, ValueT, AllocatorT> SeriesType;
    typedef AllocatorT CudaAllocator;
    typedef ValueT ValueType;
};


// generates all the methods associated with a specific architecture
template<class CSTraits
       , class ArchT
       , typename ValueT
       , typename AllocT = typename DefaultAllocator<ArchT, ValueT>::type>
struct CudaSeriesOtherSeriesMethods
{
    typedef Series<ArchT, ValueT, AllocT> Type;
    typedef typename CSTraits::CudaAllocator CudaAllocator;
    typedef typename CSTraits::SeriesType CudaSeries;

    CudaSeriesOtherSeriesMethods(int force_init, typename CSTraits::SeriesType& s)
        : _val(force_init)
    {
        CudaAllocator alloc;
        std::unique_ptr<CudaSeries> p2(this->dummy_construct(alloc, _p1.get()));
        std::unique_ptr<CudaSeries> p3(this->dummy_construct(*_p1));
        *_p1 = *p3;
        *p3 = s;   // inverse assignment operator
        s = *_p1;  // cuda assignment operator
    }

    CudaSeries* dummy_construct(CudaAllocator const& alloc, Type const* t)
    {
        if( t == nullptr) throw  std::string("dummy class to ensure instantiation. please dont use");
        return new CudaSeries(*t, alloc);
    }

    CudaSeries* dummy_construct(Type const& t)
    {
        return new CudaSeries(t);
    }

    int _val;
    std::shared_ptr<Type> _p1;
};

// This classes job is to generate a CudaSeriesOtherSeriesMethods object
// for each architecture defined in the utils::System object
template<class CSTraits, class... ArchTs>
struct CudaSeriesBase {
    CudaSeriesBase(int, typename CSTraits::SeriesType&) {};
    void dummy_construct();
};

template<class CSTraits, class ArchT, class... ArchTs>
struct CudaSeriesBase<CSTraits, ArchT, ArchTs...>
    : public CudaSeriesBase<CSTraits, ArchTs...>
{
    typedef CudaSeriesBase<CSTraits, ArchTs...> BaseT;
    typedef CudaSeriesOtherSeriesMethods<CSTraits, ArchT, typename ConvertComplexNumber<ArchT, typename CSTraits::ValueType>::type> MethodsT;

    using BaseT::dummy_construct;

    CudaSeriesBase(int i, typename CSTraits::SeriesType& s)
        : BaseT(i, s)
        , _methods(i, s)
    {}

    MethodsT _methods;
};

template<class CSTraits, typename... DataTs>
struct CudaSeriesBase<CSTraits, std::tuple<DataTs...>>
    : public CudaSeriesBase<CSTraits, DataTs...>
{
    typedef  CudaSeriesBase<CSTraits, DataTs...> BaseT;

    CudaSeriesBase(int i, typename CSTraits::SeriesType& s)
        : BaseT(i,s) {};
    using CudaSeriesBase<CSTraits, DataTs...>::dummy_construct;
};

// Generates the Series class top level for a specific numericalT
template<typename NumericalT, typename AllocatorT = DefaultAllocator_t<cheetah::Cuda, NumericalT> >
struct CudaSeriesImpl
    : public CudaSeriesBase<CudaSeriesTraits<NumericalT, AllocatorT>, typename utils::System::SupportedArchitectures>
{
    typedef CudaSeriesTraits<NumericalT, AllocatorT> Traits;
    typedef CudaSeriesBase<Traits, typename utils::System::SupportedArchitectures> BaseT;
    typedef typename Traits::SeriesType Type;
    using BaseT::dummy_construct;

    CudaSeriesImpl(int i)
        : BaseT(2*i, *_p1)
        , _val(i)
    {
        AllocatorT alloc;
        _p1.reset(this->dummy_construct(std::size_t(10), alloc));
        std::unique_ptr<Type> p2(this->dummy_construct(alloc, *_p1));
        std::unique_ptr<Type> p3(this->dummy_construct(*p2));
        std::unique_ptr<Type> p4(this->dummy_construct(alloc));
        *p3 = *p4;
    }

    Type* dummy_construct(std::size_t p1, AllocatorT const& alloc)
    {
        return new Type(p1, alloc);
    }

    Type* dummy_construct(AllocatorT const& alloc, Type const& t)
    {
        return new Type(t, alloc);
    }

    Type* dummy_construct(Type const& t)
    {
        return new Type(t);
    }

    Type* dummy_construct(AllocatorT const& alloc)
    {
        return new Type(alloc);
    }

    int _val;
    std::shared_ptr<Type> _p1;

};

// Top level dummy class that generates normal and comlex number variants
template<typename NumericalT>
struct CudaSeries
{
    CudaSeries()
        : _normal(1)
        , _complex(2)
    {}

    // normal type with default allocator
    CudaSeriesImpl<NumericalT, DefaultAllocator_t<cheetah::Cuda, NumericalT>> _normal;

    // complex number type with default allocator
    typedef ComplexTypeTraits_t<cheetah::Cuda, NumericalT> ComplexT;
    CudaSeriesImpl<ComplexT, DefaultAllocator_t<cheetah::Cuda, ComplexT>> _complex;
};

// macro to set up a dummy singleton to ensure the class gets compiled into the lib
#define INSTANTIATE_CUDA_SERIES(_NumericalT) \
    __host__ CudaSeries< _NumericalT > __dummy_implementation_ ## _NumericalT() { return CudaSeries< _NumericalT >(); }; \

// invoke this macro for each type we want to incorporate into the library
INSTANTIATE_CUDA_SERIES(char);
INSTANTIATE_CUDA_SERIES(float);
INSTANTIATE_CUDA_SERIES(double);
INSTANTIATE_CUDA_SERIES(uint8_t);
INSTANTIATE_CUDA_SERIES(uint16_t);


} // namespace data
} // namespace cheetah
} // namespace ska
