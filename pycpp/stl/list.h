//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief STL list alias.
 */

#pragma once

#include <pycpp/stl/initializer_list.h>
#include <pycpp/stl/iterator.h>
#include <pycpp/stl/memory.h>
#include <list>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

using std::swap;

template <
    typename T,
    typename Alloc = allocator<T>
>
using list = std::list<T, Alloc>;

PYCPP_END_NAMESPACE
