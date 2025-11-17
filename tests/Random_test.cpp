#include <catch2/catch_test_macros.hpp>
#include "../ES_random.hpp"
#include "ES_test_util.hpp"

TEST_CASE("ES::Random::Easy correct period", "[Random][Random::Easy][Unseeded Randomness]") {
    SECTION("Ensure that the period is correct") {
        auto z = ES::random::easy<std::uint32_t>(0, UINT_MAX);
        static constexpr unsigned PERIOD{std::minstd_rand::modulus}; // 2^31-1 is the period of minstd_rand
        for (unsigned y = 0; y < PERIOD; ++y) void (ES::random::easy<std::uint32_t>(0, UINT_MAX));
        REQUIRE(z == ES::random::easy<std::uint32_t>(0, UINT_MAX));
    }
}

TEST_CASE("ES::Random::_easy_seeded_callable (All overloads)", "[Random][Random::Easy][Seeded Randomness]") {
    SECTION("check if two callables with the same seed give the same value for the first overload (canonical)") {
        auto first = ES::random::easy_seeded_callable(255),
                second = ES::random::easy_seeded_callable(255);
        for (unsigned i = 0; i < ENOUGH_ITERATIONS; ++i) REQUIRE(first() == second());
    }
    SECTION("check if two callables with the same seed give the same value for the second overload (0 to N)") {
        auto first = ES::random::easy_seeded_callable(255, 10),
                second = ES::random::easy_seeded_callable(255, 10);
        for (unsigned i = 0; i < ENOUGH_ITERATIONS; ++i) REQUIRE(first() == second());
    }
    SECTION("check if two callables with the same seed give the same value for the second overload (M to N)") {
        auto first = ES::random::easy_seeded_callable(255, 10, 20),
                second = ES::random::easy_seeded_callable(255, 10,20);
        for (unsigned i = 0; i < ENOUGH_ITERATIONS; ++i) REQUIRE(first() == second());
    }
}


