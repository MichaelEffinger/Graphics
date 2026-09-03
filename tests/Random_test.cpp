#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/generators/catch_generators_range.hpp>
#include "ES_test_util.hpp"
#include <forward_list>
#include <numeric>
#include <ranges>
import ES_easy;

// TEST_CASE("ES::easy::random correct period", "[Random][easy::random][Unseeded Randomness]") {
//     SECTION("Ensure that the period is correct") {
//         auto z = ES::easy::random<std::uint32_t>(0, UINT_MAX);
//         static constexpr unsigned PERIOD{std::minstd_rand::modulus}; // 2^31-1 is the period of minstd_rand
//         for (unsigned y = 0; y < PERIOD; ++y) void (ES::easy::random<std::uint32_t>(0, UINT_MAX));
//         REQUIRE(z == ES::easy::random<std::uint32_t>(0, UINT_MAX));
//     }
// }

TEST_CASE("ES::Random::_easy_seeded_callable (All overloads)", "[Random][easy::random][Seeded Randomness]") {
    SECTION("check if two callables with the same seed give the same value for the first overload (canonical)") {
        unsigned i = GENERATE(range(0u, ENOUGH_ITERATIONS));
        auto first = ES::easy::random_seeded_callable(i),
                second = ES::easy::random_seeded_callable(i);
        CHECK(first() == second());
    }
    SECTION("check if two callables with the same seed give the same value for the second overload (0 to N)") {
        unsigned i = GENERATE(range(0u, ENOUGH_ITERATIONS));
        auto first = ES::easy::random_seeded_callable(i, 10),
                second = ES::easy::random_seeded_callable(i, 10);
        CHECK(first() == second());
    }
    SECTION("check if two callables with the same seed give the same value for the second overload (M to N)") {
        unsigned i = GENERATE(range(0u, ENOUGH_ITERATIONS));
        auto first = ES::easy::random_seeded_callable(i, 10, 20),
                second = ES::easy::random_seeded_callable(i, 10, 20);
        CHECK(first() == second());
    }
}

TEST_CASE("ES::easy::shuffle","[ES::easy::shuffle]") {
    std::vector arr{1,2,3,4,5,6,7,8,9,0}, arr2(arr);
    ES::easy::shuffle(arr);
    CHECK(arr != arr2);
}


TEST_CASE("ES::easy::raffle", "[ES::easy::raffle]") {
    std::forward_list<char> nasty(26);
    std::ranges::iota(nasty, 'a');

    std::vector<char> random_man(26);

    for (auto& e : random_man) e = ES::easy::raffle(nasty);

    CHECK_FALSE(random_man == (nasty | std::ranges::to<std::vector>()));
}