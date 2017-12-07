//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief SFINAE detection for `reserve` and fail-safe implementation.
 */

#pragma once

#include <pycpp/sfinae/has_member_function.h>
#include <cstddef>
#include <type_traits>

PYCPP_BEGIN_NAMESPACE

// SFINAE
// ------

PYCPP_HAS_MEMBER_FUNCTION(reserve, has_reserve, void (C::*)(typename C::size_type));


/**
 *  \brief Call `reserve` as a functor.
 */
struct reserve
{
    template <typename T>
    typename std::enable_if<has_reserve<T>::value, void>::type
    operator()(T &t, size_t size)
    {
        t.reserve(size);
    }

    template <typename T>
    typename std::enable_if<!has_reserve<T>::value, void>::type
    operator()(T &t, size_t size)
    {}
};

#ifdef HAVE_CPP14

// SFINAE
// ------

template <typename T>
constexpr bool has_reserve_v = has_reserve<T>::value;

#endif

PYCPP_END_NAMESPACE