namespace ska {
namespace cheetah {
namespace corner_turn {
namespace test {

template <class Arch, typename NumericalT>
TestContainer<Arch, NumericalT>::TestContainer(std::size_t size)
    : _data(size)
{
}


template <class Arch, typename NumericalT>
TestContainer<Arch, NumericalT>::~TestContainer()
{
}

template <class Arch, typename NumericalT>
typename TestContainer<Arch, NumericalT>::Iterator TestContainer<Arch, NumericalT>::begin()
{
    return _data.begin();
}

template <class Arch, typename NumericalT>
typename TestContainer<Arch, NumericalT>::ConstIterator TestContainer<Arch, NumericalT>::begin() const
{
    return _data.begin();
}

template <class Arch, typename NumericalT>
typename TestContainer<Arch, NumericalT>::ConstIterator TestContainer<Arch, NumericalT>::cbegin() const
{
    return _data.cbegin();
}

template <class Arch, typename NumericalT>
typename TestContainer<Arch, NumericalT>::Iterator TestContainer<Arch, NumericalT>::end()
{
    return _data.end();
}

template <class Arch, typename NumericalT>
typename TestContainer<Arch, NumericalT>::ConstIterator TestContainer<Arch, NumericalT>::end() const
{
    return _data.end();
}

template <class Arch, typename NumericalT>
typename TestContainer<Arch, NumericalT>::ConstIterator TestContainer<Arch, NumericalT>::cend() const
{
    return _data.cend();
}

} // namespace test
} // namespace corner_turn
} // namespace cheetah
} // namespace ska