//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Hash specializations for <memory>.
 */

#pragma once

#include <pycpp/stl/functional.h>
#include <pycpp/stl/new.h>
#include <pycpp/stl/type_traits.h>
#include <pycpp/stl/detail/polymorphic_allocator.h>
#include <memory>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

using std::make_shared;
using std::allocate_shared;
using std::addressof;
using std::allocator_traits;
using std::default_delete;
using std::unique_ptr;
using std::shared_ptr;
using std::weak_ptr;

template <typename T, typename Allocator>
struct uses_allocator: std::uses_allocator<T, Allocator>
{};

#if USE_POLYMORPHIC_ALLOCATOR           // POLYMOPRHIC

template <typename T>
using allocator = polymorphic_allocator<T>;

#else                                   // !POLYMOPRHIC

template <typename T>
using allocator = std::allocator<T>;

#endif                                  // POLYMOPRHIC

// FORWARD
// -------

template <typename Key>
struct hash;

// SPECIALIZATION
// --------------

#if USE_XXHASH

template <typename T, typename Deleter>
struct hash<std::unique_ptr<T, Deleter>>
{
    using argument_type = std::unique_ptr<T, Deleter>;
    inline size_t operator()(const argument_type& x) const noexcept
    {
        using pointer = typename argument_type::pointer;
        return hash<pointer>()(x.get());
    }
};

template <typename T>
struct hash<std::shared_ptr<T>>
{
    using argument_type = std::shared_ptr<T>;
    inline size_t operator()(const argument_type& x) const noexcept
    {
        using pointer = typename argument_type::pointer;
        return hash<pointer>()(x.get());
    }
};

#endif          // USE_XXHASH

// FUNCTIONS
// ---------

/**
 *  \brief `new` analog for a custom allocator.
 */
template <typename T, typename Allocator, typename ... Ts>
inline T* allocate_and_construct(Allocator& allocator, Ts&&... ts)
{
    using allocator_type = typename allocator_traits<Allocator>::template rebind_alloc<T>;
    using traits_type = allocator_traits<allocator_type>;

    // the allocator should throw if there's no memory available
    allocator_type alloc(allocator);
    T* t = traits_type::allocate(alloc, sizeof(T));
    traits_type::construct(alloc, t, std::forward<Ts>(ts)...);
    return t;
}


/**
 *  \brief `delete` analog for a custom allocator.
 */
template <typename T, typename Allocator>
inline void destroy_and_deallocate(Allocator& allocator, T* t, size_t n = 1)
{
    using allocator_type = typename allocator_traits<Allocator>::template rebind_alloc<T>;
    using traits_type = allocator_traits<allocator_type>;

    allocator_type alloc(allocator);
    traits_type::destroy(alloc, t);
    traits_type::deallocate(alloc, t, n);
}

#if defined(HAVE_CPP14)     // CPP14

using std::make_unique;

#else                       // CPP11

template <typename T, typename ... Ts >
unique_ptr<T> make_unique(Ts&&... ts)
{
    return unique_ptr<T>(new T(forward<Ts>(ts)...));
}

template <typename T>
unique_ptr<T> make_unique(size_t size)
{
    using type = remove_extent_t<T>;
    return unique_ptr<T>(new type[size]);
}

#endif                      // HAVE_CPP14

PYCPP_END_NAMESPACE
