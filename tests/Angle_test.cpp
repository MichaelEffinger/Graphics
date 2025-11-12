#include "../ES_angle.hpp"
#include <catch2/catch_test_macros.hpp>


constexpr float PIE = std::numbers::pi_v<float>; //TODO: use the promised in-house ES::math pi.
constexpr float PINF = std::numeric_limits<float>::infinity();

//This creates a constexpr and a runtime test for that which is expected to work in both!
#define DOUBLE_REQUIRE(x) \
    do{ REQUIRE(x); \
    STATIC_REQUIRE(x); }while(0)

using namespace ES::math::angle_literals;

TEST_CASE("Angle!","[Angle]") {
    SECTION("Addition") {
        DOUBLE_REQUIRE(ES::angle_deg{90.f} + ES::angle_rad{PIE/2} == 180_deg);
    }
    SECTION("Subtraction") {
        DOUBLE_REQUIRE((ES::angle_rad{-200.f} -= ES::angle_deg{3000.f}) < ES::angle_rad{-200.f});
    }
    SECTION("Multiplication") {
        DOUBLE_REQUIRE(ES::angle_rad{10} * 1 == ES::angle_rad{10});
        DOUBLE_REQUIRE(ES::angle_rad{10} * 2 == ES::angle_rad{20});
    }
    SECTION("Division (ratio)") {
        //ES::angle_deg{90} / ES::angle_rad{2*PIE} == ES::angle_deg{360};
        DOUBLE_REQUIRE(ES::angle_deg{90.f} / ES::angle_rad{2.f*PIE} == ES::angle_deg{90.f/360.f});
    }
}