#include <catch2/catch_test_macros.hpp>
#include <catch2/interfaces/catch_interfaces_config.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/generators/catch_generators_range.hpp>
#include "ES_test_util.hpp"
#include <forward_list>
#include <numeric>
#include <ranges>
#include <random>
import ES_easy;


// #pragma optimize("t", on)
// [[gnu::optimize("O3")]]
// TEST_CASE("ES::easy::random correct period", "[Random][easy::random][Unseeded Randomness]") {
//     SECTION("Ensure that the period is correct") {
//         auto z = ES::easy::random<std::uint32_t>(0, UINT_MAX);
//         static constexpr unsigned PERIOD{std::minstd_rand::modulus}; // 2^31-1 is the period of minstd_rand
//         for (unsigned y = 0; y < PERIOD; ++y) void (ES::easy::random<std::uint32_t>(0, UINT_MAX));
//         REQUIRE(z == ES::easy::random<std::uint32_t>(0, UINT_MAX));
//     }
// }
// #pragma optimize("", on)


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

TEST_CASE("ES::easy::random(float)","[random]") {
    auto i = GENERATE(range(0u, ENOUGH_ITERATIONS));
    auto first = ES::easy::random_seeded_callable(i),
    second = ES::easy::random_seeded_callable(i);
    CHECK(first() == second());
    CHECK(first() < 1.0);
    CHECK(second() < 1.0);
}

TEST_CASE("ES::easy::random tied to Catch2's seed!","[random]") {
    unsigned i = GENERATE(range(0u, ENOUGH_ITERATIONS));
    const unsigned s = Catch::getCurrentContext().getConfig()->rngSeed();
    auto two_args = ES::easy::random_seeded_callable(s, 0u, i);
    auto one_args = ES::easy::random_seeded_callable(s,  i);
    CHECK(two_args() == one_args());
}

TEST_CASE("ES::easy::shuffle","[ES::easy::shuffle]") {
    SECTION("Basic test!"){
        std::vector arr{1,2,3,4,5,6,7,8,9,0}, arr2(arr);
        ES::easy::shuffle(arr);
        CHECK(arr != arr2);
    }

    SECTION("Empty!") {
        std::string x;
        CHECK_NOTHROW(ES::easy::shuffle(x));
    }
}


TEST_CASE("ES::easy::raffle", "[ES::easy::raffle]") {
    SECTION("Alphabet soup!"){
        std::forward_list<char> nasty(26);
        std::ranges::iota(nasty, 'a');

        std::vector<char> random_man(26);

        for (auto& e : random_man) e = ES::easy::raffle(nasty);

        CHECK_FALSE(random_man == (nasty | std::ranges::to<std::vector>()));
    }

    SECTION("Empty!") {
        std::vector<int> zed{'2'};
        zed.clear();
        CHECK_THROWS_AS(ES::easy::raffle(zed), std::out_of_range);
    }
}