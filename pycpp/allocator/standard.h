//  :copyright: (c) 2016 Mariano Trebino.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Standard allocator analogous to `std::allocator`.
 *
 *  A shallow wrapper around `malloc` and `free`. This allocator
 *  has poor performance, and therefore should be used sparingly.
 */

#pragma once

#include <pycpp/config.h>
#include <cstddef>
#include <memory>

PYCPP_BEGIN_NAMESPACE

// OBJECTS
// -------

/**
 *  \brief Base for standard memory allocator.
 */
struct standard_allocator_base
{
    void* allocate(size_t n, size_t size, const void* hint = nullptr);
    void deallocate(void* p, size_t n);
};


/**
 *  \brief Standard memory allocator.
 */
template <typename T>
struct standard_allocator: private standard_allocator_base
{
    // MEMBER TYPES
    // ------------
    using self_t = standard_allocator<T>;
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = size_t;
    using difference_type = std::ptrdiff_t;

    // MEMBER FUNCTIONS
    // ----------------
    standard_allocator() noexcept = default;
    standard_allocator(const standard_allocator<T>&) noexcept = default;
    standard_allocator<T>& operator=(const standard_allocator<T>&) noexcept = default;
    ~standard_allocator() = default;

    pointer allocate(size_type, const void* = nullptr);
    void deallocate(pointer, size_type);
};

// IMPLEMENTATION
// --------------


template <typename T>
auto standard_allocator<T>::allocate(size_type n, const void* hint) -> pointer
{
    return reinterpret_cast<pointer>(standard_allocator_base::allocate(n, sizeof(value_type), hint));
}


template <typename T>
void standard_allocator<T>::deallocate(pointer p, size_type n)
{
    standard_allocator_base::deallocate(p, sizeof(value_type) * n);
}

PYCPP_END_NAMESPACE