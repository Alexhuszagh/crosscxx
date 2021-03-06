//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief string_view unittests.
 */

#include <pycpp/stl/string_view.h>
#include <pycpp/stl/utility.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// DATA
// ----

static const std::string STR = {0, 84, 104, 105, 115, 32, 105, 115, 32, 100, 97, 116, 97, 10};
static const std::string NONNULL = {84, 104, 105, 115, 32, 105, 115, 32, 100, 97, 116, 97, 10};

// HELPERS
// -------

template <typename T, template <typename> class Hash>
static void test_hash()
{
    using result_type = decltype(Hash<T>()(declval<T>()));
    static_assert(is_same<result_type, size_t>::value, "");
}

// TESTS
// -----


TEST(string_view, constructors)
{
    string_view str(STR);
    string_view other;
    EXPECT_EQ(str.size(), 14);

    str = string_view(STR.data());
    EXPECT_EQ(str.size(), 0);

    str = string_view(STR.data(), STR.size());
    EXPECT_EQ(str.size(), 14);
}


TEST(string_view, swap)
{
    string_view str(STR);
    string_view other;
    EXPECT_EQ(str.size(), 14);
    EXPECT_EQ(other.size(), 0);

    swap(str, other);
    EXPECT_EQ(str.size(), 0);
    EXPECT_EQ(other.size(), 14);

    str.swap(other);
    EXPECT_EQ(str.size(), 14);
    EXPECT_EQ(other.size(), 0);
}


TEST(string_view, relational)
{
    string_view str(STR);
    string_view other;

    EXPECT_TRUE(str == str);
    EXPECT_FALSE(str == other);
    EXPECT_FALSE(str != str);
    EXPECT_TRUE(str != other);
    EXPECT_FALSE(str < str);
    EXPECT_TRUE(str <= str);
    EXPECT_FALSE(str > str);
    EXPECT_TRUE(str >= str);
}


TEST(string_view, iterator)
{
    string_view str(STR);
    std::string reversed(STR.rbegin(), STR.rend());
    EXPECT_EQ(std::string(str.begin(), str.end()), STR);
    EXPECT_EQ(std::string(str.rbegin(), str.rend()), reversed);
}


TEST(string_view, capacity)
{
    string_view str(STR);
    string_view other;

    EXPECT_EQ(str.size(), 14);
    EXPECT_EQ(other.size(), 0);

    EXPECT_EQ(str.length(), 14);
    EXPECT_EQ(other.length(), 0);

    EXPECT_FALSE(str.empty());
    EXPECT_TRUE(other.empty());
}


TEST(string_view, element)
{
    string_view str(STR);

    EXPECT_EQ(str[0], '\0');
    EXPECT_EQ(str.at(0), '\0');
    EXPECT_EQ(str[1], 'T');
    EXPECT_EQ(str.at(1), 'T');
    EXPECT_EQ(str.front(), '\0');
    EXPECT_EQ(str.back(), '\n');
}


