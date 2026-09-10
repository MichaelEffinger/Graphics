#include <algorithm>
#include <deque>
#include <numeric>
#include <array>
#include <unordered_map>
#include <forward_list>
#include <ranges>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/generators/catch_generators_range.hpp>
#include "ES_test_util.hpp"

import ES_easy;



TEST_CASE("ES capitalizing and lowercasing ranges") {

    SECTION("Capitalize_range") {
        std::string str = "aaa", st2 = ES::easy::capitalize_range(str);
        CHECK("AAA" == st2);

        CHECK(ES::easy::capitalize_range_in_place(str) == st2);
    }

    SECTION("Lowercase range!"){
        std::string stemp{"Long string that would have to call new."}, st4{stemp}, st3{ES::easy::lowercase_range(std::move(stemp))};
        ES::easy::lowercase_range_in_place(st4);
        CHECK(st4 == st3);
    }

    SECTION("Empty!?") {
        std::string empty;
        CHECK(ES::easy::lowercase_range(empty).empty());
        CHECK(ES::easy::capitalize_range(empty).empty());
    }

    SECTION("Not neccessairly a string!?") {
        std::deque<char> deck;
        std::array<char, 26> alpha{};
        std::ranges::iota(alpha, 'A');
        ES::easy::lowercase_range_in_place(alpha);
        CHECK_FALSE(std::ranges::equal(alpha, ES::easy::capitalize_range(alpha)));
        std::ranges::copy(alpha, std::back_inserter(deck));
        CHECK(std::ranges::equal(ES::easy::capitalize_range_in_place(alpha), ES::easy::capitalize_range_in_place(deck)));
    }

}

TEST_CASE("ES::easy::min_max","[ES::easy::min_max][minmax][min_max]") {
    std::vector<std::byte> empty;
    CHECK_THROWS_AS(ES::easy::min_max(empty), std::out_of_range);
}


TEST_CASE("ES::easy::trim_whitespace","[trim_whitespace]") {

    SECTION("Left, right, and middle full of white space."){
        std::string miserable{"\t\t\t\t\t\t\tI'm ugly!!11    \t 111 :((((\n\t"};

        CHECK(ES::easy::trim_whitespace(miserable) == "I'm ugly!!11    \t 111 :((((");
    }

    SECTION("Empty string") {
        std::string empty;
        CHECK(ES::easy::trim_whitespace_in_place(empty).empty());
    }

    SECTION("Left side with whitespace") {
        std::string str{"             \v\t\t                  wow!"};
        CHECK(ES::easy::trim_whitespace(str) == "wow!");
    }

    SECTION("Right side with whitespace") {
        std::string str{"neat!        \t\t\t\v\t\n\n                  \t\n"};
        CHECK(ES::easy::trim_whitespace_in_place(str) == "neat!");
    }

    SECTION("Complex left side with whitespace") {
        std::string str{"    q         \v\t\t          r        wow!"};
        CHECK(ES::easy::trim_whitespace(str) == "q         \v\t\t          r        wow!");
    }

    SECTION("Complex right side with whitespace") {
        std::string str{"neat!    e    \t\t\t\v\t\n\n         w         \t\n"};
        CHECK(ES::easy::trim_whitespace_in_place(str) == "neat!    e    \t\t\t\v\t\n\n         w");
    }

}

TEST_CASE("ES::easy::where_am_I, Where in the world...","[ES::easy::where_am_I][filesystem][diskio]") {
    auto const you_give_love_a_bad_name = ES::easy::where_am_I().string();

    CHECK(you_give_love_a_bad_name.contains("cmake"));

}

