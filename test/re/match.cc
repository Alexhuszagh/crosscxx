//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Regular expression match object unittests.
 */

#include <pycpp/re/regex.h>
#include <pycpp/stl/deque.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(re, match_iterator_t)
{
    string data = "These are a bunch of words";
    regexp_t regex("\\w+");
    match_iterator_t first(regex, data);
    match_iterator_t last;

    deque<string> actual;
    deque<string> expected = {
        "These",
        "are",
        "a",
        "bunch",
        "of",
        "words",
    };
    EXPECT_NE(first, last);
    for (; first != last; ++first) {
        actual.emplace_back(string(first->group(0)));
    }
    EXPECT_EQ(actual, expected);
}
