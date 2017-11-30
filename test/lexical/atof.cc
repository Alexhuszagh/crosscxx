//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.

#include <pycpp/lexical/atof.h>
#include <pycpp/lexical/format.h>
#include <gtest/gtest.h>
#include <cmath>

PYCPP_USING_NAMESPACE

// TESTS
// -----

TEST(atof32, base10)
{
    EXPECT_FLOAT_EQ(atof32("0", 10), 0);
    EXPECT_FLOAT_EQ(atof32("1", 10), 1);
    EXPECT_FLOAT_EQ(atof32("12", 10), 12);
    EXPECT_FLOAT_EQ(atof32("123", 10), 123);
    EXPECT_FLOAT_EQ(atof32("1234", 10), 1234);
    EXPECT_FLOAT_EQ(atof32("12345", 10), 12345);
    EXPECT_FLOAT_EQ(atof32("123456", 10), 123456);
    EXPECT_FLOAT_EQ(atof32("1234567", 10), 1234567);
    EXPECT_FLOAT_EQ(atof32("12345678", 10), 12345678);
    EXPECT_FLOAT_EQ(atof32("123456789", 10), 123456789);
    EXPECT_FLOAT_EQ(atof32("123456789.1", 10), 123456789.1);
    EXPECT_FLOAT_EQ(atof32("123456789.12", 10), 123456789.12);
    EXPECT_FLOAT_EQ(atof32("123456789.123", 10), 123456789.123);
    EXPECT_FLOAT_EQ(atof32("123456789.1234", 10), 123456789.1234);
    EXPECT_FLOAT_EQ(atof32("123456789.12345", 10), 123456789.12345);
    EXPECT_FLOAT_EQ(atof32("1.2345678912345e8", 10), 123456789.12345);
    EXPECT_FLOAT_EQ(atof32("1.2345e+8", 10), 123450000);
    EXPECT_FLOAT_EQ(atof32("1.2345e+11", 10), 1.2345e+11);
    EXPECT_FLOAT_EQ(atof32("123450000000", 10), 1.2345e+11);
    EXPECT_FLOAT_EQ(atof32("1.2345e+38", 10), 1.2345e+38);
    EXPECT_FLOAT_EQ(atof32("123450000000000000000000000000000000000", 10), 1.2345e+38);
    EXPECT_FLOAT_EQ(atof32("1.2345e-8", 10), 1.2345e-8);
    EXPECT_FLOAT_EQ(atof32("0.000000012345", 10), 1.2345e-8);
    EXPECT_FLOAT_EQ(atof32("1.2345e-38", 10), 1.2345e-38);
    EXPECT_FLOAT_EQ(atof32("0.000000000000000000000000000000000000012345", 10), 1.2345e-38);
    EXPECT_TRUE(std::isnan(atof32(NAN_STRING, 10)));
    EXPECT_TRUE(std::isinf(atof32(INFINITY_STRING, 10)));
    EXPECT_TRUE(std::isinf(atof32("-" + INFINITY_STRING, 10)));
}


TEST(atof32, basen)
{
    EXPECT_FLOAT_EQ(atof32("YA", 36), 1234.);
}


TEST(atof64, base10)
{
    EXPECT_DOUBLE_EQ(atof64("0", 10), 0);
    EXPECT_DOUBLE_EQ(atof64("1", 10), 1);
    EXPECT_DOUBLE_EQ(atof64("12", 10), 12);
    EXPECT_DOUBLE_EQ(atof64("123", 10), 123);
    EXPECT_DOUBLE_EQ(atof64("1234", 10), 1234);
    EXPECT_DOUBLE_EQ(atof64("12345", 10), 12345);
    EXPECT_DOUBLE_EQ(atof64("123456", 10), 123456);
    EXPECT_DOUBLE_EQ(atof64("1234567", 10), 1234567);
    EXPECT_DOUBLE_EQ(atof64("12345678", 10), 12345678);
    EXPECT_DOUBLE_EQ(atof64("123456789", 10), 123456789);
    EXPECT_DOUBLE_EQ(atof64("123456789.1", 10), 123456789.1);
    EXPECT_DOUBLE_EQ(atof64("123456789.12", 10), 123456789.12);
    EXPECT_DOUBLE_EQ(atof64("123456789.123", 10), 123456789.123);
    EXPECT_DOUBLE_EQ(atof64("123456789.1234", 10), 123456789.1234);
    EXPECT_DOUBLE_EQ(atof64("123456789.12345", 10), 123456789.12345);
    EXPECT_DOUBLE_EQ(atof64("1.2345678912345e8", 10), 123456789.12345);
    EXPECT_DOUBLE_EQ(atof64("1.2345e+8", 10), 123450000);
    EXPECT_DOUBLE_EQ(atof64("123450000000", 10), 1.2345e+11);
    EXPECT_DOUBLE_EQ(atof64("1.2345e+11", 10), 1.2345e+11);
    EXPECT_DOUBLE_EQ(atof64("1.2345e+38", 10), 1.2345e+38);
    EXPECT_DOUBLE_EQ(atof64("123450000000000000000000000000000000000", 10), 1.2345e+38);
    EXPECT_DOUBLE_EQ(atof64("1.2345e+308", 10), 1.2345e+308);
    EXPECT_DOUBLE_EQ(atof64("123450000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000", 10), 1.2345e+308);
    EXPECT_DOUBLE_EQ(atof64("1.2345e-8", 10), 0.000000012345);
    EXPECT_DOUBLE_EQ(atof64("0.000000012345", 10), 1.2345e-8);
    EXPECT_DOUBLE_EQ(atof64("1.2345e-38", 10), 1.2345e-38);
    EXPECT_DOUBLE_EQ(atof64("0.000000000000000000000000000000000000012345", 10), 1.2345e-38);
    EXPECT_DOUBLE_EQ(atof64("1.2345e-308", 10), 1.2345e-308);
    // due to issues in how the data is parsed, manually extracting
    // non-exponents of 1.<e-299 is prone to error
    // test the limit of our ability
    EXPECT_DOUBLE_EQ(atof64("0.000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000012345", 10), 1.2345e-299);
    EXPECT_TRUE(std::isnan(atof64(NAN_STRING, 10)));
    EXPECT_TRUE(std::isinf(atof64(INFINITY_STRING, 10)));
    EXPECT_TRUE(std::isinf(atof64("-" + INFINITY_STRING, 10)));
}


TEST(atof64, basen)
{
    EXPECT_FLOAT_EQ(atof64("YA", 36), 1234.);
}
