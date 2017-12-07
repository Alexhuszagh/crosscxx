PyCPP
=====

Python-like C++ environment with independent, lightweight utilities for cross-platform C++ development.

[![Build Status](https://travis-ci.org/Alexhuszagh/PyCpp.svg?branch=master)](https://travis-ci.org/Alexhuszagh/PyCpp)
[![Build status](https://ci.appveyor.com/api/projects/status/b8fh8431rcu3wc3q?svg=true)](https://ci.appveyor.com/project/Alexhuszagh/pycpp)
[![GitHub license](https://img.shields.io/badge/license-MIT-blue.svg)](https://github.com/Alexhuszagh/pycpp/blob/master/LICENSE.md)

> **WARNING** PyCPP is still alpha software. APIs are destined to break, and bugs will certainly exist.

**Table of Contents**

- [Introduction](#introduction)
- [Design](#design)
- [Separation of Logic](#separation-of-logic)
- [Namespace](#namespace)
- [Building](#building)
- [Performance](#performance)
- [Platforms](#platforms)
- [Contributors](#contributors)
- [License](#license)

## Introduction

Modern C++ development depends on a mix of archaic C-libraries with poorly documented interfaces, and template-driven, header-only libraries that produce significant binary and compile time bloat. PyCPP aims to be a collection loosely interconnected features missing from the C++ standard library, boasting lean headers, fast compile times, and permissive licenses, to facilitate app development with minimal overhead.

PyCPP is, in many ways, a spiritual port of Python back to C++. There are idiomatic ways to do certain tasks, and there is a right and a wrong way. "Zero-cost abstractions" don't exist: infinite abstraction leads to unmaintainable complexity. PyCPP also ports Python's [hashlib](https://docs.python.org/3/library/hashlib.html), [os.path](https://docs.python.org/2/library/os.path.html#module-os.path), and [string](https://docs.python.org/2/library/string.html) modules, providing familiar APIs to the Python standard library.

## Design

PyCPP is broken down into multiple core parts:

1. A macro-driven abstraction library to detect compiler features.
2. Lexical conversion routines.
3. Code page conversion routines.
4. Filesystem utilities.
5. High-level features common in other standard libraries.

Combined, the library has less than 100,00 total lines of C++ headers, with binary sizes of < 5MB, and each module has only a few dependencies, making it easy to extract individual modules for use in another project. It also makes frequent use of the Pimpl idiom, abstracting away low-level routines for a simple public API.

**Abstraction Library**

The files [os.h](/pycpp/os.h), [compiler.h](/pycpp/compiler.h), [architecture.h](/pycpp/architecture.h), [processor.h](/pycpp/processor.h), and [byteorder.h](/pycpp/byteorder.h) provide an abstraction platform to detect the current compiler, compiler version, operating system, system endianness, and processor. These are the largest dependencies, with ~1000 lines of code.

**Lexical Conversion**

PyCPP includes high-performance [lexical](/pycpp/lexical.h) conversions to and from string for built-in primitive types, including:

- std::nullptr_t
- bool
- char
- int (including 8-64 bit fixed-width integers)
- float (both single- and double-precision)
- enums

Specializations for custom types can be added by extending the `lexical` template function, or the `format` and `extract` template functions.

**Code Page Conversion**

Any modern application should be Unicode aware, and PyCPP includes routines for [code point conversions](/pycpp/unicode.h) and [codec conversions](codec.h), as well as [Unicode-aware case conversions](/pycpp/casemap.h). They provide two APIs: a modern wrapper for STL string conversions, and a C-API taking a src and dst buffer.

Supported features include:

- UTF-8, UTF-16, and UTF-32 conversions
- std::string, std::u16string, and std::u32string conversions.
- Lowercase, uppercase, capitalized, and titlecase Unicode conversions.

**Fileystem**

PyCPP includes a high-level filesystem library, a spiritual port of the `os.path` module from Python.

Supported features include:

- Portable stat and lstat
- Path normalization
- Environment variable expansion

**High-Level Features**

**_General_**

- [Thread-local storage](/pycpp/preprocessor/tls.h).
- [Optional](/pycpp/collections/optional.h) type.
- [Generic "any"](/pycpp/collections/any.h) type.
- [Bit-flag aware scoped enums](/pycpp/misc/enum.h).
- [string_view](/str/view/string.h) and [vector_view](/pycpp/view/vector.h)

**_Codec_**

- [Base16](/pycpp/string/base16.h), [base32](/pycpp/string/base32.h), and [Base64](/pycpp/string/base64.h) encoding and decoding.
- [Hex](/pycpp/string/hex.h) encoding and decoding.
- [Punycode](/pycpp/string/punycode.h) encoding and decoding.
- [Url](/pycpp/string/url.h) encoding and decoding.
- [International domain name](/pycpp/lattice/url.h) encoding and decoding.

**_Streams_**

- [Wide API streams](/pycpp/stream/fstream.h) for Windows.
- [Filtering streambufs](/pycpp/stream/filter.h).
- [Sequential](/pycpp/stream/sequential.h), [random-access](/pycpp/stream/random_access.h) and [memory-mapped](/pycpp/stream/mmap.h) file streams.

**_Random_**

- [Pseudorandom](/pycpp/random.h) number generators.
- [Cryptographic random](/pycpp/random.h) number generators.

**_Cryptography_**

- [Secure memory](/pycpp/secure/allocator.h) allocation and deallocation.
- [Secure buffers](/pycpp/secure/string.h).
- [Cryptographic hash](/pycpp/hashlib.h) functions.
- [Cryptographic ciphers](/pycpp/cipher.h).

**_Iterables_**

- [Iterator adaptors](/pycpp/iterator.h).
- [Combinations, permutation](/pycpp/itertools/sampling.h) and [cartesian products](/pycpp/itertools/product.h).

**_JSON_**

- [DOM](/pycpp/json/dom.h) parser.
- [SAX](/pycpp/json/sax.h) parser.
- [DOM/Text](/pycpp/json/writer.h) writer.

**_XML_**

- [DOM](/pycpp/xml/dom.h) parser.
- [SAX](/pycpp/xml/sax.h) parser.
- [DOM/Text](/pycpp/xml/writer.h) writer.

**_Compression_**

- [Compression detection](/pycpp/compression/detect.h) for strings, streams, and files.
- [bzip](/pycpp/compression/bzip2.h) compressors and decompressors.
- [zlib](/pycpp/compression/zlib.h) compressors and decompressors.
- [lzma2](/pycpp/compression/lzma.h) compressors and decompressors.
- [blosc](/pycpp/compression/blosc.h) compressors and decompressors.
- [Decompressing streams](/pycpp/compression/stream.h).

**WARNING:** PyCPP includes cryptographic hashes and ciphers, which are tested via fuzzing. The buffer containing these objects is reset upon object destruction, to minimize side-channel attacks. However, they should be assumed to be insecure, until audited.

**_SFINAE_**

- Detect [operator compatibility](/pycpp/sfinae.h) between types.
- Detect [STL](/pycpp/sfinae.h) types.
- Detect member [variables](/pycpp/sfinae/has_member_variable.h), [functions](/pycpp/sfinae/has_member_function.h), and [types](/pycpp/sfinae/has_member_type.h).

**_Cache_**

- [Least-recently used](/pycpp/cache/lru.h) and [least-recently inserted](/pycpp/cache/lri.h) caches.

// TODO document

**_Collections_**

- [Counter](/pycpp/collections/counter.h) to count hashable objects.
- [Threshold counter](/pycpp/collections/threshold_counter.h) for long-term counting of hashable objects above a frequency threshold.
- Binary Search Tree [map](/pycpp/collections/btree_map.h) and [set](/pycpp/collections/btree_set.h) implemented using B-trees for enhanced performance.
- Hash [map](/pycpp/collections/robin_map.h) and [set](/pycpp/collections/robin_set.h) implemented using robin-hood addressing for enhanced performance.

// TODO document

**_HTTP Requests_**

PyCPP includes a lightweight, [HTTP request library](/pycpp/lattice.h) supporting:

- Custom Headers
- Parameters
- Cookies
- DNS caching
- Redirections
- Content-Type detection
- Pooled requests
- International domain names
- Unicode Support (UTF8, UTF16, UTF32)
- Auth (Basic, Digest)
- Proxies (Beta)
- SSL/TLS Adapters
- Domain validation
- Certificate validation

**_Multi-Index_**

PyCPP includes a container with multiple views of the stored data type, based off [Boost.MultiIndex](http://www.boost.org/doc/libs/1_62_0/libs/multi_index/doc/index.html), supporting:

- Multiple views based off [hashable](/pycpp/multi_index/hashed_index.h), [ordered](/pycpp/multi_index/ordered_index.h), [ranked](/pycpp/multi_index/ranked_index.h), [sequenced](/pycpp/multi_index/sequenced_index.h), and [random-access](/pycpp/multi_index/random_access_index.h) indexes.
- [Member](/pycpp/multi_index/member.h), [function](/pycpp/multi_index/mem_fun.h), or [composite keys](/pycpp/multi_index/composite_key.h) per view.
- [Tagged](/pycpp/multi_index/tag.h) views.

**_Allocators_**

Custom STL-compatible allocators for both specialized and general-purpose allocators.

- [Memory pools](/pycpp/allocator/pool.h).
- [Stack allocators](/pycpp/allocator/stack.h) that preallocate a fixed amount of memory on the stack and only dynamically request memory after the stack has been exhausted.
- [Secure allocator](/pycpp/allocator/secure.h) that allows memory locking, and securely zeroes memory prior to deallocation.

// TODO: document

**_Fixed Containers_**

PyCPP includes STL-like containers that use a stack allocator to preallocate memory for the container, and therefore defines a static size with fast allocation. If the fixed buffer size is fully exhausted, these containers use dynamic memory allocation as a fallback.

- [Fixed vector](/pycpp/fixed/vector.h)

// TODO: document

## Separation of Logic

One of the most notable design flaws in the C++ standard library is combination of discrete logic into a single interface, most notably with IOStreams. Specializing IOStreams for a new features requires overriding low-level, poorly documented methods, and attempts to remedy this (most notably Boost.IOStreams) still require copious amounts of boilerplate.

To provide a clear separation of logic and therefore a more friendly API, PyCPP provides stream overloads taking a generic callback that transforms the input sequence to the output. For example, to convert all characters within a stream to lowercase, we can use `ascii_tolower` and our `filter_callback`.

```cpp
#include <pycpp/casemap.h>          // ascii_tolower
#include <pycpp/stream/filter.h>    // filter_istream
#include <sstream>

void transform_tolower(const void*& src, size_t srclen,
    void*& dst, size_t dstlen,
    size_t char_size)
{
    // cast to our expected types
    const char* src_ = reinterpret_cast<const char*>(src);
    char* dst_ = reinterpret_cast<char*>(dst);

    // copy bytes
    while (srclen-- && dstlen--) {
        *dst_++ = ascii_tolower(*src_++);
    }

    // reassign to buffer
    src = (const void*) src_;
    dst = (void*) dst_;
}


int main(void)
{
    std::istringstream sstream("THIS IS A MESSAGE");
    filter_istream stream(sstream, transform_tolower);
    std::string line;
    std::getline(stream, line);
    std::cout << line << std::endl;     // "this is a message";
}
```

Similar choices to separate discrete logic are found throughout PyCPP, simplifying numerous interfaces in the standard library.

## Namespace

By default, PyCPP uses no namespace. However, a custom namespace can be set during configuration. For example, to use `pycpp` as the namespace name, add `-DPYCPP_NAMESPACE:STRING=pycpp` during the CMake configuration step.

To access members in the PyCPP namespace, you can either preface method calls with `PYCPP_NAMESPACE::method()`, where `PYCPP_NAMESPACE` evaluates to the desired namespace, or `PYCPP_USING_NAMESPACE`, which introduces methods from PyCPP to the current namespace.

## Building

Simply clone, configure with CMake, and build.

```bash
git clone https://github.com/Alexhuszagh/pycpp.git
git submodule update --init --recursive
cmake .                         # `-DBUILD_TESTS=ON`
make -j 5                       # "msbuild pycpp.sln" for MSVC
```

> **WARNING** Linking with libc++ (LLVM) causes issues with the LZMA compressors and decompressors. Any patches would be wonderful, in the meantime, please use GCC or link with libstdc++.

## Performance

To avoid long build times, PyCPP avoids inlining functions at all costs, and minimizes template definitions within headers, coming with a slight performance penalty. Using link-time optimization should remedy these issues, and mitigate the need for explicit inlining.

## Platforms

PyCPP is continually built with the following compiler and compiler versions:

- Clang 3.8+
- GCC 5.3+
- MinGW 5.3.0 (MXE, MinGW, and MSYS2)
- Visual Studio 14 2015
- Visual Studio 15 2017

## Contributors

- Alex Huszagh

## License

MIT, see [license](LICENSE.md).
