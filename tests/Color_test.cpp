#define NDEBUB
#include <catch2/catch_test_macros.hpp>
#include "../ES_math.hpp"
#include "../ColorN.hpp"


using namespace ES;



TEST_CASE("RGB accesors checks", "[RGB]"){
    auto rgb = RGB(1.0f,1.0f,1.0f);
    REQUIRE(rgb.R() == 1);
    REQUIRE(rgb.G() == 1);
    REQUIRE(rgb.B() == 1);

    REQUIRE(rgb == rgb);


    rgb.R() = .5f;
    rgb.G() = .5f;
    rgb.B() = .5f;

    REQUIRE(rgb.R() == .5f);
    REQUIRE(rgb.G() == .5f);
    REQUIRE(rgb.B() == .5f);

}

TEST_CASE("RGB arithmetic operator mixin check" "[RGB]"){
    auto rgb = RGB(.5f,.5f,.5f);
    auto one  = RGB(1,1,1);
    RGB zero = RGB(0,0,0);

    REQUIRE((rgb+zero) == rgb);
    REQUIRE((rgb-zero) == rgb);
    REQUIRE((rgb * 1) == rgb);
    REQUIRE((rgb*one) == rgb);
    REQUIRE((rgb/one) == rgb);
    REQUIRE((rgb/1) == rgb);
    REQUIRE((one.clamp(0,.5)) ==rgb);
    REQUIRE((one.lerp(zero,.5) == rgb));
}

TEST_CASE("RGB Color operator mixin check" "[RGB]"){
    RGB rgb1(1,1,1);
    RGB rgb2(1,1,1);
    RGB rgb3(.5,.5,.5);
    RGB rgb4(0,0,0);
    RGB rgb5(1,0,1);
    RGB rgb6(0.6424, 0.1424, 0.6424);

    REQUIRE(rgb2.adjust_brightness_in_place(.5)==rgb3);
    REQUIRE(rgb1.adjust_brightness(.5)== rgb2);
    REQUIRE(rgb1.invert() == rgb4);
    REQUIRE(rgb1.invert_in_place() == rgb4);
    REQUIRE(rgb5.adjust_saturation(.5).almost_equal(rgb6));
    REQUIRE(rgb5.adjust_saturation_in_place(.5).almost_equal(rgb6));    

}

TEST_CASE("RGB from_srgb with floats", "[RGB]") {
    auto color = RGB::from_srgb(0.5f, 0.5f, 0.5f);
    
    REQUIRE(color.R() > 0.2f);
    REQUIRE(color.R() < 0.25f);
}

TEST_CASE("RGB from_srgb with ints", "[RGB]") {
    auto color = RGB::from_srgb(255, 128, 64);
    
    REQUIRE(color.R() > 0.9f);
    REQUIRE(color.G() > 0.0f);
    REQUIRE(color.B() > 0.0f);
}

TEST_CASE("RGB from_hexRGB", "[RGB]") {
    auto red = RGB::from_hexRGB(0xFF0000);
    auto green = RGB::from_hexRGB(0x00FF00);
    auto blue = RGB::from_hexRGB(0x0000FF);
    
    REQUIRE(red.R() > 0.9f);
    REQUIRE(green.G() > 0.9f);
    REQUIRE(blue.B() > 0.9f);
}

TEST_CASE("RGB from_hexBGR", "[RGB]") {
    auto red = RGB::from_hexBGR(0x0000FF);
    auto blue = RGB::from_hexBGR(0xFF0000);
    
    REQUIRE(red.R() > 0.9f);
    REQUIRE(blue.B() > 0.9f);
}


TEST_CASE("RGB_Int accesors checks", "[RGB_Int]"){
    auto rgb = RGB_Int(255,255,255);
    REQUIRE(rgb.R() == 255);
    REQUIRE(rgb.G() == 255);
    REQUIRE(rgb.B() == 255);

    REQUIRE(rgb == rgb);


    rgb.R() = 100;
    rgb.G() = 100;
    rgb.B() = 100;

    REQUIRE(rgb.R() == 100);
    REQUIRE(rgb.G() == 100);
    REQUIRE(rgb.B() == 100);

}

TEST_CASE("RGB_Int arithmetic operator mixin check" "[RGB_Int]"){
    auto rgb = RGB_Int(128,128,128);
    auto rgb2 = RGB_Int(127,127,127);
    auto one  = RGB_Int(255,255,255);
    auto zero = RGB_Int(0,0,0);

    REQUIRE((rgb+zero) == rgb);
    REQUIRE((rgb-zero) == rgb);
    REQUIRE((rgb * 1) == rgb);
    REQUIRE((rgb/1) == rgb);
    REQUIRE((one.clamp(0,128)) ==rgb);
    REQUIRE((one.lerp(zero,.5) == rgb2));
}


