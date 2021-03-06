//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#if defined(HAVE_ZLIB)

#include <pycpp/compression/core.h>
#include <pycpp/compression/zlib.h>
#include <zlib.h>

PYCPP_BEGIN_NAMESPACE

// HELPERS
// -------

static size_t zlib_compress_bound(size_t size)
{
    return compressBound(static_cast<uLong>(size));
}


void check_zstatus(int error)
{
    switch (error) {
        case Z_OK:
        case Z_STREAM_END:
        case Z_NEED_DICT:
        case Z_BUF_ERROR:
            return;
        case Z_DATA_ERROR:
            throw compression_error(compression_data_error);
        case Z_MEM_ERROR:
            throw compression_error(compression_out_of_memory);
        case Z_VERSION_ERROR:
            throw compression_error(compression_config_error);
        case Z_STREAM_ERROR:
            throw compression_error(compression_invalid_parameter);
            case Z_ERRNO:
                throw compression_error(compression_io_error);
            default:
                throw compression_error(compression_unexpected_error);
    }
}


// OBJECTS
// -------

/**
 *  \brief Implied base class for the ZLIB compressor.
 */
struct zlib_compressor_impl: filter_impl<z_stream>
{
    using base = filter_impl<z_stream>;
    zlib_compressor_impl(int level = Z_DEFAULT_COMPRESSION);
    ~zlib_compressor_impl() noexcept;

    virtual void call();
    bool flush(void*& dst, size_t dstlen);
    compression_status operator()(const void*& src, size_t srclen, void*& dst, size_t dstlen);
};


zlib_compressor_impl::zlib_compressor_impl(int level)
{
    status = Z_OK;
    stream.zalloc = Z_NULL;
    stream.zfree = Z_NULL;
    stream.opaque = Z_NULL;
    PYCPP_CHECK(deflateInit(&stream, level));
}


zlib_compressor_impl::~zlib_compressor_impl() noexcept
{
    deflateEnd(&stream);
}


void zlib_compressor_impl::call()
{
    while (stream.avail_in && stream.avail_out && status != Z_STREAM_END) {
        status = deflate(&stream, Z_NO_FLUSH);
        check_zstatus(status);
    }
}


bool zlib_compressor_impl::flush(void*& dst, size_t dstlen)
{
    return base::flush(dst, dstlen, [&]()
    {
        if (dstlen) {
            status = deflate(&stream, Z_FINISH);
            return status == Z_STREAM_END || status == Z_OK;
        } else {
            status = deflate(&stream, Z_FULL_FLUSH);
            return status == Z_STREAM_END || status == Z_OK;
        }
    });
}


compression_status zlib_compressor_impl::operator()(const void*& src, size_t srclen, void*& dst, size_t dstlen)
{
    return base::operator()(src, srclen, dst, dstlen, Z_STREAM_END);
}


/**
 *  \brief Implied base class for the ZLIB decompressor.
 */
struct zlib_decompressor_impl: filter_impl<z_stream>
{
    using base = filter_impl<z_stream>;
    zlib_decompressor_impl();
    ~zlib_decompressor_impl() noexcept;

    virtual void call();
    bool flush(void*& dst, size_t dstlen);
    compression_status operator()(const void*& src, size_t srclen, void*& dst, size_t dstlen);
};


zlib_decompressor_impl::zlib_decompressor_impl()
{
    status = Z_OK;
    stream.zalloc = Z_NULL;
    stream.zfree = Z_NULL;
    stream.opaque = Z_NULL;
    PYCPP_CHECK(inflateInit(&stream));
}


zlib_decompressor_impl::~zlib_decompressor_impl() noexcept
{
    inflateEnd(&stream);
}


void zlib_decompressor_impl::call()
{
    while (stream.avail_in && stream.avail_out && status != Z_STREAM_END) {
        status = inflate(&stream, Z_NO_FLUSH);
        check_zstatus(status);
    }
}



bool zlib_decompressor_impl::flush(void*& dst, size_t dstlen)
{
    // null-op, always flushed
    return true;
}


compression_status zlib_decompressor_impl::operator()(const void*& src, size_t srclen, void*& dst, size_t dstlen)
{
    return base::operator()(src, srclen, dst, dstlen, Z_STREAM_END);
}


