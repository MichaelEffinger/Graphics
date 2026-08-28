#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

import ES_math;

TEMPLATE_TEST_CASE("ES::math::abs matches std::abs", "[abs]", float, double, long double) {
    using T = TestType;
    using NL = std::numeric_limits<T>;

    SECTION("ordinary positive and negative values") {
        REQUIRE(ES::math::abs(static_cast<T>(3.5)) == std::abs(static_cast<T>(3.5)));
        REQUIRE(ES::math::abs(static_cast<T>(-3.5)) == std::abs(static_cast<T>(-3.5)));
        REQUIRE(ES::math::abs(static_cast<T>(0.1)) == std::abs(static_cast<T>(0.1)));
    }

    SECTION("negative zero collapses to positive zero, matching std::abs") {
        T negZero = -static_cast<T>(0);
        T mine = ES::math::abs(negZero);
        T theirs = std::abs(negZero);

        REQUIRE(std::signbit(mine) == false);
        REQUIRE(std::signbit(theirs) == false);
        REQUIRE(mine == theirs);
    }

    SECTION("positive zero stays positive zero") {
        REQUIRE(std::signbit(ES::math::abs(static_cast<T>(0))) == false);
    }

    SECTION("infinity") {
        REQUIRE(ES::math::abs(NL::infinity()) == std::abs(NL::infinity()));
        REQUIRE(ES::math::abs(-NL::infinity()) == std::abs(NL::infinity())); // both +inf
    }

    SECTION("NaN propagates (NaN != NaN, so check isnan on both)") {
        REQUIRE(std::isnan(ES::math::abs(NL::quiet_NaN())));
        REQUIRE(std::isnan(std::abs(NL::quiet_NaN())));
    }
}

TEMPLATE_TEST_CASE("ES::math::abs matches std::abs for integral types", "[abs]", int, long, long long) {
    using T = TestType;

    REQUIRE(ES::math::abs(static_cast<T>(7)) == std::abs(static_cast<T>(7)));
    REQUIRE(ES::math::abs(static_cast<T>(-7)) == std::abs(static_cast<T>(-7)));
    REQUIRE(ES::math::abs(static_cast<T>(0)) == std::abs(static_cast<T>(0)));
    // INT_MIN deliberately excluded: UB on both sides, nothing to compare.
}

TEMPLATE_TEST_CASE("ES::math::round matches std::round", "[round]", float, double, long double) {
    using T = TestType;
    using NL = std::numeric_limits<T>;

    SECTION("half-away-from-zero on ties") {
        REQUIRE(ES::math::round(static_cast<T>(0.5)) == std::round(static_cast<T>(0.5)));
        REQUIRE(ES::math::round(static_cast<T>(-0.5)) == std::round(static_cast<T>(-0.5)));
        REQUIRE(ES::math::round(static_cast<T>(2.5)) == std::round(static_cast<T>(2.5)));
        REQUIRE(ES::math::round(static_cast<T>(-2.5)) == std::round(static_cast<T>(-2.5)));
    }

    SECTION("ordinary non-tie values") {
        REQUIRE(ES::math::round(static_cast<T>(2.3)) == std::round(static_cast<T>(2.3)));
        REQUIRE(ES::math::round(static_cast<T>(2.7)) == std::round(static_cast<T>(2.7)));
        REQUIRE(ES::math::round(static_cast<T>(-2.3)) == std::round(static_cast<T>(-2.3)));
        REQUIRE(ES::math::round(static_cast<T>(-2.7)) == std::round(static_cast<T>(-2.7)));
    }

    SECTION("signed zero is preserved on both sides") {
        T negZero = -static_cast<T>(0);
        REQUIRE(std::signbit(ES::math::round(negZero)) == std::signbit(std::round(negZero)));
        REQUIRE(std::signbit(ES::math::round(static_cast<T>(0))) == std::signbit(std::round(static_cast<T>(0))));
    }

    SECTION("infinity passes through unchanged") {
        REQUIRE(ES::math::round(NL::infinity()) == std::round(NL::infinity()));
        REQUIRE(ES::math::round(-NL::infinity()) == std::round(-NL::infinity()));
    }

    SECTION("NaN propagates") {
        REQUIRE(std::isnan(ES::math::round(NL::quiet_NaN())));
        REQUIRE(std::isnan(std::round(NL::quiet_NaN())));
    }

    SECTION("magnitudes beyond mantissa precision are already integral, no-ops on both") {
        T big = static_cast<T>(1) / NL::epsilon(); // exactly the threshold your implementation bails at
        REQUIRE(ES::math::round(big) == std::round(big));
        REQUIRE(ES::math::round(big * 2) == std::round(big * 2));
    }
}

TEMPLATE_TEST_CASE("ES::math::round matches std::round for integral types (identity)", "[round]", int, long, long long) {
    using T = TestType;
    REQUIRE(ES::math::round(static_cast<T>(42)) == static_cast<T>(42));
    REQUIRE(ES::math::round(static_cast<T>(-42)) == static_cast<T>(-42));
}