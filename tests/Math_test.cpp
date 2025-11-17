#include <catch2/catch_test_macros.hpp>

#include "ES_test_util.hpp"
#include "../ES_math.hpp"

constexpr float FLIMIT = 1 << 23;
constexpr double DLIMIT = 1LL << 52;


TEST_CASE("Floortastic","[Math][Floor][Floating-point]") {
    CHECK(ES::math::floor(3.14f) == 3.f);
    CHECK(ES::math::floor(FLIMIT + 3) == FLIMIT + 3);

}