//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Type detection for `pair`.
 *
 *  Detect if a type is a `std::pair`.
 *
 *  \synopsis
 *      template <typename T
 *      using is_pair<T> = implementation-defined;
 */

#pragma once

#include <pycpp/sfinae/is_specialization.h>

PYCPP_BEGIN_NAMESPACE

// SFINAE
// ------

template <typename T>
using is_pair = is_specialization<T, pair>;

#ifdef HAVE_CPP14

// SFINAE
// ------

template <typename T>
constexpr bool is_pair_v = is_pair<T>::value;

#endif

PYCPP_END_NAMESPACE