zlib_compressor::zlib_compressor(int level):
    ptr_(make_unique<zlib_compressor_impl>(level))
{}


zlib_compressor::zlib_compressor(zlib_compressor&& rhs) noexcept:
    ptr_(move(rhs.ptr_))
{}


zlib_compressor & zlib_compressor::operator=(zlib_compressor&& rhs) noexcept
{
    swap(rhs);
    return *this;
}


zlib_compressor::~zlib_compressor() noexcept
{}


compression_status zlib_compressor::compress(const void*& src, size_t srclen, void*& dst, size_t dstlen)
{
    return (*ptr_)(src, srclen, dst, dstlen);
}


bool zlib_compressor::flush(void*& dst, size_t dstlen)
{
    return ptr_->flush(dst, dstlen);
}


void zlib_compressor::close() noexcept
{
    ptr_.reset();
}


void zlib_compressor::swap(zlib_compressor& rhs) noexcept
{
    using PYCPP_NAMESPACE::swap;
    swap(ptr_, rhs.ptr_);
}


zlib_decompressor::zlib_decompressor():
    ptr_(make_unique<zlib_decompressor_impl>())
{}


zlib_decompressor::zlib_decompressor(zlib_decompressor&& rhs) noexcept:
    ptr_(move(rhs.ptr_))
{}


zlib_decompressor & zlib_decompressor::operator=(zlib_decompressor&& rhs) noexcept
{
    swap(rhs);
    return *this;
}


zlib_decompressor::~zlib_decompressor() noexcept
{}


compression_status zlib_decompressor::decompress(const void*& src, size_t srclen, void*& dst, size_t dstlen)
{
    return (*ptr_)(src, srclen, dst, dstlen);
}


bool zlib_decompressor::flush(void*& dst, size_t dstlen)
{
    return ptr_->flush(dst, dstlen);
}


void zlib_decompressor::close() noexcept
{
    ptr_.reset();
}


void zlib_decompressor::swap(zlib_decompressor& rhs) noexcept
{
    using PYCPP_NAMESPACE::swap;
    swap(ptr_, rhs.ptr_);
}

// FUNCTIONS
// ---------


void zlib_compress(const void*& src, size_t srclen, void* &dst, size_t dstlen)
{
    uLong srclen_ = static_cast<uLong>(srclen);
    uLong dstlen_ = static_cast<uLong>(dstlen);
    if (srclen) {
        PYCPP_CHECK(compress((Bytef*) dst, &dstlen_, (Bytef*) src, srclen_));
    } else {
        // compression no bytes
        Bytef c = 0;
        PYCPP_CHECK(compress((Bytef*) dst, &dstlen_, &c, 0));
    }

    // update pointers
    src = ((char*) src) + srclen_;
    dst = ((char*) dst) + dstlen_;
}


string zlib_compress(const string_wrapper& str)
{
    size_t dstlen = zlib_compress_bound(str.size());
    return compress_bound(str, dstlen, [](const void*& src, size_t srclen, void* &dst, size_t dstlen) {
        return zlib_compress(src, srclen, dst, dstlen);
    });
}


string zlib_decompress(const string_wrapper& str)
{
    return ctx_decompress<zlib_decompressor>(str);
}


void zlib_decompress(const void*& src, size_t srclen, void* &dst, size_t dstlen, size_t bound)
{
    uLong srclen_ = static_cast<uLong>(srclen);
    uLong dstlen_ = static_cast<uLong>(dstlen);
    if (srclen) {
        PYCPP_CHECK(uncompress((Bytef*) dst, &dstlen_, (Bytef*) src, srclen_));
    } else {
        // compression no bytes
        Bytef c = 0;
        PYCPP_CHECK(uncompress((Bytef*) dst, &dstlen_, &c, 0));
    }

    // update pointers
    src = ((char*) src) + srclen_;
    dst = ((char*) dst) + dstlen_;
}


string zlib_decompress(const string_wrapper& str, size_t bound)
{
    return decompress_bound(str, bound, [](const void*& src, size_t srclen, void* &dst, size_t dstlen, size_t bound) {
        zlib_decompress(src, srclen, dst, dstlen, bound);
    });
}


PYCPP_END_NAMESPACE

#endif                  // HAVE_ZLIB
