//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief BZIP2 compression and decompression unittests.
 */

#if defined(HAVE_LZMA)

#include <pycpp/compression/lzma.h>
#include <pycpp/stl/sstream.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// DATA
// ----

static std::string LZMA_COMPRESSED("\xfd\x37\x7a\x58\x5a\x00\x00\x04\xe6\xd6\xb4\x46\x02\x00\x21\x01\x16\x00\x00\x00\x74\x2f\xe5\xa3\xe0\x04\x55\x02\xb3\x5d\x00\x2a\x1a\x08\xa2\x01\xfb\xd6\x96\xf1\x8a\x93\x97\x90\x87\xd3\xb6\x8a\x4a\x8d\x93\xc0\x85\xc1\xd4\xaa\x09\x8a\x40\x26\xae\xa5\xdf\x7f\xe7\x56\xaa\xd3\xcd\x6e\xd1\xfa\x9e\xbf\x49\x87\xa9\x43\x1c\x17\x37\x2b\xc4\x2b\x5a\xa2\x27\x92\x06\x20\x9f\xb1\x2c\x26\x61\x72\x91\x71\xd2\x9a\x7f\x6f\xf3\xbb\xdb\xea\x8b\xd4\xa1\x34\x69\xaf\x06\x42\x9f\xaa\xa3\x6d\xd4\xef\x75\xd4\x2c\x81\xac\xb2\x2f\x3b\xe7\x9e\xed\x93\xe8\xd0\xa1\x0f\x11\x74\xf2\xf6\x54\xb8\xee\xd8\x84\xae\x36\x21\xca\x10\xe4\xea\xa8\xc7\xe1\x98\xaf\x75\x4d\x4a\x31\x99\xad\x24\x81\xfd\x43\xd3\x0e\x10\x84\xcc\x31\xf0\x70\x44\x83\x6b\xc4\x2d\xa9\xff\x43\x62\x5a\x40\xe9\x21\x1a\x8e\xb1\x29\xb1\xf6\xe7\xe8\x34\x61\xc8\x5c\xce\x95\x1c\x2e\x79\xa8\xcb\x22\xb0\x37\x1f\x9a\xe2\xa2\xb2\x71\x3b\x46\x80\xf4\xc7\x45\xe3\xa4\x37\x55\xc8\x7c\xe5\x0a\x5c\x2b\xcc\x8f\xa4\x85\x0b\xae\x2d\x12\xda\xce\xbc\xd2\xfd\xc6\x0d\x14\xe2\x02\x91\x3c\x14\x4b\x0c\x2f\xbf\x87\x5c\x4a\x19\x17\x24\xb4\x4c\x28\x52\xcc\xed\x9d\x12\x9d\x08\x44\xeb\x17\x61\xc8\x00\x0e\xa7\x89\x9a\x7e\x03\xa2\x08\x7a\xc7\xc4\x02\x44\xe6\xd2\xff\x8a\x96\xf6\x24\x71\xb7\x8b\x73\x37\x43\x29\x69\x67\x50\x01\x29\xd7\xeb\xc6\x09\x56\x68\xf3\x37\x77\xad\x23\x57\xe8\x09\x51\xb4\x02\x41\x93\x3c\x1e\xb3\xbb\x47\x06\x3b\xe9\xf7\x0b\xa1\xff\x90\x86\x35\xd0\x4b\xd7\x24\xb0\x77\x5e\xce\xd4\x6a\x8a\x90\x4d\xa0\x46\x46\x5d\xe3\x27\x58\x36\xc7\xe4\x64\x26\xa4\xb6\x9f\x85\x2f\xe2\xad\x6f\x04\x22\x6a\xcc\xb6\xbf\x26\xd7\xae\x26\x01\x67\xe4\x4c\x6b\x10\xac\x9a\xca\x58\x6d\xb7\x81\x8e\x8e\x78\x8c\xd1\xbc\x18\x37\x48\x3b\x18\xde\x10\xf5\xfd\x34\xf1\x2f\x9d\x35\x33\x2b\x81\x08\xc3\xd3\xdc\xc1\x87\xa5\xdd\x0b\xf4\x63\xeb\xcc\x0f\xb1\xf1\xcc\x86\xfc\xa7\x14\x32\x45\x4c\x79\x57\x93\x54\x65\xac\xac\x08\xbb\xbb\xe9\xb6\x3a\xcf\xa3\x6f\x69\x85\x0a\x21\x09\x89\xde\x02\x14\xe0\x21\x4f\x37\x7f\x72\x2d\x57\xb6\x4a\x8b\x3b\x0b\x02\xf3\x4a\x16\x16\x33\x46\x6d\x29\xb8\x31\xfb\x26\x07\xfe\x75\xb3\xb1\xe4\x56\x6e\x7f\xbf\x75\xc1\xb0\x38\x1f\x26\x7a\x65\x6f\x08\xbe\x1d\x53\xab\x04\x18\x07\xd0\x84\x3e\xef\xf9\xf9\x4d\x28\x62\x62\xeb\xe4\x42\x18\xa1\x6c\x16\x75\x4d\x5b\xf8\x51\xba\x77\xb6\x5a\xe6\xf2\x14\x7e\x24\x3b\xe5\x49\x80\x23\x69\x0e\x97\xf5\x16\x89\x7c\xac\x6f\xa5\x33\xd6\x70\x47\xd2\xda\x48\xd9\xa9\x41\xe2\xe4\x9b\xf1\x7e\xb4\xca\xf1\x0d\x5f\x5e\x78\xb2\xfb\x62\x47\xe8\x53\x96\xa1\xa9\xb1\x68\x70\x82\x39\x6c\x55\x53\x0c\xc3\xc9\x09\x91\x90\x6c\x85\xc0\x5a\xd7\x45\xf0\x6d\x0e\x68\x6c\x53\x74\xf0\xf9\x0e\x31\x52\x9d\x0f\x2b\x7f\x3b\xc6\x29\x53\xb0\x22\x48\xde\x30\x50\x18\x5f\x23\x33\x94\x30\xc7\x06\x66\xf7\x95\x29\x2f\x81\x84\x6f\x1d\x35\xbb\x98\xb5\x0a\xcf\x1a\x3a\x55\xe4\xb8\xdf\xa2\xa3\x8f\xa9\x83\xe7\xb3\x70\x54\x99\x74\x39\x0a\x93\x69\x75\xfe\x83\x55\xe9\xc2\x7c\x80\x69\xf7\xb6\x12\xcb\xcf\x75\xef\x6b\xa8\x1f\x69\x9a\x67\xde\x09\x17\xed\x1f\xec\x58\x00\x00\xff\x9a\x61\x8b\x57\xe4\x80\xdb\x00\x01\xcf\x05\xd6\x08\x00\x00\x31\x40\x27\xe6\xb1\xc4\x67\xfb\x02\x00\x00\x00\x00\x04\x59\x5a", 756);
static std::string LZMA_DECOMPRESSED("\x54\x68\x65\x20\x4d\x49\x54\x20\x4c\x69\x63\x65\x6e\x73\x65\x20\x28\x4d\x49\x54\x29\x0a\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x0a\x0a\x43\x6f\x70\x79\x72\x69\x67\x68\x74\x20\xc2\xa9\x20\x60\x32\x30\x31\x37\x60\x20\x60\x53\x69\x72\x20\x42\x65\x64\x69\x76\x65\x72\x65\x0a\x50\x65\x72\x6d\x69\x73\x73\x69\x6f\x6e\x20\x69\x73\x20\x68\x65\x72\x65\x62\x79\x20\x67\x72\x61\x6e\x74\x65\x64\x2c\x20\x66\x72\x65\x65\x20\x6f\x66\x20\x63\x68\x61\x72\x67\x65\x2c\x20\x74\x6f\x20\x61\x6e\x79\x20\x70\x65\x72\x73\x6f\x6e\x0a\x6f\x62\x74\x61\x69\x6e\x69\x6e\x67\x20\x61\x20\x63\x6f\x70\x79\x20\x6f\x66\x20\x74\x68\x69\x73\x20\x73\x6f\x66\x74\x77\x61\x72\x65\x20\x61\x6e\x64\x20\x61\x73\x73\x6f\x63\x69\x61\x74\x65\x64\x20\x64\x6f\x63\x75\x6d\x65\x6e\x74\x61\x74\x69\x6f\x6e\x0a\x66\x69\x6c\x65\x73\x20\x28\x74\x68\x65\x20\xe2\x80\x9c\x53\x6f\x66\x74\x77\x61\x72\x65\xe2\x80\x9d\x29\x2c\x20\x74\x6f\x20\x64\x65\x61\x6c\x20\x69\x6e\x20\x74\x68\x65\x20\x53\x6f\x66\x74\x77\x61\x72\x65\x20\x77\x69\x74\x68\x6f\x75\x74\x0a\x72\x65\x73\x74\x72\x69\x63\x74\x69\x6f\x6e\x2c\x20\x69\x6e\x63\x6c\x75\x64\x69\x6e\x67\x20\x77\x69\x74\x68\x6f\x75\x74\x20\x6c\x69\x6d\x69\x74\x61\x74\x69\x6f\x6e\x20\x74\x68\x65\x20\x72\x69\x67\x68\x74\x73\x20\x74\x6f\x20\x75\x73\x65\x2c\x0a\x63\x6f\x70\x79\x2c\x20\x6d\x6f\x64\x69\x66\x79\x2c\x20\x6d\x65\x72\x67\x65\x2c\x20\x70\x75\x62\x6c\x69\x73\x68\x2c\x20\x64\x69\x73\x74\x72\x69\x62\x75\x74\x65\x2c\x20\x73\x75\x62\x6c\x69\x63\x65\x6e\x73\x65\x2c\x20\x61\x6e\x64\x2f\x6f\x72\x20\x73\x65\x6c\x6c\x0a\x63\x6f\x70\x69\x65\x73\x20\x6f\x66\x20\x74\x68\x65\x20\x53\x6f\x66\x74\x77\x61\x72\x65\x2c\x20\x61\x6e\x64\x20\x74\x6f\x20\x70\x65\x72\x6d\x69\x74\x20\x70\x65\x72\x73\x6f\x6e\x73\x20\x74\x6f\x20\x77\x68\x6f\x6d\x20\x74\x68\x65\x0a\x53\x6f\x66\x74\x77\x61\x72\x65\x20\x69\x73\x20\x66\x75\x72\x6e\x69\x73\x68\x65\x64\x20\x74\x6f\x20\x64\x6f\x20\x73\x6f\x2c\x20\x73\x75\x62\x6a\x65\x63\x74\x20\x74\x6f\x20\x74\x68\x65\x20\x66\x6f\x6c\x6c\x6f\x77\x69\x6e\x67\x0a\x63\x6f\x6e\x64\x69\x74\x69\x6f\x6e\x73\x3a\x0a\x0a\x54\x68\x65\x20\x61\x62\x6f\x76\x65\x20\x63\x6f\x70\x79\x72\x69\x67\x68\x74\x20\x6e\x6f\x74\x69\x63\x65\x20\x61\x6e\x64\x20\x74\x68\x69\x73\x20\x70\x65\x72\x6d\x69\x73\x73\x69\x6f\x6e\x20\x6e\x6f\x74\x69\x63\x65\x20\x73\x68\x61\x6c\x6c\x20\x62\x65\x0a\x69\x6e\x63\x6c\x75\x64\x65\x64\x20\x69\x6e\x20\x61\x6c\x6c\x20\x63\x6f\x70\x69\x65\x73\x20\x6f\x72\x20\x73\x75\x62\x73\x74\x61\x6e\x74\x69\x61\x6c\x20\x70\x6f\x72\x74\x69\x6f\x6e\x73\x20\x6f\x66\x20\x74\x68\x65\x20\x53\x6f\x66\x74\x77\x61\x72\x65\x2e\x0a\x0a\x54\x48\x45\x20\x53\x4f\x46\x54\x57\x41\x52\x45\x20\x49\x53\x20\x50\x52\x4f\x56\x49\x44\x45\x44\x20\xe2\x80\x9c\x41\x53\x20\x49\x53\xe2\x80\x9d\x2c\x20\x57\x49\x54\x48\x4f\x55\x54\x20\x57\x41\x52\x52\x41\x4e\x54\x59\x20\x4f\x46\x20\x41\x4e\x59\x20\x4b\x49\x4e\x44\x2c\x0a\x45\x58\x50\x52\x45\x53\x53\x20\x4f\x52\x20\x49\x4d\x50\x4c\x49\x45\x44\x2c\x20\x49\x4e\x43\x4c\x55\x44\x49\x4e\x47\x20\x42\x55\x54\x20\x4e\x4f\x54\x20\x4c\x49\x4d\x49\x54\x45\x44\x20\x54\x4f\x20\x54\x48\x45\x20\x57\x41\x52\x52\x41\x4e\x54\x49\x45\x53\x0a\x4f\x46\x20\x4d\x45\x52\x43\x48\x41\x4e\x54\x41\x42\x49\x4c\x49\x54\x59\x2c\x20\x46\x49\x54\x4e\x45\x53\x53\x20\x46\x4f\x52\x20\x41\x20\x50\x41\x52\x54\x49\x43\x55\x4c\x41\x52\x20\x50\x55\x52\x50\x4f\x53\x45\x20\x41\x4e\x44\x0a\x4e\x4f\x4e\x49\x4e\x46\x52\x49\x4e\x47\x45\x4d\x45\x4e\x54\x2e\x20\x49\x4e\x20\x4e\x4f\x20\x45\x56\x45\x4e\x54\x20\x53\x48\x41\x4c\x4c\x20\x54\x48\x45\x20\x41\x55\x54\x48\x4f\x52\x53\x20\x4f\x52\x20\x43\x4f\x50\x59\x52\x49\x47\x48\x54\x0a\x48\x4f\x4c\x44\x45\x52\x53\x20\x42\x45\x20\x4c\x49\x41\x42\x4c\x45\x20\x46\x4f\x52\x20\x41\x4e\x59\x20\x43\x4c\x41\x49\x4d\x2c\x20\x44\x41\x4d\x41\x47\x45\x53\x20\x4f\x52\x20\x4f\x54\x48\x45\x52\x20\x4c\x49\x41\x42\x49\x4c\x49\x54\x59\x2c\x0a\x57\x48\x45\x54\x48\x45\x52\x20\x49\x4e\x20\x41\x4e\x20\x41\x43\x54\x49\x4f\x4e\x20\x4f\x46\x20\x43\x4f\x4e\x54\x52\x41\x43\x54\x2c\x20\x54\x4f\x52\x54\x20\x4f\x52\x20\x4f\x54\x48\x45\x52\x57\x49\x53\x45\x2c\x20\x41\x52\x49\x53\x49\x4e\x47\x0a\x46\x52\x4f\x4d\x2c\x20\x4f\x55\x54\x20\x4f\x46\x20\x4f\x52\x20\x49\x4e\x20\x43\x4f\x4e\x4e\x45\x43\x54\x49\x4f\x4e\x20\x57\x49\x54\x48\x20\x54\x48\x45\x20\x53\x4f\x46\x54\x57\x41\x52\x45\x20\x4f\x52\x20\x54\x48\x45\x20\x55\x53\x45\x20\x4f\x52\x0a\x4f\x54\x48\x45\x52\x20\x44\x45\x41\x4c\x49\x4e\x47\x53\x20\x49\x4e\x20\x54\x48\x45\x20\x53\x4f\x46\x54\x57\x41\x52\x45\x2e\x0a", 1110);

