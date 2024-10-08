// Boost.Geometry Index
//
// R-tree query iterators
//
// Copyright (c) 2011-2013 Adam Wulkiewicz, Lodz, Poland.
//
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GEOMETRY_INDEX_DETAIL_RTREE_QUERY_ITERATORS_HPP
#define BOOST_GEOMETRY_INDEX_DETAIL_RTREE_QUERY_ITERATORS_HPP

#define BOOST_GEOMETRY_INDEX_DETAIL_QUERY_ITERATORS_USE_VIRTUAL
//#define BOOST_GEOMETRY_INDEX_DETAIL_QUERY_ITERATORS_USE_FUNCTION
//#define BOOST_GEOMETRY_INDEX_DETAIL_QUERY_ITERATORS_USE_TYPE_ERASURE
//#define BOOST_GEOMETRY_INDEX_DETAIL_QUERY_ITERATORS_USE_MOVE

namespace boost { namespace geometry { namespace index { namespace detail { namespace rtree { namespace iterators {

template <typename Value, typename Allocators>
struct end_query_iterator
{
    typedef std::input_iterator_tag iterator_category;
    typedef Value value_type;
    typedef typename Allocators::const_reference reference;
    typedef typename Allocators::difference_type difference_type;
    typedef typename Allocators::const_pointer pointer;

    reference operator*() const
    {
        BOOST_ASSERT_MSG(false, "iterator not dereferencable");
        pointer p(0);
        return *p;
    }

    const value_type * operator->() const
    {
        BOOST_ASSERT_MSG(false, "iterator not dereferencable");
        const value_type * p = 0;
        return p;
    }

    end_query_iterator & operator++()
    {
        BOOST_ASSERT_MSG(false, "iterator not incrementable");
        return *this;
    }

    end_query_iterator operator++(int)
    {
        BOOST_ASSERT_MSG(false, "iterator not incrementable");
        return *this;
    }

    friend bool operator==(end_query_iterator const& /*l*/, end_query_iterator const& /*r*/)
    {
        return true;
    }
};

template <typename Value, typename Options, typename Translator, typename Box, typename Allocators, typename Predicates>
class spatial_query_iterator
{
    typedef visitors::spatial_query_incremental<Value, Options, Translator, Box, Allocators, Predicates> visitor_type;
    typedef typename visitor_type::node_pointer node_pointer;

public:
    typedef std::input_iterator_tag iterator_category;
    typedef Value value_type;
    typedef typename Allocators::const_reference reference;
    typedef typename Allocators::difference_type difference_type;
    typedef typename Allocators::const_pointer pointer;

    inline spatial_query_iterator(Translator const& t, Predicates const& p)
        : m_visitor(t, p)
    {}

    inline spatial_query_iterator(node_pointer root, Translator const& t, Predicates const& p)
        : m_visitor(t, p)
    {
        m_visitor.initialize(root);
    }

    reference operator*() const
    {
        return m_visitor.dereference();
    }

    const value_type * operator->() const
    {
        return boost::addressof(m_visitor.dereference());
    }

    spatial_query_iterator & operator++()
    {
        m_visitor.increment();
        return *this;
    }

    spatial_query_iterator operator++(int)
    {
        spatial_query_iterator temp = *this;
        this->operator++();
        return temp;
    }

    friend bool operator==(spatial_query_iterator const& l, spatial_query_iterator const& r)
    {
        return l.m_visitor == r.m_visitor;
    }

    friend bool operator==(spatial_query_iterator const& l, end_query_iterator<Value, Allocators> const& /*r*/)
    {
        return l.m_visitor.is_end();
    }

    friend bool operator==(end_query_iterator<Value, Allocators> const& /*l*/, spatial_query_iterator const& r)
    {
        return r.m_visitor.is_end();
    }

private:
    visitor_type m_visitor;
};

template <typename Value, typename Options, typename Translator, typename Box, typename Allocators, typename Predicates, unsigned NearestPredicateIndex>
class distance_query_iterator
{
    typedef visitors::distance_query_incremental<Value, Options, Translator, Box, Allocators, Predicates, NearestPredicateIndex> visitor_type;
    typedef typename visitor_type::node_pointer node_pointer;

public:
    typedef std::input_iterator_tag iterator_category;
    typedef Value value_type;
    typedef typename Allocators::const_reference reference;
    typedef typename Allocators::difference_type difference_type;
    typedef typename Allocators::const_pointer pointer;

    inline distance_query_iterator(Translator const& t, Predicates const& p)
        : m_visitor(t, p)
    {}

    inline distance_query_iterator(node_pointer root, Translator const& t, Predicates const& p)
        : m_visitor(t, p)
    {
        m_visitor.initialize(root);
    }

