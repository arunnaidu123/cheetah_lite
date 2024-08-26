#ifndef SKA_CHEETAH_MODULES_RFIM_RFIMBASE_H
#define SKA_CHEETAH_MODULES_RFIM_RFIMBASE_H

#include "cheetah/modules/rfim/PolicyInfo.h"
#include "cheetah/modules/rfim/policy/Policy.h"
#include "cheetah/modules/rfim/FlaggedDataReplacer.h"

namespace ska {
namespace cheetah {
namespace modules {
namespace rfim {

/**
 * @brief
 *   Wrap a Rfim flaggeing style detector and adjust the data according to the flags and a specific policy
 *
 * @details
 *
 */

template<class RfimDetector, class RfimPolicy>
class RfimBase
{
    protected:
        typedef PolicyInfo<RfimPolicy> InfoType;
        typedef typename InfoType::DataArgumentType DataType;
        typedef typename std::shared_ptr<typename std::remove_reference<DataType>::type> DataTypePtr;
        typedef typename std::remove_reference<DataType>::type TimeFrequencyDataType;

    public:
        typedef typename InfoType::AdapterType DataAdapter;
        typedef typename InfoType::ReturnType ReturnType;
        typedef FlaggedDataReplacer<TimeFrequencyDataType> FlaggedDataReplacerType;

    public:
        RfimBase();
        ~RfimBase();

        template<typename ResourceType>
        ReturnType operator()(ResourceType&&, DataTypePtr data);

        ReturnType operator()(panda::PoolResource<Cpu>&, DataTypePtr data );
        FlaggedDataReplacerType data_replacer();

    private:
        policy::Policy<RfimPolicy> _policy;
        FlaggedDataReplacerType _data_replacer;
};


} // namespace rfim
} // namespace modules
} // namespace cheetah
} // namespace ska
#include "cheetah/modules/rfim/detail/RfimBase.cpp"

#endif // SKA_CHEETAH_MODULES_RFIM_RFIMBASE_H
