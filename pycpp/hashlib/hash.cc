//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Generic hash structure.
 */

#include <pycpp/hashlib.h>
#include <pycpp/hex.h>

PYCPP_BEGIN_NAMESPACE

// FUNCTIONS
// ---------


void hash_update(void* ctx, const void* src, long srclen, void (*cb)(void*, const void*, long))
{
    long length = srclen;
    const uint8_t* first = reinterpret_cast<const uint8_t*>(src);

    while (length > 0) {
        long shift = length > 512 ? 512 : length;
        cb(ctx, first, shift);
        length -= shift;
        first += shift;
    }
}


size_t hash_digest(void* ctx, void* dst, long dstlen, long hashlen, void (*cb)(void*, void*))
{
    if (dstlen < hashlen) {
        throw std::runtime_error("dstlen not large enough to store hash digest.");
    }

    cb(ctx, dst);
    return hashlen;
}


secure_string hash_digest(void* ctx, long hashlen, void (*cb)(void*, void*))
{
    void* dst = secure_malloc(hashlen);

    try {
        hash_digest(ctx, dst, hashlen, hashlen, cb);
        secure_string output((char*)dst, hashlen);

        secure_zero(dst, hashlen);
        secure_free(dst);
        return output;

    } catch (std::exception&) {
        secure_free(dst);
        throw;
    }
}


size_t hash_hexdigest(void* ctx, void* dst, long dstlen, long hashlen, void (*cb)(void*, void*))
{
    if (dstlen < 2 * hashlen) {
        throw std::runtime_error("dstlen not large enough to store hash hexdigest.");
    }

    void* hash = secure_malloc(hashlen);

    try {
        hash_digest(ctx, hash, hashlen, hashlen, cb);
        size_t out = hex_i8(hash, hashlen, dst, dstlen);

        secure_free(hash);
        return out;
    } catch (std::exception&) {
        secure_free(hash);
        throw;
    }
}


secure_string hash_hexdigest(void* ctx, long hashlen, void (*cb)(void*, void*))
{
    void* dst = secure_malloc(hashlen * 2);

    try {
        hash_hexdigest(ctx, dst, hashlen * 2, hashlen, cb);
        secure_string output((char*)dst, hashlen * 2);

        secure_zero(dst, hashlen * 2);
        secure_free(dst);
        return output;

    } catch (std::exception&) {
        secure_free(dst);
        throw;
    }
}


/**
 *  \brief Allocate storage for hash.
 */
struct allocate_hash
{
    template <typename Hash>
    void operator()(Hash& hash)
    {
        new (&hash) Hash();
    }
};


/**
 *  \brief Deallocate storage for hash.
 */
struct deallocate_hash
{
    template <typename Hash>
    void operator()(Hash& hash)
    {
        hash.~Hash();
    }
};


/**
 *  \brief Update hash from storage.
 */
struct update_hash
{
    secure_string_view str;

    template <typename Hash>
    void operator()(Hash& hash)
    {
        hash.update(str);
    }
};


/**
 *  \brief Get digest via C-string.
 */
struct digest_cstring
{
    void* dst;
    size_t dstlen;
    size_t length;

    template <typename Hash>
    void operator()(Hash& hash)
    {
        length = hash.digest(dst, dstlen);
    }
};


/**
 *  \brief Get hex digest via C-string.
 */
struct hexdigest_cstring
{
    void* dst;
    size_t dstlen;
    size_t length;

    template <typename Hash>
    void operator()(Hash& hash)
    {
        length = hash.hexdigest(dst, dstlen);
    }
};


/**
 *  \brief Get digest via STL.
 */
struct digest_stl
{
    secure_string str;

    template <typename Hash>
    void operator()(Hash& hash)
    {
        str = hash.digest();
    }
};


/**
 *  \brief Get hex digest via STL.
 */
struct hexdigest_stl
{
    secure_string str;

    template <typename Hash>
    void operator()(Hash& hash)
    {
        str = hash.hexdigest();
    }
};


/**
 *  \brief Cast storage to the correct type.
 */