    reference operator*() const
    {
        return m_visitor.dereference();
    }

    const value_type * operator->() const
    {
        return boost::addressof(m_visitor.dereference());
    }

    distance_query_iterator & operator++()
    {
        m_visitor.increment();
        return *this;
    }

    distance_query_iterator operator++(int)
    {
        distance_query_iterator temp = *this;
        this->operator++();
        return temp;
    }

    friend bool operator==(distance_query_iterator const& l, distance_query_iterator const& r)
    {
        return l.m_visitor == r.m_visitor;
    }

    friend bool operator==(distance_query_iterator const& l, end_query_iterator<Value, Allocators> const& /*r*/)
    {
        return l.m_visitor.is_end();
    }

    friend bool operator==(end_query_iterator<Value, Allocators> const& /*l*/, distance_query_iterator const& r)
    {
        return r.m_visitor.is_end();
    }

private:
    visitor_type m_visitor;
};

template <typename L, typename R>
inline bool operator!=(L const& l, R const& r)
{
    return !(l == r);
}

}}}}}} // namespace boost::geometry::index::detail::rtree::iterators

#if defined(BOOST_GEOMETRY_INDEX_DETAIL_QUERY_ITERATORS_USE_VIRTUAL) || defined(BOOST_GEOMETRY_INDEX_DETAIL_QUERY_ITERATORS_USE_FUNCTION)

#if defined(BOOST_GEOMETRY_INDEX_DETAIL_QUERY_ITERATORS_USE_VIRTUAL)

namespace boost { namespace geometry { namespace index { namespace detail { namespace rtree { namespace iterators {

template <typename Value, typename Allocators>
class query_iterator_base
{
public:
    typedef std::input_iterator_tag iterator_category;
    typedef Value value_type;
    typedef typename Allocators::const_reference reference;
    typedef typename Allocators::difference_type difference_type;
    typedef typename Allocators::const_pointer pointer;

    virtual ~query_iterator_base() {}

    virtual query_iterator_base * clone() const = 0;

    virtual bool is_end() const = 0;
    virtual reference dereference() const = 0;
    virtual void increment() = 0;
    virtual bool equals(query_iterator_base const&) const = 0;
};

template <typename Value, typename Allocators, typename Iterator>
class query_iterator_wrapper
    : public query_iterator_base<Value, Allocators>
{
    typedef query_iterator_base<Value, Allocators> base_t;

public:
    typedef std::input_iterator_tag iterator_category;
    typedef Value value_type;
    typedef typename Allocators::const_reference reference;
    typedef typename Allocators::difference_type difference_type;
    typedef typename Allocators::const_pointer pointer;

    explicit query_iterator_wrapper(Iterator const& it) : m_iterator(it) {}

    virtual base_t * clone() const { return new query_iterator_wrapper(m_iterator); }

    virtual bool is_end() const { return m_iterator == end_query_iterator<Value, Allocators>(); }
    virtual reference dereference() const { return *m_iterator; }
    virtual void increment() { ++m_iterator; }
    virtual bool equals(base_t const& r) const
    {
        const query_iterator_wrapper * p = dynamic_cast<const query_iterator_wrapper *>(boost::addressof(r));
        BOOST_ASSERT_MSG(p, "those iterators can't be compared");
        return m_iterator == p->m_iterator;
    }

private:
    Iterator m_iterator;
};

#elif defined(BOOST_GEOMETRY_INDEX_DETAIL_QUERY_ITERATORS_USE_FUNCTION)

#include <boost/function.hpp>
#include <boost/bind.hpp>

namespace boost { namespace geometry { namespace index { namespace detail { namespace rtree { namespace iterators {

template <typename Value, typename Allocators>
class query_iterator_base
{
public:
    typedef std::input_iterator_tag iterator_category;
    typedef Value value_type;
    typedef typename Allocators::const_reference reference;
    typedef typename Allocators::difference_type difference_type;
    typedef typename Allocators::const_pointer pointer;

    virtual ~query_iterator_base() {}

