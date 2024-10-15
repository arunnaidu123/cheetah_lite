#include "cheetah/data/DmTrialsMetadata.h"
#include "panda/Error.h"

namespace ska {
namespace cheetah {
namespace data {

DmTrialsMetadata::Metadata::Metadata(DmTrialsMetadata const& parent, typename DmTrialsMetadata::DmType dm, std::size_t downsampling_factor)
    : _parent(parent)
    , _dm(dm)
    , _downsampling_factor(downsampling_factor)
{

}

DmTrialsMetadata::Metadata::~Metadata()
{

}

typename DmTrialsMetadata::DmType const& DmTrialsMetadata::Metadata::dm() const
{
    return _dm;
}

typename DmTrialsMetadata::TimeType DmTrialsMetadata::Metadata::sampling_interval() const
{
    return _downsampling_factor * _parent._sampling_interval;
}

std::size_t  DmTrialsMetadata::Metadata::size() const
{
    return _parent._number_of_samples/_downsampling_factor;
}

std::size_t  DmTrialsMetadata::Metadata::downsampling_factor() const
{
    return _downsampling_factor;
}

DmTrialsMetadata::DmTrialsMetadata(TimeType fundamental_sampling_interval, std::size_t fundamental_sample_count)
    : _sampling_interval(fundamental_sampling_interval)
    , _number_of_samples(fundamental_sample_count)
    , _hash_value(0)
{
    boost::hash_combine(_hash_value,_sampling_interval.value());
}

DmTrialsMetadata::~DmTrialsMetadata()
{

}

void DmTrialsMetadata::emplace_back(DmType dm, std::size_t downsampling_factor)
{
    if (_number_of_samples % downsampling_factor != 0)
        throw panda::Error("Downsampling factor does not perfectly divide fundamental sample count.");
    boost::hash_combine(_hash_value,dm.value());
    boost::hash_combine(_hash_value,downsampling_factor);
    _metadata.emplace_back(*this,dm,downsampling_factor);
}

typename DmTrialsMetadata::ContainerType::const_reference DmTrialsMetadata::operator[](std::size_t n) const
{
    return _metadata[n];
}

typename DmTrialsMetadata::ContainerType::reference DmTrialsMetadata::operator[](std::size_t n)
{
    return _metadata[n];
}

typename DmTrialsMetadata::ContainerType::iterator DmTrialsMetadata::begin()
{
    return _metadata.begin();
}

typename DmTrialsMetadata::ContainerType::const_iterator DmTrialsMetadata::begin() const
{
    return _metadata.begin();
}

typename DmTrialsMetadata::ContainerType::const_iterator DmTrialsMetadata::cbegin() const
{
    return _metadata.cbegin();
}

typename DmTrialsMetadata::ContainerType::iterator DmTrialsMetadata::end()
{
    return _metadata.end();
}

typename DmTrialsMetadata::ContainerType::const_iterator DmTrialsMetadata::end() const
{
    return _metadata.end();
}

typename DmTrialsMetadata::ContainerType::const_iterator DmTrialsMetadata::cend() const
{
    return _metadata.cend();
}

std::size_t DmTrialsMetadata::size() const
{
    return _metadata.size();
}

bool DmTrialsMetadata::operator==(DmTrialsMetadata const& other)
{
    return _hash_value == other._hash_value;
}

typename DmTrialsMetadata::TimeType DmTrialsMetadata::duration() const
{
    return _number_of_samples * _sampling_interval;
}

typename DmTrialsMetadata::TimeType DmTrialsMetadata::fundamental_sampling_interval() const
{
    return _sampling_interval;
}

std::size_t DmTrialsMetadata::number_of_samples() const
{
    return _number_of_samples;
}

std::size_t  DmTrialsMetadata::total_data_size() const
{
    std::size_t total_size=0;
    for (auto const& trial: *this)
    {
        total_size+=trial.size();
    }
    return total_size;
}

std::vector<unsigned> DmTrialsMetadata::number_of_ranges() const
{
    std::vector<unsigned> nranges;
    unsigned ndms = 0;
    unsigned samples = _metadata[0].size();
    for(unsigned int dm=0; dm<_metadata.size(); ++dm)
    {
        if(_metadata[dm].size()!=samples)
        {
            samples = _metadata[dm].size();
            nranges.push_back(ndms);
            ndms=0;
        }
        ndms += 1;
    }
    nranges.push_back(ndms);
    return nranges;
}

} // namespace data
} // namespace cheetah
} // namespace ska
