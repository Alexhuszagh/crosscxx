//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief LZMA compression and decompression.
 */

#pragma once

#if defined(HAVE_LZMA)

#include <pycpp/compression/exception.h>
#include <pycpp/stl/memory.h>
#include <pycpp/string/string.h>

PYCPP_BEGIN_NAMESPACE

// FORWARD
// -------

struct lzma_compressor;
struct lzma_compressor_impl;
struct lzma_decompressor;
struct lzma_decompressor_impl;

// OBJECTS
// -------

/**
 *  \brief Wrapper for a LZMA2 compressor.
 */
struct lzma_compressor
{
public:
    lzma_compressor(int compress_level = 6);
    lzma_compressor(lzma_compressor&&) noexcept;
    lzma_compressor & operator=(lzma_compressor&&) noexcept;
    ~lzma_compressor() noexcept;

    compression_status compress(const void*& src, size_t srclen, void*& dst, size_t dstlen);
    bool flush(void*& dst, size_t dstlen);
    void close() noexcept;
    void swap(lzma_compressor&) noexcept;

private:
    unique_ptr<lzma_compressor_impl> ptr_;
};


/**
 *  \brief Wrapper for a LZMA2 decompressor.
 */
struct lzma_decompressor
{
public:
    lzma_decompressor();
    lzma_decompressor(lzma_decompressor&&) noexcept;
    lzma_decompressor & operator=(lzma_decompressor&&) noexcept;
    ~lzma_decompressor() noexcept;

    compression_status decompress(const void*& src, size_t srclen, void*& dst, size_t dstlen);
    bool flush(void*& dst, size_t dstlen);
    void close() noexcept;
    void swap(lzma_decompressor&) noexcept;

private:
    unique_ptr<lzma_decompressor_impl> ptr_;
};

// SPECIALIZATION
// --------------

template <>
struct is_relocatable<lzma_compressor>: true_type
{};

template <>
struct is_relocatable<lzma_decompressor>: true_type
{};

// FUNCTIONS
// ---------

/**
 *  \brief LZMA2-compress data.
 */
void lzma_compress(const void*& src, size_t srclen, void*& dst, size_t dstlen);

/**
 *  \brief LZMA2-compress data.
 */
string lzma_compress(const string_wrapper& str);

/**
 *  \brief LZMA2-decompress data.
 */
string lzma_decompress(const string_wrapper& str);

/**
 *  \brief LZMA2-decompress data.
 *
 *  \param bound            Known size of decompressed buffer.
 */
void lzma_decompress(const void*& src, size_t srclen, void*& dst, size_t dstlen, size_t bound);

/**
 *  \brief LZMA2-decompress data.
 *
 *  \param bound            Known size of decompressed buffer.
 */
string lzma_decompress(const string_wrapper& str, size_t bound);

PYCPP_END_NAMESPACE

#endif                  // HAVE_LZMA