template <typename Memory, typename Function>
static void get_hash(Memory& mem, hash_algorithm algorithm, Function& function)
{
    switch (algorithm) {
        case md2_hash_algorithm:
            function(reinterpret_cast<md2_hash&>(mem));
            break;

        case md4_hash_algorithm:
            function(reinterpret_cast<md4_hash&>(mem));
            break;

        case md5_hash_algorithm:
            function(reinterpret_cast<md5_hash&>(mem));
            break;

        case sha1_hash_algorithm:
            function(reinterpret_cast<sha1_hash&>(mem));
            break;

        case sha2_224_hash_algorithm:
            function(reinterpret_cast<sha2_224_hash&>(mem));
            break;

        case sha2_256_hash_algorithm:
            function(reinterpret_cast<sha2_256_hash&>(mem));
            break;

        case sha2_384_hash_algorithm:
            function(reinterpret_cast<sha2_384_hash&>(mem));
            break;

        case sha2_512_hash_algorithm:
            function(reinterpret_cast<sha2_512_hash&>(mem));
            break;

        case sha3_224_hash_algorithm:
            function(reinterpret_cast<sha3_224_hash&>(mem));
            break;

        case sha3_256_hash_algorithm:
            function(reinterpret_cast<sha3_256_hash&>(mem));
            break;

        case sha3_384_hash_algorithm:
            function(reinterpret_cast<sha3_384_hash&>(mem));
            break;

        case sha3_512_hash_algorithm:
            function(reinterpret_cast<sha3_512_hash&>(mem));
            break;

        case whirlpool_hash_algorithm:
            function(reinterpret_cast<whirlpool_hash&>(mem));
            break;

        default:
            throw std::runtime_error("Unrecognized hashing algorithm.");
    }
}

// OBJECTS
// -------


hash::hash(hash_algorithm algorithm):
    algorithm(algorithm)
{
    allocate_hash hasher;
    get_hash(mem, algorithm, hasher);
}


hash::hash(hash_algorithm algorithm, const void* src, size_t srclen):
    algorithm(algorithm)
{
    allocate_hash hasher;
    get_hash(mem, algorithm, hasher);
    update(src, srclen);
}


hash::hash(hash_algorithm algorithm, const secure_string_view& str):
    algorithm(algorithm)
{
    allocate_hash hasher;
    get_hash(mem, algorithm, hasher);
    update(str);
}


hash::~hash()
{
    deallocate_hash hasher;
    get_hash(mem, algorithm, hasher);
}


hash::hash(const hash& other):
    algorithm(other.algorithm),
    mem(other.mem)
{}


hash& hash::operator=(const hash& other)
{
    algorithm = other.algorithm;
    mem = other.mem;
    return *this;
}


hash::hash(hash&& other):
    algorithm(std::move(other.algorithm)),
    mem(std::move(other.mem))
{}


hash& hash::operator=(hash&& other)
{
    algorithm = std::move(other.algorithm);
    mem = std::move(other.mem);
    return *this;
}


void hash::update(const void* src, size_t srclen)
{
    update(secure_string_view(reinterpret_cast<const char*>(src), srclen));
}


void hash::update(const secure_string_view& str)
{
    update_hash functor = {str};
    get_hash(mem, algorithm, functor);
}


size_t hash::digest(void* dst, size_t dstlen) const
{
    digest_cstring functor = {dst, dstlen};
    get_hash(const_cast<memory_type&>(mem), algorithm, functor);
    return functor.length;
}


size_t hash::hexdigest(void* dst, size_t dstlen) const
{
    hexdigest_cstring functor = {dst, dstlen};
    get_hash(const_cast<memory_type&>(mem), algorithm, functor);
    return functor.length;
}


secure_string hash::digest() const
{
    digest_stl functor;
    get_hash(const_cast<memory_type&>(mem), algorithm, functor);
    return std::move(functor.str);
}


secure_string hash::hexdigest() const
{
    hexdigest_stl functor;
    get_hash(const_cast<memory_type&>(mem), algorithm, functor);
    return std::move(functor.str);
}

PYCPP_END_NAMESPACE