//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Zero-copy vector view container.
 */

#pragma once

#include <pycpp/stl/algorithm.h>
#include <pycpp/stl/stdexcept.h>
#include <pycpp/stl/vector.h>
#include <assert.h>
#include <string.h>

PYCPP_BEGIN_NAMESPACE

// DECLARATION
// -----------


/**
 *  \brief STL vector wrapper.
 *
 *  Binds a pointer and the vector length, accepting either a C++
 *  vector, a null-terminated vector, a character array and length,
 *  or an begin/end pointer pair.
 *
 *  \warning The lifetime of the source data must outlive the wrapper.
 *  The wrapper has **no** ownership, and is merely an STL-like wrapper
 *  for performance reasons.
 */
template <typename T>
class vector_view
{
public:
    // MEMBER TYPES
    // ------------
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using iterator = pointer;
    using const_iterator = const_pointer;
    using reverse_iterator = PYCPP_NAMESPACE::reverse_iterator<pointer>;
    using const_reverse_iterator = PYCPP_NAMESPACE::reverse_iterator<const_pointer>;

    // MEMBER FUNCTIONS
    // ----------------
    vector_view() noexcept = default;
    vector_view(const vector_view<T>& vec) noexcept;
    vector_view<T>& operator=(const vector_view<T>& vec) noexcept;
    vector_view(vector_view<T>&& vec) noexcept;
    vector_view<T>& operator=(vector_view<T>&& vec) noexcept;

    template <typename A> vector_view(const vector<T, A>& vec) noexcept;
    template <typename A> vector_view<T>& operator=(const vector<T, A>& vec) noexcept;
    vector_view(pointer t, size_type n) noexcept;
    vector_view(pointer first, pointer last) noexcept;

    // ITERATORS
    const_iterator begin() const noexcept;
    const_iterator end() const noexcept;
    const_reverse_iterator rbegin() const noexcept;
    const_reverse_iterator rend() const noexcept;
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;
    const_reverse_iterator crbegin() const noexcept;
    const_reverse_iterator crend() const noexcept;

    // CAPACITY
    size_t size() const noexcept;
    bool empty() const noexcept;

    // ELEMENT ACCESS
    const_reference operator[](size_type pos) const;
    const_reference at(size_type pos) const;
    const_reference front() const;
    const_reference back() const;
    const_pointer data() const noexcept;

    // MODIFIERS
    void swap(vector_view<T>& rhs) noexcept;
    void remove_prefix(size_type n) noexcept;
    void remove_suffix(size_type n) noexcept;

    // OPERATIONS
    size_type copy(value_type* dst, size_type count, size_type pos = 0);

    // CONVERSIONS
    explicit operator bool() const noexcept;
    template <typename A> explicit operator vector<T, A>() const;

private:
    const_pointer data_ = nullptr;
    size_t size_ = 0;

    // NON-MEMBER FUNCTIONS
    // --------------------
    template <typename U>
    friend void swap(vector_view<U> &lhs, vector_view<U> &right);

    // RELATIONAL OPERATORS
    template <typename U>
    friend bool operator==(const vector_view<U>& lhs, const vector_view<U>& right) noexcept;

    template <typename U, typename A>
    friend bool operator==(const vector<U, A>& lhs, const vector_view<U>& right) noexcept;

    template <typename U, typename A>
    friend bool operator==(const vector_view<U>& lhs, const vector<U, A>& right) noexcept;

    template <typename U>
    friend bool operator!=(const vector_view<U>& lhs, const vector_view<U>& right) noexcept;

    template <typename U, typename A>
    friend bool operator!=(const vector<U, A>& lhs, const vector_view<U>& right) noexcept;

    template <typename U, typename A>
    friend bool operator!=(const vector_view<U>& lhs, const vector<U, A>& right) noexcept;

    template <typename U>
    friend bool operator<(const vector_view<U>& lhs, const vector_view<U>& right) noexcept;

    template <typename U, typename A>
    friend bool operator<(const vector<U, A>& lhs, const vector_view<U>& right) noexcept;

    template <typename U, typename A>
    friend bool operator<(const vector_view<U>& lhs, const vector<U, A>& right) noexcept;

    template <typename U>
    friend bool operator<=(const vector_view<U>& lhs, const vector_view<U>& right) noexcept;

    template <typename U, typename A>
    friend bool operator<=(const vector<U, A>& lhs, const vector_view<U>& right) noexcept;