// TESTS
// -----


TEST(lzma, lzma_compressor)
{
    std::string lzma = LZMA_DECOMPRESSED;
    const void* src;
    void* dst;
    char* buffer = nullptr;

    try {
        buffer = new char[4096];

        // first example
        lzma_compressor ctx;
        src = lzma.data();
        dst = buffer;
        EXPECT_EQ(ctx.compress(src, lzma.size(), dst, 0), compression_need_output);
        ctx.compress(src, lzma.size(), dst, 4096);
        EXPECT_TRUE(ctx.flush(dst, 4096));
        EXPECT_EQ(std::distance(buffer, (char*) dst), LZMA_COMPRESSED.size());
        EXPECT_EQ(strncmp(buffer, LZMA_COMPRESSED.data(), LZMA_COMPRESSED.size()), 0);

        // second example
        ctx = lzma_compressor();
        src = lzma.data();
        dst = buffer;
        ctx.compress(src, lzma.size(), dst, 4096);
        EXPECT_TRUE(ctx.flush(dst, 4096));
        EXPECT_EQ(std::distance(buffer, (char*) dst), LZMA_COMPRESSED.size());
        EXPECT_EQ(strncmp(buffer, LZMA_COMPRESSED.data(), LZMA_COMPRESSED.size()), 0);

    } catch(...) {
        delete[] buffer;
        throw;
    }

    delete[] buffer;
}


