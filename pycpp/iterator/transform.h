//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Iterator utilities.
 */

#pragma once

#include <pycpp/iterator/category.h>
#include <pycpp/misc/ordering.h>
#include <pycpp/stl/tuple.h>
#include <pycpp/stl/type_traits.h>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------

/**
 *  \brief Transformation iterator implied base class.
 */
template <typename Iterator, typename UnaryFunction>
struct traits_impl
{
    // MEMBER TYPES
    // ------------
    using traits_type = iterator_traits<Iterator>;
    using value_type = result_of_t<const UnaryFunction(typename traits_type::reference)>;
    using iterator_category = typename traits_type::iterator_category;
    using difference_type = typename traits_type::difference_type;
    using reference = value_type&;
    using pointer = remove_reference_t<value_type>*;
};


/**
 *  \brief Base class for transform iterator.
 */
template <typename Iterator, typename UnaryFunction>
struct transform_iterator_base:
    iterator<
        typename traits_impl<Iterator, UnaryFunction>::iterator_category,
        typename traits_impl<Iterator, UnaryFunction>::value_type,
        typename traits_impl<Iterator, UnaryFunction>::difference_type,
        typename traits_impl<Iterator, UnaryFunction>::pointer,
        typename traits_impl<Iterator, UnaryFunction>::reference
    >
{
    // MEMBER TYPES
    // ------------
    using traits_type = traits_impl<Iterator, UnaryFunction>;

    // SFINAE
    // ------
    // TYPES
    static constexpr bool is_input = is_input_iterator<Iterator>::value;
    static constexpr bool is_output = is_output_iterator<Iterator>::value;
    static constexpr bool is_forward = is_forward_iterator<Iterator>::value;
    static constexpr bool is_bidirectional = is_bidirectional_iterator<Iterator>::value;
    static constexpr bool is_random_access = is_random_access_iterator<Iterator>::value;

    // FEATURES
    static constexpr bool has_total = is_random_access;
    static constexpr bool has_decrement = is_bidirectional || is_random_access;
    static constexpr bool has_indexing = is_random_access;
    static constexpr bool has_arithmetic = is_random_access;
};


/**
 *  \brief Iterator transformations.
 */
template <typename Iterator, typename UnaryFunction>
struct transform_iterator: transform_iterator_base<Iterator, UnaryFunction>
{
public:
    // MEMBER TEMPLATES
    // ----------------
    template <typename It, typename F>
    using rebind = transform_iterator<It, F>;

    // MEMBER TYPES
    // ------------
    using base_t = transform_iterator_base<Iterator, UnaryFunction>;
    using self_t = transform_iterator<Iterator, UnaryFunction>;
    using typename base_t::value_type;
    using typename base_t::reference;
    using typename base_t::pointer;
    using typename base_t::difference_type;

    // MEMBER FUNCTIONS
    // ----------------
    transform_iterator();
    transform_iterator(const self_t&) = default;
    self_t& operator=(const self_t&) = default;
    transform_iterator(self_t&&) = default;
    self_t& operator=(self_t&&) = default;
    transform_iterator(const Iterator& it, UnaryFunction function);

    // BASE
    UnaryFunction functor() const;
    const Iterator& base() const;
    void swap(self_t&);

    // GLOBAL OPERATORS
    bool operator==(const self_t&) const;
    bool operator!=(const self_t&) const;
    self_t& operator++();
    self_t operator++(int);
    value_type operator*();
    value_type operator*() const;

    // CONDITIONAL OPERATORS

    template <typename It, typename F>
    enable_if_t<rebind<It, F>::has_total, bool> operator<(const rebind<It, F>& other) const
    {
        return it < other.it;
    }

    template <typename It, typename F>
    enable_if_t<rebind<It, F>::has_total, bool> operator<=(const rebind<It, F>& other) const
    {
        return ordering::less_equal(*this, other);
    }

