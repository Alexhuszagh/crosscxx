//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Polymorphic allocator as described in N3525.
 *
 *  A polymorphic allocator that wraps a virtual, byte-based resource
 *  that abstracts general bytes. The polymorphic allocator is a single
 *  type that may have different allocation properties depending
 *  on the underlying resource.
 *
 *  http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2013/n3525.pdf
 *
 *  \synopsis
 *      memory_resource* new_delete_resource() noexcept;
 *      memory_resource* null_memory_resource() noexcept;
 *      memory_resource* get_default_resource() noexcept;
 *      memory_resource* set_default_resource(memory_resource* r) noexcept;
 *
 *      struct memory_resource
 *      {
 *      public:
 *          virtual ~memory_resource();
 *
 *          void* allocate(size_t n, size_t alignment = implementation-defined);
 *          void deallocate(void* p, size_t n, size_t alignment = implementation-defined);
 *          bool is_equal(const memory_resource&) const noexcept;
 *
 *      protected:
 *          virtual void* do_allocate(size_t n, size_t alignment) = 0;
 *          virtual void do_deallocate(void* p, size_t n, size_t alignment) = 0;
 *          virtual bool do_is_equal(const memory_resource&) const noexcept = 0;
 *
 *      private:
 *          static std::atomic<memory_resource*> default_resource_;
 *
 *          friend memory_resource* set_default_resource(memory_resource*) noexcept;
 *          friend memory_resource* get_default_resource() noexcept;
 *      };
 *
 *      template <typename Allocator>
 *      struct resource_adaptor_imp;
 *
 *      template <typename Allocator>
 *      using resource_adaptor = resource_adaptor_imp<
 *          typename std::allocator_traits<Allocator>::template rebind_alloc<byte>
 *      >;
 *
 *      template <typename T>
 *      struct polymorphic_allocator
 *      {
 *          using value_type = T;
 *          polymorphic_allocator() noexcept;
 *          polymorphic_allocator(memory_resource *r);
 *          polymorphic_allocator(const polymorphic_allocator<T>& rhs);
 *          template <typename U> polymorphic_allocator(const polymorphic_allocator<U>& rhs) noexcept;
 *          polymorphic_allocator<T>& operator=(const polymorphic_allocator<T>& rhs) = delete;
 *
 *          T* allocate(size_t n);
 *          void deallocate(T* p, size_t n);
 *          polymorphic_allocator select_on_container_copy_construction() const;
 *          memory_resource* resource() const;
 *      };
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/preprocessor/compiler_traits.h>
#if defined(HAVE_CPP17)
#   include <memory_resource>
#else
#   include <atomic>
#   include <limits>
#   include <memory>
#   include <new>
#   include <stdexcept>
#   include <type_traits>
#   include <cstddef>
#endif

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

#if defined(HAVE_CPP17)     // HAVE_CPP17

using std::byte;

#else                       // !HAVE_CPP17

enum class byte: unsigned char
{};

#endif                      // HAVE_CPP17

// MACROS
// ------

// Right now we depend on some non-standard extensions to
// polymorphic allocator, specifically,
// `propagate_on_container_move_assignment`.
// The conditions to reproduce this are, using
// GCC on Linux:
//      using pair = std::pair<const int, int>;
//      using list = std::list<pair, polymorphic_allocator<pair>>;
//      list x_, y_;
//      y_ = std::move(x_);
//
// This calls `list::operator=(list&&); which then
// checks if `propagate_on_container_move_assignment`
// or `is_always_equal` for the container allocator,
// if not, it uses move assignment to move nodes.
//
// Since the allocator is not `is_always_equal`, since
// it wraps an internal resource, simply manage the
// allocator move assignment so it is **well formed**.
//
// Forcing a custom implementation of polymorphic allocator
// is controlled by `PYCPP_DEFINE_POLYMORPHIC_ALLOCATOR`, and
// allowing nonstandard behavior in polymorphic allocator is
// controlled by `PYCPP_NON_STANDARD_POLYMORPHIC_ALLOCATOR`.
// Revert this whenever possible.
#define PYCPP_DEFINE_POLYMORPHIC_ALLOCATOR 1        // ! defined(HAVE_CPP17)
#define PYCPP_NON_STANDARD_POLYMORPHIC_ALLOCATOR 1  // 0

#if !defined(PYCPP_DEFINE_POLYMORPHIC_ALLOCATOR)    // HAVE_CPP17

// ALIAS
// -----

using memory_resource = std::pmr::memory_resource;
template <typename T> using polymorphic_allocator = std::pmr::polymorphic_allocator;
template <typename T> using resource_adaptor_imp = std::pmr::resource_adaptor_imp<T>;
template <typename T> using resource_adaptor = std::pmr::resource_adaptor<T>;
using pool_options = std::pmr::pool_options;
using synchronized_pool_resource = std::pmr::synchronized_pool_resource;
using unsynchronized_pool_resource = std::pmr::unsynchronized_pool_resource;
using monotonic_buffer_resource = std::pmr::monotonic_buffer_resource;