    template <typename U, typename A>
    friend bool operator<=(const vector_view<U>& lhs, const vector<U, A>& right) noexcept;

    template <typename U>
    friend bool operator>(const vector_view<U>& lhs, const vector_view<U>& right) noexcept;

    template <typename U, typename A>
    friend bool operator>(const vector<U, A>& lhs, const vector_view<U>& right) noexcept;

    template <typename U, typename A>
    friend bool operator>(const vector_view<U>& lhs, const vector<U, A>& right) noexcept;

    template <typename U>
    friend bool operator>=(const vector_view<U>& lhs, const vector_view<U>& right) noexcept;

    template <typename U, typename A>
    friend bool operator>=(const vector<U, A>& lhs, const vector_view<U>& right) noexcept;

    template <typename U, typename A>
    friend bool operator>=(const vector_view<U>& lhs, const vector<U, A>& right) noexcept;
};

// SPECIALIZATION
// --------------

template <typename T>
struct is_relocatable<vector_view<T>>: true_type
{};

// IMPLEMENTATION
// --------------


template <typename T>
void swap(vector_view<T>& lhs, vector_view<T>& rhs)
{
    lhs.swap(rhs);
}


template <typename T>
bool operator==(const vector_view<T>& lhs, const vector_view<T>& rhs) noexcept
{
    return lhs.size() == rhs.size() && equal(lhs.begin(), lhs.end(), rhs.begin());
}


template <typename T, typename A>
bool operator==(const vector<T, A>& lhs, const vector_view<T>& rhs) noexcept
{
    return vector_view<T>(lhs) == rhs;
}


template <typename T, typename A>
bool operator==(const vector_view<T>& lhs, const vector<T, A>& rhs) noexcept
{
    return lhs == vector_view<T>(rhs);
}


template <typename T>
bool operator!=(const vector_view<T>& lhs, const vector_view<T>& rhs) noexcept
{
    return !(lhs == rhs);
}


template <typename T, typename A>
bool operator!=(const vector<T, A>& lhs, const vector_view<T>& rhs) noexcept
{
    return vector_view<T>(lhs) != rhs;
}


template <typename T, typename A>
bool operator!=(const vector_view<T>& lhs, const vector<T, A>& rhs) noexcept
{
    return lhs != vector_view<T>(rhs);
}


template <typename T>
bool operator<(const vector_view<T>& lhs, const vector_view<T>& rhs) noexcept
{
    return lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}


template <typename T, typename A>
bool operator<(const vector<T, A>& lhs, const vector_view<T>& rhs) noexcept
{
    return vector_view<T>(lhs) < rhs;
}


template <typename T, typename A>
bool operator<(const vector_view<T>& lhs, const vector<T, A> &rhs) noexcept
{
    return lhs < vector_view<T>(rhs);
}


template <typename T>
bool operator<=(const vector_view<T>& lhs, const vector_view<T>& rhs) noexcept
{
    return !(rhs < lhs);
}


template <typename T, typename A>
bool operator<=(const vector<T, A>& lhs, const vector_view<T>& rhs) noexcept
{
    return vector_view<T>(lhs) <= rhs;
}


template <typename T, typename A>
bool operator<=(const vector_view<T>& lhs, const vector<T, A>& rhs) noexcept
{
    return lhs <= vector_view<T>(rhs);
}


template <typename T>
bool operator>(const vector_view<T>& lhs, const vector_view<T>& rhs) noexcept
{
    return rhs < lhs;
}


template <typename T, typename A>
bool operator>(const vector<T, A> &lhs, const vector_view<T> &rhs) noexcept
{
    return vector_view<T>(lhs) > rhs;
}


template <typename T, typename A>
bool operator>(const vector_view<T> &lhs, const vector<T, A> &rhs) noexcept
{
    return lhs > vector_view<T>(rhs);
}


template <typename T>
bool operator>=(const vector_view<T> &lhs, const vector_view<T> &rhs) noexcept
{
    return !(lhs < rhs);
}


template <typename T, typename A>
bool operator>=(const vector<T, A> &lhs, const vector_view<T> &rhs) noexcept
{
    return vector_view<T>(lhs) >= rhs;
}


template <typename T, typename A>
bool operator>=(const vector_view<T> &lhs, const vector<T, A> &rhs) noexcept
{
    return lhs >= vector_view<T>(rhs);
}