    boost::function<query_iterator_base*()> clone;
    boost::function<bool()> is_end;
    boost::function<reference()> dereference;
    boost::function<void()> increment;
    boost::function<bool(query_iterator_base const&)> equals;
};

template <typename Value, typename Allocators, typename Iterator>
class query_iterator_wrapper
    : public query_iterator_base<Value, Allocators>
{
    typedef query_iterator_base<Value, Allocators> base_t;

public:
    typedef std::input_iterator_tag iterator_category;
    typedef Value value_type;
    typedef typename Allocators::const_reference reference;
    typedef typename Allocators::difference_type difference_type;
    typedef typename Allocators::const_pointer pointer;

    explicit query_iterator_wrapper(Iterator const& it)
        : m_iterator(it)
    {
        base_t::clone = boost::bind(&query_iterator_wrapper::clone_, this);
        base_t::is_end = boost::bind(&query_iterator_wrapper::is_end_, this);
        base_t::dereference = boost::bind(&query_iterator_wrapper::dereference_, this);
        base_t::increment = boost::bind(&query_iterator_wrapper::increment_, this);
        base_t::equals = boost::bind(&query_iterator_wrapper::equals_, this, _1);
    }

private:
    base_t * clone_() const { return new query_iterator_wrapper(m_iterator); }

    bool is_end_() const { return m_iterator == end_query_iterator<Value, Allocators>(); }
    reference dereference_() const { return *m_iterator; }
    void increment_() { ++m_iterator; }
    bool equals_(base_t const& r) const
    {
        const query_iterator_wrapper * p = dynamic_cast<const query_iterator_wrapper *>(boost::addressof(r));
        BOOST_ASSERT_MSG(p, "those iterators can't be compared");
        return m_iterator == p->m_iterator;
    }

private:
    Iterator m_iterator;
};

#endif

template <typename Value, typename Allocators>
class query_iterator
{
    typedef query_iterator_base<Value, Allocators> iterator_base;
    typedef std::auto_ptr<iterator_base> iterator_ptr;

public:
    typedef std::input_iterator_tag iterator_category;
    typedef Value value_type;
    typedef typename Allocators::const_reference reference;
    typedef typename Allocators::difference_type difference_type;
    typedef typename Allocators::const_pointer pointer;

    query_iterator() {}

    template <typename It>
    query_iterator(It const& it)
        : m_ptr(static_cast<iterator_base*>(
                    new query_iterator_wrapper<Value, Allocators, It>(it) ))
    {}

    query_iterator(end_query_iterator<Value, Allocators> const& /*it*/)
    {}

    query_iterator(query_iterator const& o)
        : m_ptr(o.m_ptr.get() ? o.m_ptr->clone() : 0)
    {}

#ifndef BOOST_GEOMETRY_INDEX_DETAIL_QUERY_ITERATORS_USE_MOVE
    query_iterator & operator=(query_iterator const& o)
    {
        m_ptr.reset(o.m_ptr.get() ? o.m_ptr->clone() : 0);
        return *this;
    }
#ifndef BOOST_NO_CXX11_RVALUE_REFERENCES
    query_iterator(query_iterator && o)
        : m_ptr(o.m_ptr.get())
    {
        o.m_ptr.release();
    }
    query_iterator & operator=(query_iterator && o)
    {
        if ( this != boost::addressof(o) )
        {
            m_ptr.reset(o.m_ptr.get());
            o.m_ptr.release();
        }
        return *this;
    }
#endif
#else // !BOOST_GEOMETRY_INDEX_DETAIL_QUERY_ITERATORS_USE_MOVE
private:
    BOOST_COPYABLE_AND_MOVABLE(query_iterator)
public:
    query_iterator & operator=(BOOST_COPY_ASSIGN_REF(query_iterator) o)
    {
        m_ptr.reset(o.m_ptr.get() ? o.m_ptr->clone() : 0);
        return *this;
    }
    query_iterator(BOOST_RV_REF(query_iterator) o)
        : m_ptr(o.m_ptr.get())
    {
        o.m_ptr.release();
    }
    query_iterator & operator=(BOOST_RV_REF(query_iterator) o)
    {
        if ( this != boost::addressof(o) )
        {
            m_ptr.reset(o.m_ptr.get());
            o.m_ptr.release();
        }
        return *this;
    }
#endif // BOOST_GEOMETRY_INDEX_DETAIL_QUERY_ITERATORS_USE_MOVE

    reference operator*() const
    {
        return m_ptr->dereference();
    }

    const value_type * operator->() const
    {
        return boost::addressof(m_ptr->dereference());
    }

    query_iterator & operator++()
    {
        m_ptr->increment();
        return *this;
    }

    query_iterator operator++(int)
    {
        query_iterator temp = *this;
        this->operator++();
        return temp;
    }