// FUNCTIONS
// ---------

/**
 *  \brief Return a pointer to a global resource that uses `new` and `delete`.
 */
memory_resource* new_delete_resource() noexcept;

/**
 *  \brief Return a pointer to a global resource that does no allocation.
 */
memory_resource* null_memory_resource() noexcept;

/**
 *  \brief Get the current default resource.
 */
memory_resource* get_default_resource() noexcept;

/**
 *  \brief Set the default resource.
 */
memory_resource* set_default_resource(memory_resource* r) noexcept;

#else                               // !HAVE_CPP17

namespace polymorphic_detail
{
// DETAIL
// ------

/**
 *  \brief Round up to nearest multiple of `alignment`.
 */
size_t aligned_allocation_size(size_t n, size_t alignment) noexcept;

/**
 *  \brief Get memory max size.
 */
size_t max_size(size_t align);

}   /* polymorphic_detail */

// FORWARD
// -------

struct memory_resource;
template <typename T> struct polymorphic_allocator;
template <typename Allocator> struct resource_adaptor_imp;
template <typename Allocator> struct is_relocatable;

// ALIAS
// -----

template <typename Allocator>
using resource_adaptor = resource_adaptor_imp<
    typename std::allocator_traits<Allocator>::template rebind_alloc<byte>
>;

// FUNCTIONS
// ---------

/**
 *  \brief Return a pointer to a global resource that uses `new` and `delete`.
 */
memory_resource* new_delete_resource() noexcept;

/**
 *  \brief Return a pointer to a global resource that does no allocation.
 */
memory_resource* null_memory_resource() noexcept;

/**
 *  \brief Get the current default resource.
 */
memory_resource* get_default_resource() noexcept;

/**
 *  \brief Set the default resource.
 */
memory_resource* set_default_resource(memory_resource* r) noexcept;

// OBJECTS
// -------

// MEMORY RESOURCE

/**
 *  \brief Abstract base class for polymorphic resources.
 */
struct memory_resource
{
public:
    virtual ~memory_resource();

    // ALLOCATOR TRAITS
    void* allocate(size_t n, size_t alignment = alignof(std::max_align_t));
    void deallocate(void* p, size_t n, size_t alignment = alignof(std::max_align_t));
    bool is_equal(const memory_resource&) const noexcept;

protected:
    // MEMORY TRAITS
    virtual void* do_allocate(size_t n, size_t alignment) = 0;
    virtual void do_deallocate(void* p, size_t n, size_t alignment) = 0;
    virtual bool do_is_equal(const memory_resource&) const noexcept = 0;

private:
    static std::atomic<memory_resource*> default_resource_;

    friend memory_resource* set_default_resource(memory_resource*) noexcept;
    friend memory_resource* get_default_resource() noexcept;
};

// RESOURCE ADAPTOR

/**
 *  \brief Adapts an STL allocator to a polymorphic resource.
 */
template <typename Allocator>
struct resource_adaptor_imp: memory_resource
{
    // MEMBER TYPES
    // ------------
    using allocator_type = Allocator;

    // MEMBER FUNCTIONS
    // ----------------

    // CONSTRUCTORS

    resource_adaptor_imp() = default;
    resource_adaptor_imp(const resource_adaptor_imp&) = default;
    resource_adaptor_imp(resource_adaptor_imp&&) = default;
    resource_adaptor_imp& operator=(const resource_adaptor_imp&) = default;

    explicit resource_adaptor_imp(const allocator_type& alloc):
        alloc_(alloc)
    {}

    explicit resource_adaptor_imp(allocator_type&& alloc):
        alloc_(std::move(alloc))
    {}

    // ALLOCATOR TRAITS

    allocator_type get_allocator() const
    {
        return alloc_;
    }

protected:
    // MEMORY TRAITS

    virtual void* do_allocate(size_t n, size_t alignment)
    {
        if (n > polymorphic_detail::max_size(max_align)) {
            throw std::bad_alloc();
        }

        size_t s = polymorphic_detail::aligned_allocation_size(n, max_align) / max_align;
        return alloc_.allocate(s);
    }

    virtual void do_deallocate(void* p, size_t n, size_t alignment)
    {
        using value_type = typename std::allocator_traits<alloc>::value_type;
        size_t s = polymorphic_detail::aligned_allocation_size(n, max_align) / max_align;
        alloc_.deallocate(reinterpret_cast<value_type*>(p), s);
    }

