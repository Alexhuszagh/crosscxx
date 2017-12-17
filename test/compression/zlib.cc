//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief ZLIB compression and decompression unittests.
 */

#if defined(HAVE_ZLIB)

#include <pycpp/compression/zlib.h>
#include <pycpp/stl/sstream.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// DATA
// ----

static std::string ZLIB_COMPRESSED("\x78\x9c\x6d\x52\x4b\x6e\xdb\x30\x10\xdd\xf3\x14\xb3\x4c\x01\x21\xfd\x6c\x0a\x14\xe8\x82\x96\xe8\x98\xa8\x2c\x0a\x14\x1d\xd7\xbb\xc8\x12\x6d\xb1\x90\x45\x43\xa4\x63\x64\x97\x83\xb4\x07\xe8\x35\x7a\x94\x9c\xa4\x43\xca\x49\xda\xa2\x82\x01\x99\xc3\x99\xf7\x1b\xa9\x4e\xc3\x92\x2b\xc8\x4d\xa3\x07\xa7\xe1\x0a\x0f\x6f\xc8\xe7\xff\x3d\x84\xa4\xf6\xf8\x30\x9a\x7d\xe7\xe1\xd7\x4f\xb8\xfb\xf0\xee\xfd\xc7\x3b\xb8\xab\xcc\x08\x33\xdd\x9a\x7b\x3d\x6a\x52\xea\xf1\x60\x9c\x33\x76\x00\xe3\xa0\xc3\xd2\xf6\x01\xf6\x63\x3d\x78\xdd\x26\xb0\x1b\xb5\x06\xbb\x83\xa6\xab\xc7\xbd\x4e\xc0\x5b\xa8\x87\x07\x38\xea\xd1\xd9\x81\xd8\xad\xaf\xcd\x60\x86\x3d\xd4\xd0\x20\x53\xe8\xf4\x1d\xc2\x38\xbb\xf3\xe7\x7a\xd4\xd8\xdc\x42\xed\x9c\x6d\x4c\x8d\x78\xd0\xda\xe6\x74\xd0\x83\xaf\x3d\xf2\x91\x9d\xe9\xb5\x83\x2b\x8f\x86\x9e\x1e\xbf\x57\x97\x99\xa7\xc7\x1f\x6f\x22\x51\xab\xeb\x1e\xcc\x00\xe1\xfe\xf9\x12\xce\xc6\x77\xf6\xe4\xc9\xa8\x9d\x1f\x4d\x13\x70\x12\x6c\x6a\xfa\x53\x1b\x74\x5c\xae\xa1\x37\x07\x33\xb1\xc4\xf1\x18\x81\x0b\xa0\x27\xa7\x13\x12\xb4\x26\x70\xb0\xad\xd9\x85\xb7\x8e\xd6\x8e\xa7\x6d\x6f\x5c\x97\x40\x6b\x02\xf4\xf6\xe4\xb1\xe8\x42\x31\xe6\x9c\x04\x2f\x6f\xed\x08\x4e\xf7\x7d\x40\x30\xa8\x3d\xfa\x7d\x55\x17\x7b\x02\xcb\x31\x84\xea\x2f\x31\x45\xde\x73\x67\x0f\xa1\x97\xbc\x38\xc1\x98\x76\xa7\x71\x40\x4a\x1d\x67\x5a\x8b\xb1\x45\xc6\x6f\xba\xf1\xa1\x12\xa0\x77\xb6\xef\xed\x19\xad\x21\xe5\xd0\x9a\xe0\xc8\x7d\x22\x44\xe1\x55\xbd\xb5\xf7\x3a\xe6\x3e\x6d\x78\xb0\x1e\xa5\x4e\x12\xc2\x12\x8e\xaf\x9b\xbd\x5c\xb9\xae\xee\x7b\xd8\x6a\x32\x05\x86\xbc\x18\x6f\x28\x3d\xdb\x19\x03\xbd\xf3\xb8\x7c\x83\xd9\x1f\xed\x18\xf9\xfe\xb5\x79\x8d\xfc\x0b\x06\x95\x98\xab\x35\x95\x0c\x78\x05\xa5\x14\xb7\x3c\x63\x59\x58\x24\xad\xb0\x82\x5b\x4c\x60\xcd\xd5\x42\xac\x14\x60\x97\xa4\x85\xda\x80\x98\x03\x2d\x36\xf0\x85\x17\x59\x42\xd8\xd7\x52\xb2\xaa\x02\x21\x81\x2f\xcb\x9c\xb3\x2c\x01\x5e\xa4\xf9\x2a\xe3\xc5\x0d\xcc\x70\xae\x10\xf8\x99\x73\xfc\xbe\x11\x58\x09\x08\xa4\x17\x28\xce\x2a\x82\x60\x4b\x26\xd3\x05\x1e\xe9\x8c\xe7\x5c\x6d\x12\x98\x73\x55\x04\xcc\x39\x82\x52\x28\xa9\x54\x3c\x5d\xe5\x54\x42\xb9\x92\xa5\xa8\x18\xd2\x67\xa4\x10\x05\x2f\xe6\x12\x59\xd8\x92\x15\xea\x1a\x59\x91\x0a\xd8\x2d\x1e\xa0\x5a\xd0\x3c\x8f\x54\x74\x85\xea\x65\xd4\x97\x8a\x72\x23\xf9\xcd\x42\x91\x85\xc8\x33\x86\xc5\x19\x43\x65\x74\x96\xb3\x89\x0a\x4d\xa5\x39\xe5\xcb\x04\x32\xba\xa4\x37\x2c\x4e\x09\x44\x91\xb1\x6d\x52\x47\xd6\x0b\x16\x4b\xc8\x47\xf1\x97\x2a\x2e\x8a\x90\x49\x2a\x0a\x25\xf1\x98\xa0\x4b\xa9\x5e\x46\xd7\xbc\x62\x09\x50\xc9\x2b\x94\x4a\xe6\x52\x20\x7c\x88\x13\x27\x44\x04\xc1\xb9\x82\x4d\x28\x21\x6a\xf8\x6b\x2b\xd8\x12\xce\xab\x2a\xfc\x25\x93\x96\x8c\xd1\x1c\xb1\xaa\x30\xfc\x67\xf3\x35\xf9\x0d\x9b\x11\x64\xfe", 669);
static std::string ZLIB_DECOMPRESSED("\x54\x68\x65\x20\x4d\x49\x54\x20\x4c\x69\x63\x65\x6e\x73\x65\x20\x28\x4d\x49\x54\x29\x0a\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x0a\x0a\x43\x6f\x70\x79\x72\x69\x67\x68\x74\x20\xc2\xa9\x20\x60\x32\x30\x31\x37\x60\x20\x60\x53\x69\x72\x20\x42\x65\x64\x69\x76\x65\x72\x65\x0a\x50\x65\x72\x6d\x69\x73\x73\x69\x6f\x6e\x20\x69\x73\x20\x68\x65\x72\x65\x62\x79\x20\x67\x72\x61\x6e\x74\x65\x64\x2c\x20\x66\x72\x65\x65\x20\x6f\x66\x20\x63\x68\x61\x72\x67\x65\x2c\x20\x74\x6f\x20\x61\x6e\x79\x20\x70\x65\x72\x73\x6f\x6e\x0a\x6f\x62\x74\x61\x69\x6e\x69\x6e\x67\x20\x61\x20\x63\x6f\x70\x79\x20\x6f\x66\x20\x74\x68\x69\x73\x20\x73\x6f\x66\x74\x77\x61\x72\x65\x20\x61\x6e\x64\x20\x61\x73\x73\x6f\x63\x69\x61\x74\x65\x64\x20\x64\x6f\x63\x75\x6d\x65\x6e\x74\x61\x74\x69\x6f\x6e\x0a\x66\x69\x6c\x65\x73\x20\x28\x74\x68\x65\x20\xe2\x80\x9c\x53\x6f\x66\x74\x77\x61\x72\x65\xe2\x80\x9d\x29\x2c\x20\x74\x6f\x20\x64\x65\x61\x6c\x20\x69\x6e\x20\x74\x68\x65\x20\x53\x6f\x66\x74\x77\x61\x72\x65\x20\x77\x69\x74\x68\x6f\x75\x74\x0a\x72\x65\x73\x74\x72\x69\x63\x74\x69\x6f\x6e\x2c\x20\x69\x6e\x63\x6c\x75\x64\x69\x6e\x67\x20\x77\x69\x74\x68\x6f\x75\x74\x20\x6c\x69\x6d\x69\x74\x61\x74\x69\x6f\x6e\x20\x74\x68\x65\x20\x72\x69\x67\x68\x74\x73\x20\x74\x6f\x20\x75\x73\x65\x2c\x0a\x63\x6f\x70\x79\x2c\x20\x6d\x6f\x64\x69\x66\x79\x2c\x20\x6d\x65\x72\x67\x65\x2c\x20\x70\x75\x62\x6c\x69\x73\x68\x2c\x20\x64\x69\x73\x74\x72\x69\x62\x75\x74\x65\x2c\x20\x73\x75\x62\x6c\x69\x63\x65\x6e\x73\x65\x2c\x20\x61\x6e\x64\x2f\x6f\x72\x20\x73\x65\x6c\x6c\x0a\x63\x6f\x70\x69\x65\x73\x20\x6f\x66\x20\x74\x68\x65\x20\x53\x6f\x66\x74\x77\x61\x72\x65\x2c\x20\x61\x6e\x64\x20\x74\x6f\x20\x70\x65\x72\x6d\x69\x74\x20\x70\x65\x72\x73\x6f\x6e\x73\x20\x74\x6f\x20\x77\x68\x6f\x6d\x20\x74\x68\x65\x0a\x53\x6f\x66\x74\x77\x61\x72\x65\x20\x69\x73\x20\x66\x75\x72\x6e\x69\x73\x68\x65\x64\x20\x74\x6f\x20\x64\x6f\x20\x73\x6f\x2c\x20\x73\x75\x62\x6a\x65\x63\x74\x20\x74\x6f\x20\x74\x68\x65\x20\x66\x6f\x6c\x6c\x6f\x77\x69\x6e\x67\x0a\x63\x6f\x6e\x64\x69\x74\x69\x6f\x6e\x73\x3a\x0a\x0a\x54\x68\x65\x20\x61\x62\x6f\x76\x65\x20\x63\x6f\x70\x79\x72\x69\x67\x68\x74\x20\x6e\x6f\x74\x69\x63\x65\x20\x61\x6e\x64\x20\x74\x68\x69\x73\x20\x70\x65\x72\x6d\x69\x73\x73\x69\x6f\x6e\x20\x6e\x6f\x74\x69\x63\x65\x20\x73\x68\x61\x6c\x6c\x20\x62\x65\x0a\x69\x6e\x63\x6c\x75\x64\x65\x64\x20\x69\x6e\x20\x61\x6c\x6c\x20\x63\x6f\x70\x69\x65\x73\x20\x6f\x72\x20\x73\x75\x62\x73\x74\x61\x6e\x74\x69\x61\x6c\x20\x70\x6f\x72\x74\x69\x6f\x6e\x73\x20\x6f\x66\x20\x74\x68\x65\x20\x53\x6f\x66\x74\x77\x61\x72\x65\x2e\x0a\x0a\x54\x48\x45\x20\x53\x4f\x46\x54\x57\x41\x52\x45\x20\x49\x53\x20\x50\x52\x4f\x56\x49\x44\x45\x44\x20\xe2\x80\x9c\x41\x53\x20\x49\x53\xe2\x80\x9d\x2c\x20\x57\x49\x54\x48\x4f\x55\x54\x20\x57\x41\x52\x52\x41\x4e\x54\x59\x20\x4f\x46\x20\x41\x4e\x59\x20\x4b\x49\x4e\x44\x2c\x0a\x45\x58\x50\x52\x45\x53\x53\x20\x4f\x52\x20\x49\x4d\x50\x4c\x49\x45\x44\x2c\x20\x49\x4e\x43\x4c\x55\x44\x49\x4e\x47\x20\x42\x55\x54\x20\x4e\x4f\x54\x20\x4c\x49\x4d\x49\x54\x45\x44\x20\x54\x4f\x20\x54\x48\x45\x20\x57\x41\x52\x52\x41\x4e\x54\x49\x45\x53\x0a\x4f\x46\x20\x4d\x45\x52\x43\x48\x41\x4e\x54\x41\x42\x49\x4c\x49\x54\x59\x2c\x20\x46\x49\x54\x4e\x45\x53\x53\x20\x46\x4f\x52\x20\x41\x20\x50\x41\x52\x54\x49\x43\x55\x4c\x41\x52\x20\x50\x55\x52\x50\x4f\x53\x45\x20\x41\x4e\x44\x0a\x4e\x4f\x4e\x49\x4e\x46\x52\x49\x4e\x47\x45\x4d\x45\x4e\x54\x2e\x20\x49\x4e\x20\x4e\x4f\x20\x45\x56\x45\x4e\x54\x20\x53\x48\x41\x4c\x4c\x20\x54\x48\x45\x20\x41\x55\x54\x48\x4f\x52\x53\x20\x4f\x52\x20\x43\x4f\x50\x59\x52\x49\x47\x48\x54\x0a\x48\x4f\x4c\x44\x45\x52\x53\x20\x42\x45\x20\x4c\x49\x41\x42\x4c\x45\x20\x46\x4f\x52\x20\x41\x4e\x59\x20\x43\x4c\x41\x49\x4d\x2c\x20\x44\x41\x4d\x41\x47\x45\x53\x20\x4f\x52\x20\x4f\x54\x48\x45\x52\x20\x4c\x49\x41\x42\x49\x4c\x49\x54\x59\x2c\x0a\x57\x48\x45\x54\x48\x45\x52\x20\x49\x4e\x20\x41\x4e\x20\x41\x43\x54\x49\x4f\x4e\x20\x4f\x46\x20\x43\x4f\x4e\x54\x52\x41\x43\x54\x2c\x20\x54\x4f\x52\x54\x20\x4f\x52\x20\x4f\x54\x48\x45\x52\x57\x49\x53\x45\x2c\x20\x41\x52\x49\x53\x49\x4e\x47\x0a\x46\x52\x4f\x4d\x2c\x20\x4f\x55\x54\x20\x4f\x46\x20\x4f\x52\x20\x49\x4e\x20\x43\x4f\x4e\x4e\x45\x43\x54\x49\x4f\x4e\x20\x57\x49\x54\x48\x20\x54\x48\x45\x20\x53\x4f\x46\x54\x57\x41\x52\x45\x20\x4f\x52\x20\x54\x48\x45\x20\x55\x53\x45\x20\x4f\x52\x0a\x4f\x54\x48\x45\x52\x20\x44\x45\x41\x4c\x49\x4e\x47\x53\x20\x49\x4e\x20\x54\x48\x45\x20\x53\x4f\x46\x54\x57\x41\x52\x45\x2e\x0a", 1110);

