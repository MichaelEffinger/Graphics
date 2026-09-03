#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
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