    friend bool operator==(query_iterator const& l, query_iterator const& r)
    {
        if ( l.m_ptr.get() )
        {
            if ( r.m_ptr.get() )
                return l.m_ptr->equals(*r.m_ptr);
            else
                return l.m_ptr->is_end();
        }
        else
        {
            if ( r.m_ptr.get() )
                return r.m_ptr->is_end();
            else
                return true;
        }
    }

private:
    iterator_ptr m_ptr;
};

}}}}}} // namespace boost::geometry::index::detail::rtree::iterators

#elif defined(BOOST_GEOMETRY_INDEX_DETAIL_QUERY_ITERATORS_USE_TYPE_ERASURE)

#include <boost/type_erasure/any.hpp>
#include <boost/type_erasure/operators.hpp>
#include <boost/type_erasure/is_empty.hpp>

namespace boost { namespace geometry { namespace index { namespace detail { namespace rtree { namespace iterators {

template<typename T, typename Value, typename Allocators>
struct single_pass_iterator_concept :
    ::boost::mpl::vector<
        ::boost::type_erasure::copy_constructible<T>,
        ::boost::type_erasure::equality_comparable<T>,
        ::boost::type_erasure::dereferenceable<typename Allocators::const_reference, T>,
        ::boost::type_erasure::assignable<T>,
        ::boost::type_erasure::incrementable<T>,
        ::boost::type_erasure::equality_comparable<T, end_query_iterator<Value, Allocators> >,
        ::boost::type_erasure::relaxed // default ctor
    >
{};

template <typename Value, typename Allocators>
struct single_pass_iterator_type
{
    typedef ::boost::type_erasure::any<
        single_pass_iterator_concept<
            ::boost::type_erasure::_self, Value, Allocators
        >
    > type;
};

}}}}}} // namespace boost::geometry::index::detail::rtree::iterators

namespace boost { namespace type_erasure {

template<typename T, typename Value, typename Allocators, typename Base>
struct concept_interface<
    ::boost::geometry::index::detail::rtree::single_pass_iterator_concept<
        T, Value, Allocators
    >, Base, T>
    : Base
{
    typedef Value value_type;
    typedef typename Allocators::const_reference reference;
    typedef typename Allocators::const_pointer pointer;
    typedef typename Allocators::difference_type difference_type;
    typedef ::std::input_iterator_tag iterator_category;
};

}} // boost::type_erasure

namespace boost { namespace geometry { namespace index { namespace detail { namespace rtree { namespace iterators {

template <typename Value, typename Allocators>
class query_iterator
{
public:
    typedef std::input_iterator_tag iterator_category;
    typedef Value value_type;
    typedef typename Allocators::const_reference reference;
    typedef typename Allocators::difference_type difference_type;
    typedef typename Allocators::const_pointer pointer;

private:
    typedef typename rtree::single_pass_iterator_type<Value, Allocators>::type iterator_type;

public:

    query_iterator() {}

    template <typename It>
    query_iterator(It const& it)
        : m_iterator(it)
    {}

    query_iterator(end_query_iterator<Value, Allocators> const& /*it*/)
    {}

#ifdef BOOST_GEOMETRY_INDEX_DETAIL_QUERY_ITERATORS_USE_MOVE
private:
    BOOST_COPYABLE_AND_MOVABLE(query_iterator)
public:
    query_iterator(query_iterator const& o)
        : m_iterator(o.m_iterator)
    {}
    query_iterator & operator=(BOOST_COPY_ASSIGN_REF(query_iterator) o)
    {
        m_iterator = o.m_iterator;
        return *this;
    }
    query_iterator(BOOST_RV_REF(query_iterator) o)
        : m_iterator(boost::move(o.m_iterator))
    {}
    query_iterator & operator=(BOOST_RV_REF(query_iterator) o)
    {
        if ( this != boost::addressof(o) )
        {
            m_iterator = boost::move(o.m_iterator);
        }
        return *this;
    }
#endif // BOOST_GEOMETRY_INDEX_DETAIL_QUERY_ITERATORS_USE_MOVE

    reference operator*() const
    {
        return *m_iterator;
    }

    const value_type * operator->() const
    {
        return boost::addressof(*m_iterator);
    }

    query_iterator & operator++()
    {
        ++m_iterator;
        return *this;
    }

    query_iterator operator++(int)
    {
        query_iterator temp = *this;
        ++m_iterator;
        return temp;
    }

    friend bool operator==(query_iterator const& l, query_iterator const& r)
    {
        if ( !::boost::type_erasure::is_empty(l.m_iterator) )
        {
            if ( !::boost::type_erasure::is_empty(r.m_iterator) )
                return l.m_iterator == r.m_iterator;
            else
                return l.m_iterator == end_query_iterator<Value, Allocators>();
        }
        else
        {
            if ( !::boost::type_erasure::is_empty(r.m_iterator) )
                return r.m_iterator == end_query_iterator<Value, Allocators>();
            else
                return true;
        }
    }

private:
    iterator_type m_iterator;
};

}}}}}} // namespace boost::geometry::index::detail::rtree::iterators

#endif // BOOST_GEOMETRY_INDEX_DETAIL_QUERY_ITERATORS_USE_TYPE_ERASURE

#endif // BOOST_GEOMETRY_INDEX_DETAIL_RTREE_QUERY_ITERATORS_HPP