// TESTS
// -----


TEST(zlib, zlib_compressor)
{
    std::string zlib = ZLIB_DECOMPRESSED;
    const void* src;
    void* dst;
    char* buffer = nullptr;

    try {
        buffer = new char[4096];

        // first example
        zlib_compressor ctx;
        src = zlib.data();
        dst = buffer;
        EXPECT_EQ(ctx.compress(src, zlib.size(), dst, 0), compression_need_output);
        ctx.compress(src, zlib.size(), dst, 4096);
        EXPECT_TRUE(ctx.flush(dst, 4096));
        EXPECT_EQ(std::distance(buffer, (char*) dst), ZLIB_COMPRESSED.size());
        EXPECT_EQ(strncmp(buffer, ZLIB_COMPRESSED.data(), ZLIB_COMPRESSED.size()), 0);

        // second example
        ctx = zlib_compressor();
        src = zlib.data();
        dst = buffer;
        ctx.compress(src, zlib.size(), dst, 4096);
        EXPECT_TRUE(ctx.flush(dst, 4096));
        EXPECT_EQ(std::distance(buffer, (char*) dst), ZLIB_COMPRESSED.size());
        EXPECT_EQ(strncmp(buffer, ZLIB_COMPRESSED.data(), ZLIB_COMPRESSED.size()), 0);

    } catch(...) {
        delete[] buffer;
        throw;
    }

    delete[] buffer;
}


