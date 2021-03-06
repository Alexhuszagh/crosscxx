//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Fixed-size deque implementation.
 *
 *  Based on the EASTL, this deque implementation preallocates a fixed-
 *  size buffer on the stack for allocation up until a certain size,
 *  and uses dynamic memory allocation afterwards.
 */

#pragma once

#include <pycpp/fixed/arena.h>
#include <pycpp/stl/deque.h>
#include <pycpp/stl/initializer_list.h>


PYCPP_BEGIN_NAMESPACE

// DECLARATION
// -----------

/**
 *  \brief Fixed-sized deque that preallocates memory on the stack.
 *
 *  By default, the deque preallocates ~4096 bytes, which can hold
 *  roughly `4096 / (sizeof(T) + sizeof(uintrptr_t))` elements.
 */
template <
    typename T,
    size_t StackSize = 4096,
    template <typename, typename> class Container = deque
>
struct fixed_deque:
    fixed_arena<T, StackSize>,
    Container<T, stack_allocator<T, StackSize>>
{
public:
    // MEMBER TYPES
    // ------------
    using self_t = fixed_deque<T, StackSize, Container>;
    using container_type = Container<T, stack_allocator<T, StackSize>>;
    using value_type = typename container_type::value_type;
    using allocator_type = typename container_type::allocator_type;
    using arena_type = typename allocator_type::arena_type;

    // MEMBER FUNCTIONS
    // ----------------
    fixed_deque();
    fixed_deque(const self_t&);
    fixed_deque(self_t&&) = delete;
    fixed_deque(size_t n, const value_type& value = value_type());
    template <typename Iter> fixed_deque(Iter first, Iter last);
    fixed_deque(initializer_list<value_type> list);
    self_t& operator=(const self_t&);
    self_t& operator=(self_t&&) = delete;
    self_t& operator=(initializer_list<value_type> list);

private:
    void reset();
};

// SPECIALIZATION
// --------------

template <
    typename T,
    size_t StackSize ,
    template <typename, typename> class Container
>
struct is_relocatable<fixed_deque<T, StackSize, Container>>: bool_constant<
        is_relocatable<typename fixed_deque<T, StackSize, Container>::arena_type>::value &&
        is_relocatable<typename fixed_deque<T, StackSize, Container>::container_type>::value
    >
{};

// IMPLEMENTATION
// --------------

template <typename T, size_t StackSize, template <typename, typename> class _>
fixed_deque<T, StackSize, _>::fixed_deque():
    container_type(allocator_type(this->arena_))
{}


template <typename T, size_t StackSize, template <typename, typename> class _>
fixed_deque<T, StackSize, _>::fixed_deque(const self_t& rhs):
    container_type(rhs, allocator_type(this->arena_))
{}


template <typename T, size_t StackSize, template <typename, typename> class _>
fixed_deque<T, StackSize, _>::fixed_deque(size_t n, const value_type& value):
    container_type(n, value, allocator_type(this->arena_))
{}


template <typename T, size_t StackSize, template <typename, typename> class _>
template <typename Iter>
fixed_deque<T, StackSize, _>::fixed_deque(Iter first, Iter last):
    container_type(first, last, allocator_type(this->arena_))
{}


template <typename T, size_t StackSize, template <typename, typename> class _>
fixed_deque<T, StackSize, _>::fixed_deque(initializer_list<value_type> list):
    container_type(list.begin(), list.end(), allocator_type(this->arena_))
{}


template <typename T, size_t StackSize, template <typename, typename> class _>
auto fixed_deque<T, StackSize, _>::operator=(const self_t& rhs) -> self_t&
{
    if (this != &rhs) {
        reset();
        container_type::assign(rhs.begin(), rhs.end());
    }
    return *this;
}


template <typename T, size_t StackSize, template <typename, typename> class _>
auto fixed_deque<T, StackSize, _>::operator=(initializer_list<value_type> list) -> self_t&
{
    reset();
    container_type::assign(list.begin(), list.end());
    return *this;
}


template <typename T, size_t StackSize, template <typename, typename> class _>
void fixed_deque<T, StackSize, _>::reset()
{
    // clear the existing container and reset the allocator for efficiency
    // do not reset the arena, since that is **undefined behavior**
    container_type::clear();
    container_type::shrink_to_fit();
}

PYCPP_END_NAMESPACE
