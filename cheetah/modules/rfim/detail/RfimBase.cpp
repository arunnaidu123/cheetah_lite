#include "cheetah/modules/rfim/RfimBase.h"


namespace ska {
namespace cheetah {
namespace modules {
namespace rfim {


template<class RfimDetector, class RfimPolicy>
RfimBase<RfimDetector, RfimPolicy>::RfimBase()
{
}

template<class RfimDetector, class RfimPolicy>
RfimBase<RfimDetector, RfimPolicy>::~RfimBase()
{
}

template<class RfimDetector, class RfimPolicy>
template<typename ResourceType>
typename RfimBase<RfimDetector, RfimPolicy>::ReturnType RfimBase<RfimDetector, RfimPolicy>::operator()(ResourceType&& device, DataTypePtr  data)
{
    return _policy.template exec([this, device](DataType& data, DataAdapter& adapter) mutable
                        {
                             static_cast<RfimDetector&>(*this)(device, data, adapter);
                        }, *data);
}

template<class RfimDetector, class RfimPolicy>
typename RfimBase<RfimDetector, RfimPolicy>::ReturnType RfimBase<RfimDetector, RfimPolicy>::operator()(panda::PoolResource<Cpu>&, DataTypePtr data)
{
    return _policy.template exec([this](DataType data, DataAdapter& adapter) mutable
                        {
                             static_cast<RfimDetector&>(*this)(data, adapter);
                        }, *data);
}

template<class RfimDetector, class RfimPolicy>
typename RfimBase<RfimDetector, RfimPolicy>::FlaggedDataReplacerType RfimBase<RfimDetector, RfimPolicy>::data_replacer()
{
     return _data_replacer;
}

} // namespace rfim
} // namespace modules
} // namespace cheetah
} // namespace ska
