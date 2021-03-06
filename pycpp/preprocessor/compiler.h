//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Compiler detection macros.
 *
 *  Macros to detect the compiler type, version, and the C++ ISO standard
 *  supported by the compiler. To check a given compiler version against
 *  a desired version, use the `COMPILER_VERSION` macro against
 *  `COMPILER_VERSION_CODE`.
 *
 *  \synopsis
 *      #define COMPILER_DETECTED           implementation-defined
 *      #define COMPILER_MAJOR_VERSION      implementation-defined
 *      #define COMPILER_MINOR_VERSION      implementation-defined
 *      #define COMPILER_PATCH_VERSION      implementation-defined
 *      #define COMPILER_VERSION(a, b, c)   implementation-defined
 *      #define COMPILER_VERSION_CODE       implementation-defined
 *      #define GNUC_MAJOR_VERSION          implementation-defined
 *      #define GNUC_MINOR_VERSION          implementation-defined
 *      #define GNUC_PATCH_VERSION          implementation-defined
 *      #define HAVE_CLANG                  implementation-defined
 *      #define HAVE_CLANG_2                implementation-defined
 *      #define HAVE_CLANG_3                implementation-defined
 *      #define HAVE_CLANG_4                implementation-defined
 *      #define HAVE_CLANG_5                implementation-defined
 *      #define HAVE_CPP11                  implementation-defined
 *      #define HAVE_CPP14                  implementation-defined
 *      #define HAVE_CPP17                  implementation-defined
 *      #define HAVE_CPP20                  implementation-defined
 *      #define HAVE_GCC                    implementation-defined
 *      #define HAVE_GCC_3                  implementation-defined
 *      #define HAVE_GCC_4                  implementation-defined
 *      #define HAVE_GCC_5                  implementation-defined
 *      #define HAVE_GCC_6                  implementation-defined
 *      #define HAVE_GCC_7                  implementation-defined
 *      #define HAVE_GCC_8                  implementation-defined
 *      #define HAVE_GNUC                   implementation-defined
 *      #define HAVE_INTEL                  implementation-defined
 *      #define HAVE_MSVC                   implementation-defined
 *      #define HAVE_MSVC_10                implementation-defined
 *      #define HAVE_MSVC_11                implementation-defined
 *      #define HAVE_MSVC_12                implementation-defined
 *      #define HAVE_MSVC_14                implementation-defined
 *      #define HAVE_MSVC_5                 implementation-defined
 *      #define HAVE_MSVC_6                 implementation-defined
 *      #define HAVE_MSVC_7                 implementation-defined
 *      #define HAVE_MSVC_7                 implementation-defined
 *      #define HAVE_MSVC_8                 implementation-defined
 *      #define HAVE_MSVC_9                 implementation-defined
 */

#pragma once

// INTEL
// -----

#if defined(__INTEL_COMPILER)
// Intel compilers can use different compatibility modes,
// such as GCC, Clang, or Visual Studio compatibility,
// so define the presence of the Intel compiler, but don't
// treat it as a compiler.
#   define HAVE_INTEL 1
#endif

// MSVC
// ----