TEST(string_view, operations)
{
    string_view str(STR);
    string_view null;
    string_view non_null(NONNULL);

    // find
    EXPECT_EQ(str.find(string_view("is")), 3);
    EXPECT_EQ(str.find("is"), 3);
    EXPECT_EQ(str.find(std::string("is")), 3);
    EXPECT_EQ(str.find("is", 0, 2), 3);
    EXPECT_EQ(str.find('i'), 3);

    // find_first_of
    EXPECT_EQ(str.find_first_of(string_view("hsi")), 2);
    EXPECT_EQ(str.find_first_of("hsi"), 2);
    EXPECT_EQ(str.find_first_of(std::string("hsi")), 2);
    EXPECT_EQ(str.find_first_of("hsi", 0, 3), 2);
    EXPECT_EQ(str.find_first_of('h'), 2);

    EXPECT_EQ(str.find_first_of(string_view("x")), string_view::npos);
    EXPECT_EQ(str.find_first_of("x"), string_view::npos);
    EXPECT_EQ(str.find_first_of(std::string("x")), string_view::npos);
    EXPECT_EQ(str.find_first_of("x", 0, 1), string_view::npos);
    EXPECT_EQ(str.find_first_of('x'), string_view::npos);

    // find_first_not_of
    EXPECT_EQ(non_null.find_first_not_of(string_view("Tish")), 4);
    EXPECT_EQ(non_null.find_first_not_of("Tish"), 4);
    EXPECT_EQ(non_null.find_first_not_of(std::string("Tish")), 4);
    EXPECT_EQ(non_null.find_first_not_of("Tish", 0, 4), 4);
    EXPECT_EQ(non_null.find_first_not_of('T'), 1);

    EXPECT_EQ(non_null.find_first_not_of(string_view("Thisdta \n")), string_view::npos);
    EXPECT_EQ(non_null.find_first_not_of("Thisdta \n"), string_view::npos);
    EXPECT_EQ(non_null.find_first_not_of(std::string("Thisdta \n")), string_view::npos);
    EXPECT_EQ(non_null.find_first_not_of("Thisdta \n", 0, 9), string_view::npos);

    // rfind
    EXPECT_EQ(str.rfind(string_view("is")), 6);
    EXPECT_EQ(str.rfind("is"), 6);
    EXPECT_EQ(str.rfind(std::string("is")), 6);
    EXPECT_EQ(str.rfind("is", 0, 2), string_view::npos);
    EXPECT_EQ(str.rfind('i'), 6);

    EXPECT_EQ(str.rfind(string_view("isx")), string_view::npos);
    EXPECT_EQ(str.rfind("isx"), string_view::npos);
    EXPECT_EQ(str.rfind(std::string("isx")), string_view::npos);
    EXPECT_EQ(str.rfind("isx", 0, 3), string_view::npos);
    EXPECT_EQ(str.rfind('x'), string_view::npos);

    EXPECT_EQ(null.rfind('i'), string_view::npos);

    // find_last_of
    EXPECT_EQ(str.find_last_of(string_view("hsi")), 7);
    EXPECT_EQ(str.find_last_of("hsi"), 7);
    EXPECT_EQ(str.find_last_of(std::string("hsi")), 7);
    EXPECT_EQ(str.find_last_of("hsi", 0, 3), string_view::npos);
    EXPECT_EQ(str.find_last_of('h'), 2);

    EXPECT_EQ(str.find_last_of(string_view("x")), string_view::npos);
    EXPECT_EQ(str.find_last_of("x"), string_view::npos);
    EXPECT_EQ(str.find_last_of(std::string("x")), string_view::npos);
    EXPECT_EQ(str.find_last_of("x", 0, 1), string_view::npos);
    EXPECT_EQ(str.find_last_of('x'), string_view::npos);

    // find_last_not_of
    EXPECT_EQ(non_null.find_last_not_of(string_view("dat\n")), 7);
    EXPECT_EQ(non_null.find_last_not_of("dat\n"), 7);
    EXPECT_EQ(non_null.find_last_not_of(std::string("dat\n")), 7);
    EXPECT_EQ(non_null.find_last_not_of("dat\n", 0, 4), 0);
    EXPECT_EQ(non_null.find_last_not_of('\n'), 11);

    EXPECT_EQ(non_null.find_last_not_of(string_view("Thisdta \n")), string_view::npos);
    EXPECT_EQ(non_null.find_last_not_of("Thisdta \n"), string_view::npos);
    EXPECT_EQ(non_null.find_last_not_of(std::string("Thisdta \n")), string_view::npos);
    EXPECT_EQ(non_null.find_last_not_of("Thisdta \n", 0, 9), string_view::npos);
}


TEST(string_view, conversions)
{
    string_view str(STR);
    string_view other;

    EXPECT_EQ(std::string(str), STR);
    EXPECT_EQ(std::string(other), "");
}


TEST(string_view, hash)
{
    // std::hash
    test_hash<string_view, std::hash>();
    test_hash<wstring_view, std::hash>();
    test_hash<u16string_view, std::hash>();
    test_hash<u32string_view, std::hash>();

    // hash
    test_hash<string_view, hash>();
    test_hash<wstring_view, hash>();
    test_hash<u16string_view, hash>();
    test_hash<u32string_view, hash>();
}
