//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Filtering compression stream unittests.
 */

#include <pycpp/compression/stream.h>
#include <gtest/gtest.h>
#include <sstream>

PYCPP_USING_NAMESPACE

// DATA
// ----

static std::string BZ2_COMPRESSED("\x42\x5a\x68\x39\x31\x41\x59\x26\x53\x59\xfd\x9e\x0c\x3e\x00\x01\x20\xff\xf5\x00\x10\x20\x00\x40\x65\xf0\x92\x3f\xef\xdf\xe0\x7f\xf7\xdf\xa0\x40\x00\x00\x06\x00\x20\x10\x00\x10\x00\x40\x02\x88\xa7\x15\xd0\x68\x89\xa2\x30\x93\x4d\x03\xd4\x68\x1a\x7a\x87\xa8\xf5\x0c\x80\xd0\x7a\x80\x00\xd4\xc0\x9a\x23\x48\xd4\xf2\x8f\x51\xa4\xc8\xc4\x00\xc0\x68\x21\xa6\x02\x2a\x7f\xa2\x6a\x86\x08\x68\xd3\x26\x9a\x00\x00\x64\x00\x34\x00\x25\x34\x9a\x10\x53\xc2\x9b\xd2\x20\x40\x01\xa6\x81\xa0\x68\xd0\xc8\x04\xf1\x9e\x04\x2d\x96\x98\xc9\xc1\x11\x0a\x75\xef\x2d\x27\x17\xca\x67\xc4\xd0\x98\xb8\x94\x69\x2c\x3e\x84\xa1\x29\x17\x15\x2a\x80\x9b\x88\xaa\x70\xc9\x98\x8b\x55\x1c\xcd\xa6\x1c\x91\x26\x23\xe2\xc8\xcd\x2a\x4b\xe9\xcd\x57\xb6\x97\x40\xa3\xa7\xa7\x3e\xfd\xfa\x10\x61\xe1\xd9\xb3\xb4\xba\xdd\x9c\x9d\x13\x67\x91\x2c\xd5\xa9\xc4\x80\x8d\x1c\x4c\x84\x1b\x4e\x6f\xc2\x4f\x35\x0b\xe3\x4b\xc3\x9d\xc8\xe0\xc5\x60\x38\xa5\x97\x24\xe6\x19\x7a\x2f\x13\x6f\x74\xa2\x9b\xbb\x8f\x0b\x6b\xcc\x2e\x04\x3e\x65\x1a\x91\x44\xf7\xa8\x5b\x89\xa1\x7b\xa2\x1c\x86\x5b\x64\x6c\xb0\x81\x4e\x05\x08\x79\x10\xa2\x14\xfc\x30\x69\x20\x81\xc5\xc2\x43\x05\x6f\xc1\x90\x11\x30\x5a\xed\xfc\x59\x2b\xd8\xa6\xb2\x06\x7a\xab\x89\xd3\x9f\xbb\x2a\x48\x98\xd9\xa7\x0b\x6d\x5f\xb0\x0e\x4c\x89\x23\x96\x30\x77\xdb\x69\x89\x0d\xd8\x31\x7e\x20\xf5\x99\xe1\x88\xbd\x70\x12\xee\xb5\xb1\x87\xa0\x9d\x94\xf7\x24\x87\xc6\x84\x2d\x76\xb6\x93\x39\x16\xbb\x0a\xb2\x95\x23\x84\x1f\x49\x90\x9a\x69\x27\x5b\xb5\x76\x20\xa7\x05\x71\x52\x13\x56\xb3\x28\xac\xdc\x96\xc0\xaf\x13\x53\x8c\x43\x52\xfc\xe9\x84\xda\x77\xbd\x9f\xb2\x46\x74\x41\xa2\x49\x23\x45\x8d\x48\x86\xc7\xa7\x29\x29\x5a\x44\xee\x1d\xca\xf1\xf3\x44\xa5\x1b\xa9\x57\xc9\x26\x83\xc5\x9c\x9b\xa2\x7b\xa0\xe0\x57\x23\x4c\xd0\x18\x8a\x10\x13\xb8\x84\xda\x27\x01\x26\x13\x5c\x89\x05\x06\x8f\x8c\xf4\xb8\x63\x63\xcd\xa6\x5f\x93\x93\xe2\x26\xd7\xf6\x4c\x3a\xbd\x1d\xbd\x50\xc7\x5c\xdf\xbc\x68\x79\xc0\xfd\xcd\xf5\x5e\xe9\xde\x82\x72\xf8\x0f\x92\x10\x0a\xaa\xa8\x40\x00\x19\x45\x80\xd1\x86\x4e\x37\x3e\x93\xa0\x49\xe3\xda\x25\x23\x62\x1a\x98\xe3\x54\x27\x5a\xee\x89\x51\xfa\x27\x5b\x8f\xbe\x35\x7e\x1d\xe1\xdb\xb1\x79\xb0\x6f\xb0\xe3\x28\xd9\xa6\x3b\x4b\x23\x2d\x39\x93\x24\xd2\x87\x39\x51\xb3\x2a\x5c\xe7\x93\x29\x66\x6c\xed\xdb\x18\x1a\xac\x74\xaf\x3e\x39\x88\x3a\x6a\x6f\x30\xbb\x79\x7a\x30\x1c\xe7\x9b\x62\x6d\x2b\xd7\x3c\x1c\x29\xb0\xfd\x15\xb3\x5e\xf0\x3a\x12\x11\x50\x77\x08\x82\x4d\x73\x4d\xb5\x0c\x53\x71\xee\x52\xd2\xe0\x96\xe6\x2d\x6c\xa1\x06\x89\xd3\xb1\xe6\xef\xbb\x87\x02\xe3\x7e\xf9\x85\x84\x20\xc1\x1c\xbb\x6c\xc2\xd2\xd2\x92\xd8\xfc\x17\xc3\x72\x22\xf5\x80\xb3\x4e\x56\x4d\x35\x48\x4b\x20\x64\x5a\x04\x15\xd4\x74\x69\x40\xd6\xb9\x9b\xd2\xac\xf4\x48\x12\xe9\x20\xc1\x2c\x18\x56\xf6\x79\x4a\x82\xed\x46\x72\xca\xc5\xd1\x4c\x0c\xd5\xfa\x8d\x12\x44\x5f\x95\x5d\xbf\xc1\xfd\x0f\xf5\x5a\xdc\xdf\xf1\x77\x24\x53\x85\x09\x0f\xd9\xe0\xc3\xe0", 710);
static std::string BZ2_DECOMPRESSED("\x54\x68\x65\x20\x4d\x49\x54\x20\x4c\x69\x63\x65\x6e\x73\x65\x20\x28\x4d\x49\x54\x29\x0a\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x3d\x0a\x0a\x43\x6f\x70\x79\x72\x69\x67\x68\x74\x20\xc2\xa9\x20\x60\x32\x30\x31\x37\x60\x20\x60\x53\x69\x72\x20\x42\x65\x64\x69\x76\x65\x72\x65\x0a\x50\x65\x72\x6d\x69\x73\x73\x69\x6f\x6e\x20\x69\x73\x20\x68\x65\x72\x65\x62\x79\x20\x67\x72\x61\x6e\x74\x65\x64\x2c\x20\x66\x72\x65\x65\x20\x6f\x66\x20\x63\x68\x61\x72\x67\x65\x2c\x20\x74\x6f\x20\x61\x6e\x79\x20\x70\x65\x72\x73\x6f\x6e\x0a\x6f\x62\x74\x61\x69\x6e\x69\x6e\x67\x20\x61\x20\x63\x6f\x70\x79\x20\x6f\x66\x20\x74\x68\x69\x73\x20\x73\x6f\x66\x74\x77\x61\x72\x65\x20\x61\x6e\x64\x20\x61\x73\x73\x6f\x63\x69\x61\x74\x65\x64\x20\x64\x6f\x63\x75\x6d\x65\x6e\x74\x61\x74\x69\x6f\x6e\x0a\x66\x69\x6c\x65\x73\x20\x28\x74\x68\x65\x20\xe2\x80\x9c\x53\x6f\x66\x74\x77\x61\x72\x65\xe2\x80\x9d\x29\x2c\x20\x74\x6f\x20\x64\x65\x61\x6c\x20\x69\x6e\x20\x74\x68\x65\x20\x53\x6f\x66\x74\x77\x61\x72\x65\x20\x77\x69\x74\x68\x6f\x75\x74\x0a\x72\x65\x73\x74\x72\x69\x63\x74\x69\x6f\x6e\x2c\x20\x69\x6e\x63\x6c\x75\x64\x69\x6e\x67\x20\x77\x69\x74\x68\x6f\x75\x74\x20\x6c\x69\x6d\x69\x74\x61\x74\x69\x6f\x6e\x20\x74\x68\x65\x20\x72\x69\x67\x68\x74\x73\x20\x74\x6f\x20\x75\x73\x65\x2c\x0a\x63\x6f\x70\x79\x2c\x20\x6d\x6f\x64\x69\x66\x79\x2c\x20\x6d\x65\x72\x67\x65\x2c\x20\x70\x75\x62\x6c\x69\x73\x68\x2c\x20\x64\x69\x73\x74\x72\x69\x62\x75\x74\x65\x2c\x20\x73\x75\x62\x6c\x69\x63\x65\x6e\x73\x65\x2c\x20\x61\x6e\x64\x2f\x6f\x72\x20\x73\x65\x6c\x6c\x0a\x63\x6f\x70\x69\x65\x73\x20\x6f\x66\x20\x74\x68\x65\x20\x53\x6f\x66\x74\x77\x61\x72\x65\x2c\x20\x61\x6e\x64\x20\x74\x6f\x20\x70\x65\x72\x6d\x69\x74\x20\x70\x65\x72\x73\x6f\x6e\x73\x20\x74\x6f\x20\x77\x68\x6f\x6d\x20\x74\x68\x65\x0a\x53\x6f\x66\x74\x77\x61\x72\x65\x20\x69\x73\x20\x66\x75\x72\x6e\x69\x73\x68\x65\x64\x20\x74\x6f\x20\x64\x6f\x20\x73\x6f\x2c\x20\x73\x75\x62\x6a\x65\x63\x74\x20\x74\x6f\x20\x74\x68\x65\x20\x66\x6f\x6c\x6c\x6f\x77\x69\x6e\x67\x0a\x63\x6f\x6e\x64\x69\x74\x69\x6f\x6e\x73\x3a\x0a\x0a\x54\x68\x65\x20\x61\x62\x6f\x76\x65\x20\x63\x6f\x70\x79\x72\x69\x67\x68\x74\x20\x6e\x6f\x74\x69\x63\x65\x20\x61\x6e\x64\x20\x74\x68\x69\x73\x20\x70\x65\x72\x6d\x69\x73\x73\x69\x6f\x6e\x20\x6e\x6f\x74\x69\x63\x65\x20\x73\x68\x61\x6c\x6c\x20\x62\x65\x0a\x69\x6e\x63\x6c\x75\x64\x65\x64\x20\x69\x6e\x20\x61\x6c\x6c\x20\x63\x6f\x70\x69\x65\x73\x20\x6f\x72\x20\x73\x75\x62\x73\x74\x61\x6e\x74\x69\x61\x6c\x20\x70\x6f\x72\x74\x69\x6f\x6e\x73\x20\x6f\x66\x20\x74\x68\x65\x20\x53\x6f\x66\x74\x77\x61\x72\x65\x2e\x0a\x0a\x54\x48\x45\x20\x53\x4f\x46\x54\x57\x41\x52\x45\x20\x49\x53\x20\x50\x52\x4f\x56\x49\x44\x45\x44\x20\xe2\x80\x9c\x41\x53\x20\x49\x53\xe2\x80\x9d\x2c\x20\x57\x49\x54\x48\x4f\x55\x54\x20\x57\x41\x52\x52\x41\x4e\x54\x59\x20\x4f\x46\x20\x41\x4e\x59\x20\x4b\x49\x4e\x44\x2c\x0a\x45\x58\x50\x52\x45\x53\x53\x20\x4f\x52\x20\x49\x4d\x50\x4c\x49\x45\x44\x2c\x20\x49\x4e\x43\x4c\x55\x44\x49\x4e\x47\x20\x42\x55\x54\x20\x4e\x4f\x54\x20\x4c\x49\x4d\x49\x54\x45\x44\x20\x54\x4f\x20\x54\x48\x45\x20\x57\x41\x52\x52\x41\x4e\x54\x49\x45\x53\x0a\x4f\x46\x20\x4d\x45\x52\x43\x48\x41\x4e\x54\x41\x42\x49\x4c\x49\x54\x59\x2c\x20\x46\x49\x54\x4e\x45\x53\x53\x20\x46\x4f\x52\x20\x41\x20\x50\x41\x52\x54\x49\x43\x55\x4c\x41\x52\x20\x50\x55\x52\x50\x4f\x53\x45\x20\x41\x4e\x44\x0a\x4e\x4f\x4e\x49\x4e\x46\x52\x49\x4e\x47\x45\x4d\x45\x4e\x54\x2e\x20\x49\x4e\x20\x4e\x4f\x20\x45\x56\x45\x4e\x54\x20\x53\x48\x41\x4c\x4c\x20\x54\x48\x45\x20\x41\x55\x54\x48\x4f\x52\x53\x20\x4f\x52\x20\x43\x4f\x50\x59\x52\x49\x47\x48\x54\x0a\x48\x4f\x4c\x44\x45\x52\x53\x20\x42\x45\x20\x4c\x49\x41\x42\x4c\x45\x20\x46\x4f\x52\x20\x41\x4e\x59\x20\x43\x4c\x41\x49\x4d\x2c\x20\x44\x41\x4d\x41\x47\x45\x53\x20\x4f\x52\x20\x4f\x54\x48\x45\x52\x20\x4c\x49\x41\x42\x49\x4c\x49\x54\x59\x2c\x0a\x57\x48\x45\x54\x48\x45\x52\x20\x49\x4e\x20\x41\x4e\x20\x41\x43\x54\x49\x4f\x4e\x20\x4f\x46\x20\x43\x4f\x4e\x54\x52\x41\x43\x54\x2c\x20\x54\x4f\x52\x54\x20\x4f\x52\x20\x4f\x54\x48\x45\x52\x57\x49\x53\x45\x2c\x20\x41\x52\x49\x53\x49\x4e\x47\x0a\x46\x52\x4f\x4d\x2c\x20\x4f\x55\x54\x20\x4f\x46\x20\x4f\x52\x20\x49\x4e\x20\x43\x4f\x4e\x4e\x45\x43\x54\x49\x4f\x4e\x20\x57\x49\x54\x48\x20\x54\x48\x45\x20\x53\x4f\x46\x54\x57\x41\x52\x45\x20\x4f\x52\x20\x54\x48\x45\x20\x55\x53\x45\x20\x4f\x52\x0a\x4f\x54\x48\x45\x52\x20\x44\x45\x41\x4c\x49\x4e\x47\x53\x20\x49\x4e\x20\x54\x48\x45\x20\x53\x4f\x46\x54\x57\x41\x52\x45\x2e\x0a", 1110);

// TESTS
// -----


TEST(bz2_stream, bz2_istream)
{
    std::istringstream sstream(BZ2_COMPRESSED);
    bz2_istream bz2(sstream);
    std::ostringstream ostream;
    ostream << bz2.rdbuf();

    EXPECT_EQ(ostream.str(), BZ2_DECOMPRESSED);
}


TEST(bz2_stream, bz2_ostream)
{
    std::ostringstream sstream;
    {
        bz2_ostream bz2(sstream);
        bz2 << BZ2_DECOMPRESSED;
    }

    EXPECT_EQ(sstream.str(), BZ2_COMPRESSED);
}