#if !defined(COMPILER_DETECTED) && defined(_MSC_VER)
#   define HAVE_MSVC 1
#   define COMPILER_DETECTED HAVE_MSVC
#   if _MSC_VER >= 1911
#      define HAVE_MSVC_14 1
#      define COMPILER_MAJOR_VERSION 15
#      define COMPILER_MINOR_VERSION 1
#      define COMPILER_PATCH_VERSION 3
#   elif _MSC_VER == 1910
#      define HAVE_MSVC_14 1
#      define COMPILER_MAJOR_VERSION 14
#      define COMPILER_MINOR_VERSION 1
#      define COMPILER_PATCH_VERSION 0
#   elif _MSC_VER == 1900
#      define HAVE_MSVC_14 1
#      define COMPILER_MAJOR_VERSION 14
#      define COMPILER_MINOR_VERSION 0
#      define COMPILER_PATCH_VERSION 0
#   elif _MSC_VER == 1800
#      define HAVE_MSVC_12 1
#      define COMPILER_MAJOR_VERSION 12
#      define COMPILER_MINOR_VERSION 0
#      define COMPILER_PATCH_VERSION 0
#   elif _MSC_VER == 1700
#      define HAVE_MSVC_11 1
#      define COMPILER_MAJOR_VERSION 11
#      define COMPILER_MINOR_VERSION 0
#      define COMPILER_PATCH_VERSION 0
#   elif _MSC_VER == 1600
#      define HAVE_MSVC_10 1
#      define COMPILER_MAJOR_VERSION 10
#      define COMPILER_MINOR_VERSION 0
#      define COMPILER_PATCH_VERSION 0
#   elif _MSC_VER == 1500
#      define HAVE_MSVC_9 1
#      define COMPILER_MAJOR_VERSION 9
#      define COMPILER_MINOR_VERSION 0
#      define COMPILER_PATCH_VERSION 0
#   elif _MSC_VER == 1400
#      define HAVE_MSVC_8 1
#      define COMPILER_MAJOR_VERSION 8
#      define COMPILER_MINOR_VERSION 0
#      define COMPILER_PATCH_VERSION 0
#   elif _MSC_VER == 1310
#      define HAVE_MSVC_7 1
#      define COMPILER_MAJOR_VERSION 7
#      define COMPILER_MINOR_VERSION 1
#      define COMPILER_PATCH_VERSION 0
#   elif _MSC_VER == 1300
#      define HAVE_MSVC_7 1
#      define COMPILER_MAJOR_VERSION 7
#      define COMPILER_MINOR_VERSION 0
#      define COMPILER_PATCH_VERSION 0
#   elif _MSC_VER == 1200
#      define HAVE_MSVC_6 1
#      define COMPILER_MAJOR_VERSION 6
#      define COMPILER_MINOR_VERSION 0
#      define COMPILER_PATCH_VERSION 0
#   elif _MSC_VER == 1100
#      define HAVE_MSVC_5 1
#      define COMPILER_MAJOR_VERSION 5
#      define COMPILER_MINOR_VERSION 0
#      define COMPILER_PATCH_VERSION 0
#   endif
#endif

// GNUC
// ----

#if defined(__GNUC__)
#   define HAVE_GNUC
#   define GNUC_MAJOR_VERSION __GNUC__
#   define GNUC_MINOR_VERSION __GNUC_MINOR__
#   define GNUC_PATCH_VERSION __GNUC_PATCHLEVEL__
#endif

// CLANG
// -----

#if !defined(COMPILER_DETECTED) && defined(__clang__)
#   define HAVE_CLANG 1
#   define COMPILER_DETECTED HAVE_CLANG
#   define COMPILER_MAJOR_VERSION __clang_major__
#   define COMPILER_MINOR_VERSION __clang_minor__
#   define COMPILER_PATCH_VERSION __clang_patchlevel__
#   if COMPILER_MAJOR_VERSION == 5
#      define HAVE_CLANG_5 1
#   elif COMPILER_MAJOR_VERSION == 4
#      define HAVE_CLANG_4 1
#   elif COMPILER_MAJOR_VERSION == 3
#      define HAVE_CLANG_3 1
#   elif COMPILER_MAJOR_VERSION == 2
#      define HAVE_CLANG_2 1
#   endif
#endif

// GCC
// ---

#if !defined(COMPILER_DETECTED) && defined(HAVE_GNUC)
#   define HAVE_GCC 1
#   define COMPILER_DETECTED HAVE_GCC
#   define COMPILER_MAJOR_VERSION GNUC_MAJOR_VERSION
#   define COMPILER_MINOR_VERSION GNUC_MINOR_VERSION
#   define COMPILER_PATCH_VERSION GNUC_PATCH_VERSION
#   if COMPILER_MAJOR_VERSION == 8
#      define HAVE_GCC_8 1
#   elif COMPILER_MAJOR_VERSION == 7
#      define HAVE_GCC_7 1
#   elif COMPILER_MAJOR_VERSION == 6
#      define HAVE_GCC_6 1
#   elif COMPILER_MAJOR_VERSION == 5
#      define HAVE_GCC_5 1
#   elif COMPILER_MAJOR_VERSION == 4
#      define HAVE_GCC_4 1
#   elif COMPILER_MAJOR_VERSION == 3
#      define HAVE_GCC_3 1
#   endif
#endif

// C++
// ---

#if defined(__cplusplus)
// TODO: C++20 has not yet been standardized, refine when ratified.
#   if __cplusplus >= 202099L
#       define HAVE_CPP20 1
#   endif
#   if __cplusplus >= 201703L
#       define HAVE_CPP17 1
#   endif
#   if __cplusplus >= 201402L
#       define HAVE_CPP14 1
#   endif
#   if __cplusplus >= 201103L
#       define HAVE_CPP11 1
#   endif
#endif

// VERSION
// -------

#define COMPILER_VERSION(major, minor, patch)     \
    ((major << 16) + (minor << 8) + patch)

#define COMPILER_VERSION_CODE                     \
    COMPILER_VERSION(COMPILER_MAJOR_VERSION,      \
                     COMPILER_MINOR_VERSION,      \
                     COMPILER_PATCH_VERSION)
