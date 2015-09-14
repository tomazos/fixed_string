#include "core/fixed_string.h"

#include "gtest/gtest.h"

using std::experimental::fixed_string;
using std::experimental::make_fixed_string;

constexpr auto s1 = make_fixed_string("foo");

STATIC_ASSERT(s1 == "foo");
STATIC_ASSERT("bar" < s1);

constexpr auto s2 = s1 + "bar";

STATIC_ASSERT(s2 == "foobar");

constexpr auto s3 = make_fixed_string("123");
constexpr auto s4 = make_fixed_string("456");
constexpr auto s5 = s3 + s4;
STATIC_ASSERT(s5 == "123456");
constexpr int x = stoi(s5);
STATIC_ASSERT(x == 123456);
