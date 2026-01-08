#include "../Angle.hpp"
#include <catch2/catch_test_macros.hpp>
#include "ES_test_util.hpp"


constexpr float PIE = std::numbers::pi_v<float>; //TODO: use the promised in-house ES::math pi.
constexpr float PINF = std::numeric_limits<float>::infinity();



using namespace ES::math::angle_literals;

TEST_CASE("Angle!","[Angle]") {
    SECTION("Trivial equality") {
        DOUBLE_REQUIRE(ES::AngleDeg{90.f} == ES::AngleRad{PIE/2.f});
    }
    SECTION("Addition") {
        DOUBLE_REQUIRE(ES::AngleDeg{90.f} + ES::AngleRad{PIE/2} == 180_deg);
    }
    SECTION("Subtraction") {
        DOUBLE_REQUIRE((ES::AngleRad{-200.f} -= ES::AngleDeg{3000.f}) < ES::AngleRad{-200.f});
    }
    SECTION("Multiplication") {
        DOUBLE_REQUIRE(ES::AngleRad{10} * 1 == ES::AngleRad{10});
        DOUBLE_REQUIRE(ES::AngleRad{10} * 2 == ES::AngleRad{20});
    }
    SECTION("Division (ratio)") {
        ES::AngleDeg{90} / ES::AngleRad{2*PIE} == ES::AngleDeg{360};
        DOUBLE_REQUIRE(ES::AngleDeg{90.f} / ES::AngleRad{2.f*PIE} == ES::AngleDeg{90.f/360.f});
    }
    SECTION("Coterminals!") {
        DOUBLE_REQUIRE(ES::AngleDeg{360e10f}.normalize() == ES::AngleRad{0.f});
    }
}

TEST_CASE("template sanity crisis!","[Angle]") {
    ES::AngleDeg zed{ES::AngleRad{180.f}};
    STATIC_REQUIRE( std::is_same_v<
        decltype(zed)::value_type, float
    >
    );

    auto rad = ES::Angle<ES::Secret::radian, float>(ES::Angle<ES::Secret::degree>{90});
    static_assert( std::is_same_v<
            decltype(rad)::value_type, float>
        );

    {
        using namespace ES;
        static constexpr float w = AngleRad(AngleDeg{180.f}).get();
    }
}
