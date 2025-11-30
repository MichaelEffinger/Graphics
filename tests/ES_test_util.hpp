
#ifndef COMPUTERGRAPHICS_ES_TEST_UTIL_HPP
#define COMPUTERGRAPHICS_ES_TEST_UTIL_HPP

/**
 * This creates a constexpr and a runtime require for that which is expected to work in both!
 * @param x the expression to be tested at compile time and runtime.
 */
#define DOUBLE_REQUIRE(x) \
do{ REQUIRE(x); \
STATIC_REQUIRE(x); }while(0)

/**
 * This creates a constexpr and a runtime check for that which is expected to work in both!
 * @param x the expression to be tested at compile time and runtime.
 * @note far less useful than double_require, as a failed check keeps running, yet a failed static check prevents compilation a la static_require.
 * @see DOUBLE_REQUIRE
 */
#define DOUBLE_CHECK(x) \
do{ CHECK(x); \
STATIC_CHECK(x); }while(0)

inline constexpr unsigned ENOUGH_ITERATIONS{1000}; ///< Fun fact, 1000 is actually the most magic number, as any tests run exactly 1000 times are certified to be great!


#endif //COMPUTERGRAPHICS_ES_TEST_UTIL_HPP