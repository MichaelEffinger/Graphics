#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/generators/catch_generators_range.hpp>
#include <filesystem>
#include <iostream>

import ES_flyweight;

TEST_CASE("ES::flyweight dummy") {
    struct unsigned_database : ES::flyweight<unsigned>{};

    unsigned y = 42u;

    unsigned_database::store("thirty-three", 33u);

    auto z = unsigned_database::request("thirty-three");
    CHECK(z.lock());

    unsigned_database::store("fourty-two", y);

    z = unsigned_database::request("fourty-two");
    CHECK(z.lock());
}