template <typename T>
vector_view<T>::vector_view(const vector_view<T>& vec) noexcept:
    data_(vec.data()),
    size_(vec.size())
{}


template <typename T>
auto vector_view<T>::operator=(const vector_view<T>& vec) noexcept -> vector_view<T>&
{
    data_ = vec.data();
    size_ = vec.size();
    return *this;
}


template <typename T>
vector_view<T>::vector_view(vector_view<T>&& vec) noexcept
{
    swap(vec);
}


template <typename T>
auto vector_view<T>::operator=(vector_view<T>&& vec) noexcept -> vector_view<T>&
{
    swap(vec);
    return *this;
}

template <typename T>
template <typename A>
vector_view<T>::vector_view(const vector<T, A>& vec) noexcept:
    data_(vec.data()),
    size_(vec.size())
{}


template <typename T>
template <typename A>
auto vector_view<T>::operator=(const vector<T, A>& vec) noexcept -> vector_view<T>&
{
    data_ = vec.data();
    size_ = vec.size();
    return *this;
}


template <typename T>
vector_view<T>::vector_view(pointer t, size_type n) noexcept:
    data_(t),
    size_(n)
{}


template <typename T>
vector_view<T>::vector_view(pointer first, pointer last) noexcept:
    data_(first),
    size_(last - first)
{}


template <typename T>
auto vector_view<T>::begin() const noexcept -> const_iterator
{
    return data_;
}


template <typename T>
auto vector_view<T>::end() const noexcept -> const_iterator
{
    return data_ + size_;
}


template <typename T>
auto vector_view<T>::rbegin() const noexcept -> const_reverse_iterator
{
    return const_reverse_iterator(end());
}


template <typename T>
auto vector_view<T>::rend() const noexcept -> const_reverse_iterator
{
    return const_reverse_iterator(begin());
}


template <typename T>
auto vector_view<T>::cbegin() const noexcept -> const_iterator
{
    return begin();
}


template <typename T>
auto vector_view<T>::cend() const noexcept -> const_iterator
{
    return end();
}


template <typename T>
auto vector_view<T>::crbegin() const noexcept -> const_reverse_iterator
{
    return rbegin();
}


template <typename T>
auto vector_view<T>::crend() const noexcept -> const_reverse_iterator
{
    return rend();
}


template <typename T>
size_t vector_view<T>::size() const noexcept
{
    return size_;
}


template <typename T>
bool vector_view<T>::empty() const noexcept
{
    return size_ == 0;
}


template <typename T>
auto vector_view<T>::operator[](size_type pos) const -> const_reference
{
    assert(pos <= size() && "vector index out of bounds");
    return *(data_ + pos);
}


template <typename T>
auto vector_view<T>::at(size_type pos) const -> const_reference
{
    return operator[](pos);
}


template <typename T>
auto vector_view<T>::front() const -> const_reference
{
    assert(!empty() && "vector::front(): vector is empty");
    return *data_;
}


template <typename T>
auto vector_view<T>::back() const -> const_reference
{
    assert(!empty() && "vector::back(): vector is empty");
    return *(data_ + size_ - 1);
}


template <typename T>
auto vector_view<T>::data() const noexcept -> const_pointer
{
    return data_;
}


template <typename T>
void vector_view<T>::swap(vector_view<T>& rhs) noexcept
{
    using PYCPP_NAMESPACE::swap;
    swap(data_, rhs.data_);
    swap(size_, rhs.size_);
}


template <typename T>
void vector_view<T>::remove_prefix(size_type n) noexcept
{
    assert(n <= size() && "vector_view::remove_prefix greater than size.");
    data_ += n;
    size_ -= n;
}


template <typename T>
void vector_view<T>::remove_suffix(size_type n) noexcept
{
    assert(n <= size() && "vector_view::remove_suffix greater than size.");
    size_ -= n;
}


template <typename T>
auto vector_view<T>::copy(value_type* dst, size_type count, size_type pos) -> size_type
{
    if (pos > size()) {
        throw out_of_range("vector_view::copy");
    }

    size_type length = min(count, size() - pos);
    // can pass 0 to memcpy
    memcpy(dst, data() + pos, length * sizeof(value_type));

    return length;
}


template <typename T>
vector_view<T>::operator bool() const noexcept
{
    return !empty();
}


template <typename T>
template <typename A>
vector_view<T>::operator vector<T, A>() const
{
    return vector<T, A>(begin(), end());
}

PYCPP_END_NAMESPACE