TEST_CASE("ES::easy::cyclical_iterator on std::forward_list", "[cyclical_iterator][forward]") {
    using ES::easy::cyclical_iterator;
    std::forward_list<int> data{10, 20, 30, 40};
    auto b = data.begin();
    auto e = data.end();

    SECTION("dereference at construction yields the first element") {
        cyclical_iterator it(b, b, e);
        REQUIRE(*it == 10);
    }

    SECTION("single increments walk the underlying sequence in order") {
        cyclical_iterator it(b, b, e);
        REQUIRE(*it == 10);
        ++it;
        REQUIRE(*it == 20);
        ++it;
        REQUIRE(*it == 30);
        ++it;
        REQUIRE(*it == 40);
    }

    SECTION("incrementing past the last element wraps to the first") {
        cyclical_iterator it(b, b, e);
        std::advance(it, 4); // exactly one full lap
        REQUIRE(*it == 10);
    }

    SECTION("multiple full laps return to the same logical position") {
        cyclical_iterator it(b, b, e);
        std::advance(it, 4 * 3); // three full laps
        REQUIRE(*it == 10);
        ++it; ++it;
        REQUIRE(*it == 30);
    }

    SECTION("post-increment returns the pre-increment value and still advances") {
        cyclical_iterator it(b, b, e);
        auto prev = it++;
        REQUIRE(*prev == 10);
        REQUIRE(*it == 20);
    }

    SECTION("post-increment wraps correctly at the boundary") {
        cyclical_iterator it(b, b, e);
        std::advance(it, 3); // now at 40, the last element
        auto prev = it++;
        REQUIRE(*prev == 40);
        REQUIRE(*it == 10);
    }

    SECTION("two iterators at the same logical position compare equal") {
        cyclical_iterator it1(b, b, e);
        cyclical_iterator it2(b, b, e);
        std::advance(it1, 4); // full lap back to start
        REQUIRE(it1 == it2);
    }

    SECTION("two iterators at different logical positions compare unequal") {
        cyclical_iterator it1(b, b, e);
        cyclical_iterator it2(b, b, e);
        ++it2;
        REQUIRE_FALSE(it1 == it2);
    }

    SECTION("std::advance works across one or more wraparounds") {
        cyclical_iterator it(b, b, e);
        std::advance(it, 4 + 2); // one full lap plus two more steps
        REQUIRE(*it == 30);
    }

    SECTION("default-constructed iterators of the same type compare equal") {
        using It = cyclical_iterator<decltype(b), decltype(b), decltype(e)>;
        It it1{};
        It it2{};
        REQUIRE(it1 == it2);
    }

    SECTION("type satisfies forward_iterator but not bidirectional_iterator") {
        using It = cyclical_iterator<decltype(b), decltype(b), decltype(e)>;
        STATIC_REQUIRE(std::forward_iterator<It>);
        STATIC_REQUIRE_FALSE(std::bidirectional_iterator<It>);
    }
}

TEST_CASE("ES::easy::cyclical_iterator both ways!?", "[cyclical_iterator][bidirectional]") {
    using ES::easy::cyclical_iterator;
    std::list<int> data{10, 20, 30, 40};
    auto b = data.begin();
    auto e = data.end();

    SECTION("single decrement from the first element wraps to the last") {
        cyclical_iterator it(b, b, e);
        --it;
        REQUIRE(*it == 40);
    }

    SECTION("decrementing walks the underlying sequence backward") {
        cyclical_iterator it(b, b, e);
        std::advance(it, 3); // now at 40
        --it;
        REQUIRE(*it == 30);
        --it;
        REQUIRE(*it == 20);
        --it;
        REQUIRE(*it == 10);
    }

    SECTION("decrementing past the first element wraps repeatedly through a full lap") {
        cyclical_iterator it(b, b, e);
        --it; --it; --it; --it; // one full lap backward
        REQUIRE(*it == 10);
    }

    SECTION("multiple full backward laps return to the same logical position") {
        cyclical_iterator it(b, b, e);
        std::advance(it, -(4 * 3)); // three full laps backward
        REQUIRE(*it == 10);
    }

    SECTION("post-decrement returns the pre-decrement value and still moves back") {
        cyclical_iterator it(b, b, e);
        auto prev = it--;
        REQUIRE(*prev == 10);
        REQUIRE(*it == 40);
    }

    SECTION("post-decrement wraps correctly across the boundary") {
        cyclical_iterator it(b, b, e);
        std::advance(it, 1); // now at 20
        --it;                // back to 10
        auto prev = it--;
        REQUIRE(*prev == 10);
        REQUIRE(*it == 40);
    }

    SECTION("n steps forward followed by n steps back return to the start") {
        cyclical_iterator it(b, b, e);
        cyclical_iterator start(b, b, e);
        for (int n = 0; n < 10; ++n) ++it;
        for (int n = 0; n < 10; ++n) --it;
        REQUIRE(it == start);
    }

    SECTION("a full forward lap and a full backward lap are mutual inverses") {
        cyclical_iterator it(b, b, e);
        ++it; ++it; ++it; ++it; // one full forward lap
        REQUIRE(*it == 10);
        --it; --it; --it; --it; // one full backward lap
        REQUIRE(*it == 10);
    }

    SECTION("std::advance accepts negative distances across wraparound") {
        cyclical_iterator it(b, b, e);
        std::advance(it, -1);
        REQUIRE(*it == 40);
        std::advance(it, -5); // one full lap plus one more step back
        REQUIRE(*it == 30);
    }

    SECTION("two iterators reached via different paths to the same element compare equal") {
        cyclical_iterator it1(b, b, e);
        std::advance(it1, 6);  // 6 forward, wraps once, lands on 30
        cyclical_iterator it2(b, b, e);
        std::advance(it2, -2); // 2 back from start, wraps once, lands on 30
        REQUIRE(it1 == it2);
    }

    SECTION("type satisfies bidirectional_iterator") {
        using It = cyclical_iterator<decltype(b), decltype(b), decltype(e)>;
        STATIC_REQUIRE(std::bidirectional_iterator<It>);
    }
}