    template <typename It, typename F>
    enable_if_t<rebind<It, F>::has_total, bool> operator>(const rebind<It, F>& other) const
    {
        return ordering::greater(*this, other);
    }

    template <typename It, typename F>
    enable_if_t<rebind<It, F>::has_total, bool> operator>=(const rebind<It, F>& other) const
    {
        return ordering::greater_equal(*this, other);
    }

    template <typename It = Iterator, typename F = UnaryFunction>
    enable_if_t<rebind<It, F>::has_decrement, rebind<It, F>&> operator--()
    {
        --it;
        return *this;
    }

    template <typename It = Iterator, typename F = UnaryFunction>
    enable_if_t<rebind<It, F>::has_decrement, rebind<It, F>> operator--(int)
    {
        self_t copy(*this);
        operator--();
        return copy;
    }

    template <typename It = Iterator, typename F = UnaryFunction>
    enable_if_t<rebind<It, F>::has_indexing, typename rebind<It, F>::value_type> operator[](size_t pos) const
    {
        return function(it[pos]);
    }

    template <typename It = Iterator, typename F = UnaryFunction>
    enable_if_t<rebind<It, F>::has_arithmetic, rebind<It, F>&> operator+=(difference_type n)
    {
        it += n;
        return *this;
    }

    template <typename It = Iterator, typename F = UnaryFunction>
    enable_if_t<rebind<It, F>::has_arithmetic, rebind<It, F>&> operator-=(difference_type n)
    {
        it -= n;
        return *this;
    }

    template <typename It = Iterator, typename F = UnaryFunction>
    enable_if_t<rebind<It, F>::has_arithmetic, rebind<It, F>> operator+(difference_type n) const
    {
        self_t copy(*this);
        copy += n;
        return copy;
    }

    template <typename It = Iterator, typename F = UnaryFunction>
    enable_if_t<rebind<It, F>::has_arithmetic, rebind<It, F>> operator-(difference_type n) const
    {
        self_t copy(*this);
        copy -= n;
        return copy;
    }

    template <typename It = Iterator, typename F = UnaryFunction>
    enable_if_t<rebind<It, F>::has_arithmetic, difference_type> operator-(const rebind<It, F>& rhs) const
    {
        return it - rhs.it;
    }

private:
    Iterator it;
    UnaryFunction function;
};


// FUNCTIONS
// ---------


template <typename It, typename F>
transform_iterator<It, F> make_transform_iterator(const It& it, F function)
{
    return transform_iterator<It, F>(it, function);
}


// IMPLEMENTATION
// --------------

template <typename It, typename F>
transform_iterator<It, F>::transform_iterator()
{}


template <typename It, typename F>
transform_iterator<It, F>::transform_iterator(const It& it, F function):
    it(it),
    function(function)
{}


template <typename It, typename F>
F transform_iterator<It, F>::functor() const
{
    return function;
}


template <typename It, typename F>
const It& transform_iterator<It, F>::base() const
{
    return it;
}


template <typename It, typename F>
void transform_iterator<It, F>::swap(self_t& other)
{
    using PYCPP_NAMESPACE::swap;
    swap(it, other.it);
    swap(function, other.function);
}


template <typename It, typename F>
bool transform_iterator<It, F>::operator==(const self_t& other) const
{
    return it == other.it;
}

template <typename It, typename F>
bool transform_iterator<It, F>::operator!=(const self_t& other) const
{
    return ordering::not_equal_to(*this, other);
}


template <typename It, typename F>
auto transform_iterator<It, F>::operator++() -> self_t&
{
    ++it;
    return *this;
}


template <typename It, typename F>
auto transform_iterator<It, F>::operator++(int) -> self_t
{
    self_t copy(*this);
    operator++();
    return copy;
}


template <typename It, typename F>
auto transform_iterator<It, F>::operator*() -> value_type
{
    return function(*it);
}


template <typename It, typename F>
auto transform_iterator<It, F>::operator*() const -> value_type
{
    return function(*it);
}

PYCPP_END_NAMESPACE