TEST(zlib, zlib_decompressor)
{
    std::string zlib = ZLIB_COMPRESSED;
    const void* src;
    void* dst;
    char* buffer = nullptr;

    try {
        buffer = new char[4096];

        // first example
        zlib_decompressor ctx;
        src = zlib.data();
        dst = buffer;
        EXPECT_EQ(ctx.decompress(src, zlib.size(), dst, 0), compression_need_output);
        EXPECT_EQ(ctx.decompress(src, zlib.size(), dst, 4096), compression_eof);
        EXPECT_EQ(std::distance(buffer, (char*) dst), ZLIB_DECOMPRESSED.size());
        EXPECT_EQ(strncmp(buffer, ZLIB_DECOMPRESSED.data(), ZLIB_DECOMPRESSED.size()), 0);

        // second example
        ctx = zlib_decompressor();
        src = zlib.data();
        dst = buffer;
        EXPECT_EQ(ctx.decompress(src, zlib.size(), dst, 4096), compression_eof);
        EXPECT_EQ(std::distance(buffer, (char*) dst), ZLIB_DECOMPRESSED.size());
        EXPECT_EQ(strncmp(buffer, ZLIB_DECOMPRESSED.data(), ZLIB_DECOMPRESSED.size()), 0);

    } catch(...) {
        delete[] buffer;
        throw;
    }

    delete[] buffer;
}


TEST(zlib, zlib_compress)
{
    EXPECT_EQ(zlib_compress(ZLIB_DECOMPRESSED), ZLIB_COMPRESSED);
}


TEST(zlib, zlib_decompress)
{
    EXPECT_EQ(zlib_decompress(ZLIB_COMPRESSED), ZLIB_DECOMPRESSED);
    EXPECT_EQ(zlib_decompress(ZLIB_COMPRESSED, ZLIB_DECOMPRESSED.size()), ZLIB_DECOMPRESSED);
}

#endif                  // HAVE_ZLIB
