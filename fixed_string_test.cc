#include "fixed_string.h"

using std::experimental::fixed_string;
using std::experimental::make_fixed_string;

constexpr auto test_string_1 = make_fixed_string("foo");

static_assert(test_string_1 == "foo", "");

int main() {}