    virtual bool do_is_equal(const memory_resource& rhs) const noexcept
    {
        using self = resource_adaptor_imp<Allocator>;
        const self* p = dynamic_cast<const self*>(&rhs);
        return p ? alloc_ == p->alloc_ : false;
    }

private:
    static constexpr size_t max_align = alignof(std::max_align_t);
    using traits_type = std::allocator_traits<Allocator>;
    using storage = typename std::aligned_storage<max_align, max_align>::type;
    using alloc = typename traits_type::template rebind_alloc<storage>;
    static_assert(std::is_same<typename traits_type::value_type, byte>::value, "");

    alloc alloc_;
};

// POLYMORPHIC ALLOCATOR

/**
 *  \brief STL allocator wrapping the polymorphic resource.
 */
template <typename T>
struct polymorphic_allocator
{
public:
    // MEMBER TYPES
    // ------------
    using value_type = T;
#if defined(CPP11_PARTIAL_ALLOCATOR_TRAITS)
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    template <typename U> struct rebind { using other = polymorphic_allocator<U>; };
#endif      // CPP11_PARTIAL_ALLOCATOR_TRAITS

    // MEMBER FUNCTIONS
    // ----------------
    polymorphic_allocator(const polymorphic_allocator<T>&) = default;
    polymorphic_allocator<T>& operator=(const polymorphic_allocator<T>&) = delete;

    polymorphic_allocator() noexcept:
        resource_(get_default_resource())
    {}

    polymorphic_allocator(memory_resource *r):
        resource_(r ? r : get_default_resource())
    {}

    template <typename U>
    polymorphic_allocator(const polymorphic_allocator<U>& rhs) noexcept:
        resource_(rhs.resource())
    {}

    // ALLOCATOR TRAITS

    T* allocate(size_t n)
    {
        return static_cast<T*>(resource_->allocate(n * sizeof(T), alignof(T)));
    }

    void deallocate(T* p, size_t n)
    {
        resource_->deallocate(p, n * sizeof(T), alignof(T));
    }

#if defined(CPP11_PARTIAL_ALLOCATOR_TRAITS)

    template <typename ... Ts>
    void construct(T* p, Ts&&... ts)
    {
        ::new (static_cast<void*>(p)) T(std::forward<Ts>(ts)...);
    }

    void destroy(T* p)
    {
        p->~T();
    }

    size_type max_size()
    {
        return std::numeric_limits<size_type>::max();
    }

#endif      // CPP11_PARTIAL_ALLOCATOR_TRAITS

    // PROPERTIES

    polymorphic_allocator select_on_container_copy_construction() const
    {
        return polymorphic_allocator<T>();
    }

    memory_resource* resource() const
    {
        return resource_;
    }

#if PYCPP_NON_STANDARD_POLYMORPHIC_ALLOCATOR

    using propagate_on_container_move_assignment = std::true_type;

    polymorphic_allocator(polymorphic_allocator<T>&& rhs) noexcept:
        polymorphic_allocator()
    {
        // let resource use the default constructor
        std::swap(resource_, rhs.resource_);
    }

    template <typename U>
    polymorphic_allocator(polymorphic_allocator<U>&& rhs) noexcept:
        polymorphic_allocator()
    {
        // let resource use the default constructor
        std::swap(resource_, rhs.resource_);
    }

    polymorphic_allocator<T>& operator=(polymorphic_allocator<T>&& rhs) noexcept
    {
        std::swap(resource_, rhs.resource_);
        return *this;
    }

    template <typename U>
    friend struct polymorphic_allocator;

#endif      // PYCPP_NON_STANDARD_POLYMORPHIC_ALLOCATOR

private:
    memory_resource* resource_;
};

// TODO: implement pool_options
// TODO: implement synchronized_pool_resource
// TODO: implement unsynchronized_pool_resource
// TODO: implement monotonic_buffer_resource

// SPECIALIZATION
// --------------

template <>
struct is_relocatable<memory_resource>: std::true_type
{};

template <typename Allocator>
struct is_relocatable<resource_adaptor_imp<Allocator>>: is_relocatable<Allocator>
{};

template <typename T>
struct is_relocatable<polymorphic_allocator<T>>: std::true_type
{};

// TODO: specialize pool_options
// TODO: specialize synchronized_pool_resource
// TODO: specialize unsynchronized_pool_resource
// TODO: specialize monotonic_buffer_resource

// IMPLEMENTATION
// --------------

// MEMORY RESOURCE

bool operator==(const memory_resource& lhs, const memory_resource& rhs);
bool operator!=(const memory_resource& lhs, const memory_resource& rhs);

// POLYMORPHIC ALLOCATOR

template <typename T1, typename T2>
bool operator==(const polymorphic_allocator<T1>& lhs, const polymorphic_allocator<T2>& rhs)
{
    return *lhs.resource() == *rhs.resource();
}

template <typename T1, typename T2>
bool operator!=(const polymorphic_allocator<T1>& lhs, const polymorphic_allocator<T2>& rhs)
{
    return !(lhs == rhs);
}

#endif                              // HAVE_CPP17

PYCPP_END_NAMESPACE