TEST(lzma, lzma_decompressor)
{
    std::string lzma = LZMA_COMPRESSED;
    const void* src;
    void* dst;
    char* buffer = nullptr;

    try {
        buffer = new char[4096];

        // first example
        lzma_decompressor ctx;
        src = lzma.data();
        dst = buffer;
        ctx.decompress(src, lzma.size(), dst, 0);
        ctx.decompress(src, lzma.size(), dst, 4096);
        EXPECT_EQ(std::distance(buffer, (char*) dst), LZMA_DECOMPRESSED.size());
        EXPECT_EQ(strncmp(buffer, LZMA_DECOMPRESSED.data(), LZMA_DECOMPRESSED.size()), 0);

        // second example
        ctx = lzma_decompressor();
        src = lzma.data();
        dst = buffer;
        ctx.decompress(src, lzma.size(), dst, 4096);
        EXPECT_EQ(std::distance(buffer, (char*) dst), LZMA_DECOMPRESSED.size());
        EXPECT_EQ(strncmp(buffer, LZMA_DECOMPRESSED.data(), LZMA_DECOMPRESSED.size()), 0);

    } catch(...) {
        delete[] buffer;
        throw;
    }

    delete[] buffer;
}


TEST(lzma, lzma_compress)
{
    // don't tie to any specific configuration
    EXPECT_EQ(lzma_decompress(lzma_compress(LZMA_DECOMPRESSED)), LZMA_DECOMPRESSED);
}


TEST(lzma, lzma_decompress)
{
    EXPECT_EQ(lzma_decompress(LZMA_COMPRESSED), LZMA_DECOMPRESSED);
    EXPECT_EQ(lzma_decompress(LZMA_COMPRESSED, LZMA_DECOMPRESSED.size()), LZMA_DECOMPRESSED);
}

#endif                  // HAVE_LZMA
