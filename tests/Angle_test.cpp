#include "../ES_angle.hpp"
#include <catch2/catch_test_macros.hpp>
#include "ES_test_util.hpp"


constexpr float PIE = std::numbers::pi_v<float>; //TODO: use the promised in-house ES::math pi.
constexpr float PINF = std::numeric_limits<float>::infinity();



using namespace ES::math::angle_literals;

TEST_CASE("Angle!","[Angle]") {
    SECTION("Trivial equality") {
        DOUBLE_REQUIRE(ES::angle_deg{90.f} == ES::angle_rad{PIE/2.f});
    }
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
        ES::angle_deg{90} / ES::angle_rad{2*PIE} == ES::angle_deg{360};
        DOUBLE_REQUIRE(ES::angle_deg{90.f} / ES::angle_rad{2.f*PIE} == ES::angle_deg{90.f/360.f});
    }
    SECTION("Coterminals!") {
        DOUBLE_REQUIRE(ES::angle_deg{360e10f}.normalize() == ES::angle_rad{0.f});
    }
}

TEST_CASE("template sanity crisis!","[Angle]") {
    ES::angle_deg zed{ES::angle_rad{180.f}};
    STATIC_REQUIRE( std::is_same_v<
        decltype(zed)::value_type, float
    >
    );

    auto rad = ES::angle<ES::Secret::radian, float>(ES::angle<ES::Secret::degree>{90});
    static_assert( std::is_same_v<
            decltype(rad)::value_type, float
        >
        );

    {
        using namespace ES;
        static constexpr float w = angle_rad(angle_deg{180.f}).get();
    }
}
