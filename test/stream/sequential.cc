//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Sequential file I/O unittests.
 */

#include <pycpp/filesystem.h>
#include <pycpp/stream/sequential.h>
#include <warnings/push.h>
#include <warnings/narrowing-conversions.h>
#include <gtest/gtest.h>
#if defined(HAVE_WFOPEN)
#   include <io.h>
#endif

 PYCPP_USING_NAMESPACE

static const std::string UTF8_ENGLISH = {69, 110, 103, 108, 105, 115, 104};
static const std::string UTF8_KOREAN = {-19, -107, -100, -22, -75, -83, -20, -106, -76};
#if defined(HAVE_WFOPEN)
#if BYTE_ORDER == LITTLE_ENDIAN
static const std::u16string UTF16_ENGLISH = {69, 110, 103, 108, 105, 115, 104};
static const std::u16string UTF16_KOREAN = {54620, 44397, 50612};
#else
static const std::u16string UTF16_ENGLISH = {23765, 28077, -19259};
static const std::u16string UTF16_KOREAN = {23765,  28077, -19259};
#endif
#endif

// HELPERS
// -------

template <typename IStream, typename OStream>
struct test_stream
{
    template <typename String>
    void standard(const String &path)
    {
        std::string expected = "Single line";

        OStream ofs1(path, ios_base::out);
        ofs1 << expected << endl;
        ofs1.close();

        IStream ifs1(path, ios_base::in);
        std::string result;
        std::getline(ifs1, result);
        ifs1.close();

        EXPECT_EQ(result, expected);
        EXPECT_TRUE(remove_file(path));
    }

    template <typename String>
    void move(const String &path)
    {
        std::string expected = "Single line";

        OStream ofs1(path, ios_base::out);
        OStream ofs2(std::move(ofs1));
        ofs2 << expected << endl;
        ofs2.close();

        IStream ifs1(path, ios_base::in);
        IStream ifs2(std::move(ifs1));
        std::string result;
        std::getline(ifs2, result);
        ifs2.close();

        EXPECT_EQ(result, expected);
        EXPECT_TRUE(remove_file(path));
    }

    template <typename String>
    void operator()(const String &path)
    {
        standard(path);
        move(path);
    }
};


// TESTS
// -----


TEST(sequential_fstream, sequential_fstream)
{
    using tester = test_stream<sequential_fstream, sequential_fstream>;

    tester()(UTF8_ENGLISH);
#if defined(HAVE_WFOPEN)         // WINDOWS
    tester()(UTF16_ENGLISH);
    tester()(UTF16_KOREAN);
#else                           // POSIX
    tester()(UTF8_KOREAN);
#endif
}


TEST(sequential_fstream, sequential_iofstream)
{
    using tester = test_stream<sequential_ifstream, sequential_ofstream>;

    tester()(UTF8_ENGLISH);
#if defined(HAVE_WFOPEN)         // WINDOWS
    tester()(UTF16_ENGLISH);
    tester()(UTF16_KOREAN);
#else                           // POSIX
    tester()(UTF8_KOREAN);
#endif
}

#include <warnings/pop.h>
