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

TEST_CASE("RGB luminance", "[RGB]") {
    auto white = RGB(1.0f, 1.0f, 1.0f);
    auto black = RGB(0.0f, 0.0f, 0.0f);
    
    REQUIRE(white.luminance() > 0.9f);
    REQUIRE(black.luminance() < 0.1f);
}

TEST_CASE("RGB to_srgb", "[RGB]") {
    auto color = RGB::from_linear(0.5f, 0.5f, 0.5f);
    auto srgb = color.to_srgb();
    
    REQUIRE(srgb[0] > 0.7f);
    REQUIRE(srgb[1] > 0.7f);
    REQUIRE(srgb[2] > 0.7f);
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

TEST_CASE("RGB_Int Color operator mixin check" "[RGB_Int]"){
    RGB_Int rgb1(255,255,255);
    RGB_Int rgb2(127,127,127);
    RGB_Int rgb3(127,127,127);
    RGB_Int rgb4(0,0,0);
    RGB_Int rgb5(255,0,255);
    RGB_Int rgb6(163,36,163);

    REQUIRE(rgb2.adjust_brightness_in_place(1)==rgb3);
    REQUIRE(rgb1.adjust_brightness(.5)== rgb2);
    REQUIRE(rgb1.invert() == rgb4);
    REQUIRE(rgb1.invert_in_place() == rgb4);
    REQUIRE(rgb5.adjust_saturation(.5).almost_equal(rgb6));
    REQUIRE(rgb5.adjust_saturation_in_place(.5).almost_equal(rgb6));    

}

TEST_CASE("RGB_Int from_srgb with floats", "[RGB_Int]") {
    auto color = RGB_Int::from_srgb(0.5f, 0.5f, 0.5f);
    
    REQUIRE(color.R() > 50);
    REQUIRE(color.R() < 65);
}

TEST_CASE("RGB_Int from_srgb with ints", "[RGB_Int]") {
    auto color = RGB_Int::from_srgb(255, 128, 64);
    
    REQUIRE(color.R() > 250);
    REQUIRE(color.G() > 0);
    REQUIRE(color.B() > 0);
}

TEST_CASE("RGB_Int from_hexRGB", "[RGB_Int]") {
    auto red = RGB_Int::from_hexRGB(0xFF0000);
    auto green = RGB_Int::from_hexRGB(0x00FF00);
    auto blue = RGB_Int::from_hexRGB(0x0000FF);
    
    REQUIRE(red.R() > 250);
    REQUIRE(green.G() > 250);
    REQUIRE(blue.B() > 250);
}

TEST_CASE("RGB_Int from_hexBGR", "[RGB_Int]") {
    auto red = RGB_Int::from_hexBGR(0x0000FF);
    auto blue = RGB_Int::from_hexBGR(0xFF0000);
    
    REQUIRE(red.R() > 250);
    REQUIRE(blue.B() > 250);
}

TEST_CASE("RGB_Int from_linear", "[RGB_Int]") {
    auto color1 = RGB_Int::from_linear(255, 128, 64);
    auto color2 = RGB_Int::from_linear(1.0f, 0.5f, 0.25f);
    
    REQUIRE(color1.R() == 255);
    REQUIRE(color1.G() == 128);
    REQUIRE(color1.B() == 64);
    
    REQUIRE(color2.R() == 255);
    REQUIRE(color2.G() > 125);
    REQUIRE(color2.B() > 60);
}

TEST_CASE("RGB_Int luminance", "[RGB_Int]") {
    auto white = RGB_Int(255, 255, 255);
    auto black = RGB_Int(0, 0, 0);
    
    REQUIRE(white.luminance() > 0.9f);
    REQUIRE(black.luminance() < 0.1f);
}

TEST_CASE("RGB_Int to_srgb", "[RGB_Int]") {
    auto color = RGB_Int::from_linear(128, 128, 128);
    auto srgb = color.to_srgb();
    
    REQUIRE(srgb[0] > 0.7f);
    REQUIRE(srgb[1] > 0.7f);
    REQUIRE(srgb[2] > 0.7f);
}


TEST_CASE("RGBA accessors checks", "[RGBA]"){
    auto rgba = RGBA(1.0f, 1.0f, 1.0f, 1.0f);
    REQUIRE(rgba.R() == 1.0f);
    REQUIRE(rgba.G() == 1.0f);
    REQUIRE(rgba.B() == 1.0f);
    REQUIRE(rgba.A() == 1.0f);

    REQUIRE(rgba == rgba);

    rgba.R() = 0.5f;
    rgba.G() = 0.5f;
    rgba.B() = 0.5f;
    rgba.A() = 0.5f;

    REQUIRE(rgba.R() == 0.5f);
    REQUIRE(rgba.G() == 0.5f);
    REQUIRE(rgba.B() == 0.5f);
    REQUIRE(rgba.A() == 0.5f);
}

TEST_CASE("RGBA arithmetic operator mixin check", "[RGBA]"){
    auto rgba = RGBA(0.5f, 0.5f, 0.5f, 0.5f);
    auto one = RGBA(1.0f, 1.0f, 1.0f, 1.0f);
    auto zero = RGBA(0.0f, 0.0f, 0.0f, 0.0f);

    REQUIRE((rgba + zero) == rgba);
    REQUIRE((rgba - zero) == rgba);
    REQUIRE((rgba * 1) == rgba);
    REQUIRE((rgba * one) == rgba);
    REQUIRE((rgba / one) == rgba);
    REQUIRE((rgba / 1) == rgba);
    REQUIRE((one.clamp(0, 0.5)) == rgba);
}

TEST_CASE("RGBA Color operator mixin check", "[RGBA]"){
    RGBA rgba1(1.0f, 1.0f, 1.0f, 1.0f);
    RGBA rgba2(0.5f, 0.5f, 0.5f, 1.0f);
    RGBA rgba3(0.0f, 0.0f, 0.0f, 1.0f);
    
    REQUIRE(rgba1.adjust_brightness(0.5).almost_equal(rgba2));
    REQUIRE(rgba1.adjust_brightness_in_place(0.5).almost_equal(rgba2));
    
    RGBA rgba4(1.0f, 1.0f, 1.0f, 1.0f);
    REQUIRE(rgba4.invert().almost_equal(rgba3));
    REQUIRE(rgba4.invert_in_place().almost_equal(rgba3));
}

TEST_CASE("RGBA from_srgba with floats", "[RGBA]") {
    auto color = RGBA::from_srgba(0.5f, 0.5f, 0.5f, 1.0f);
    
    REQUIRE(color.R() > 0.2f);
    REQUIRE(color.R() < 0.25f);
    REQUIRE(color.A() == 1.0f);
}

TEST_CASE("RGBA from_srgba with ints", "[RGBA]") {
    auto color = RGBA::from_srgba(255, 128, 64, 255);
    
    REQUIRE(color.R() > 0.9f);
    REQUIRE(color.G() > 0.0f);
    REQUIRE(color.B() > 0.0f);
    REQUIRE(color.A() == 1.0f);
}

TEST_CASE("RGBA from_hexRGBA", "[RGBA]") {
    auto red = RGBA::from_hexRGBA(0xFF0000FF);
    auto green = RGBA::from_hexRGBA(0x00FF00FF);
    auto transparent = RGBA::from_hexRGBA(0xFF000000);
    
    REQUIRE(red.R() > 0.9f);
    REQUIRE(red.A() == 1.0f);
    REQUIRE(green.G() > 0.9f);
    REQUIRE(transparent.A() < 0.1f);
}

TEST_CASE("RGBA from_hexBGRA", "[RGBA]") {
    auto red = RGBA::from_hexBGRA(0x0000FFFF);
    auto blue = RGBA::from_hexBGRA(0xFF0000FF);
    
    REQUIRE(red.R() > 0.9f);
    REQUIRE(blue.B() > 0.9f);
}

TEST_CASE("RGBA from_straight_linear", "[RGBA]") {
    auto color1 = RGBA::from_straight_linear(1.0f, 0.5f, 0.25f, 0.5f);
    auto color2 = RGBA::from_straight_linear(255, 128, 64, 128);
    
    REQUIRE(color1.R() == 0.5f);
    REQUIRE(color1.A() == 0.5f);
    
    REQUIRE(color2.A() > 0.49f);
    REQUIRE(color2.A() < 0.51f);
}

TEST_CASE("RGBA from_premultiplied_linear", "[RGBA]") {
    auto color1 = RGBA::from_premultiplied_linear(0.5f, 0.25f, 0.125f, 0.5f);
    auto color2 = RGBA::from_premultiplied_linear(128, 64, 32, 128);
    
    REQUIRE(color1.R() == 0.5f);
    REQUIRE(color1.G() == 0.25f);
    REQUIRE(color1.A() == 0.5f);
    
    REQUIRE(color2.A() > 0.49f);
    REQUIRE(color2.A() < 0.51f);
}

TEST_CASE("RGBA luminance", "[RGBA]") {
    auto white = RGBA::from_straight_linear(1.0f, 1.0f, 1.0f, 1.0f);
    auto black = RGBA::from_straight_linear(0.0f, 0.0f, 0.0f, 1.0f);
    auto transparent = RGBA::from_straight_linear(1.0f, 1.0f, 1.0f, 0.0f);
    
    REQUIRE(white.luminance() > 0.9f);
    REQUIRE(black.luminance() < 0.1f);
    REQUIRE(transparent.luminance() == 0.0f);
}

TEST_CASE("RGBA to_srgba", "[RGBA]") {
    auto color = RGBA::from_straight_linear(0.5f, 0.5f, 0.5f, 1.0f);
    auto srgba = color.to_srgba();
    
    REQUIRE(srgba[0] > 0.7f);
    REQUIRE(srgba[1] > 0.7f);
    REQUIRE(srgba[2] > 0.7f);
    REQUIRE(srgba[3] == 1.0f);
}

TEST_CASE("RGBA to_straight_linear", "[RGBA]") {
    auto color = RGBA::from_straight_linear(0.5f, 0.4f, 0.3f, 0.8f);
    auto straight = color.to_straight_linear();
    
    REQUIRE(straight[0] > 0.49f);
    REQUIRE(straight[0] < 0.51f);
    REQUIRE(straight[3] == 0.8f);
}


TEST_CASE("RGBA_Int accessors checks", "[RGBA_Int]"){
    auto rgba = RGBA_Int(255, 255, 255, 255);
    REQUIRE(rgba.R() == 255);
    REQUIRE(rgba.G() == 255);
    REQUIRE(rgba.B() == 255);
    REQUIRE(rgba.A() == 255);

    REQUIRE(rgba == rgba);

    rgba.R() = 128;
    rgba.G() = 128;
    rgba.B() = 128;
    rgba.A() = 128;

    REQUIRE(rgba.R() == 128);
    REQUIRE(rgba.G() == 128);
    REQUIRE(rgba.B() == 128);
    REQUIRE(rgba.A() == 128);
}

TEST_CASE("RGBA_Int arithmetic operator mixin check", "[RGBA_Int]"){
    auto rgba = RGBA_Int(128, 128, 128, 128);
    auto rgba2 = RGBA_Int(127, 127, 127, 127);
    auto one = RGBA_Int(255, 255, 255, 255);
    auto zero = RGBA_Int(0, 0, 0, 0);

    REQUIRE((rgba + zero) == rgba);
    REQUIRE((rgba - zero) == rgba);
    REQUIRE((rgba * 1) == rgba);
    REQUIRE((rgba / 1) == rgba);
    REQUIRE((one.clamp(0, 128)) == rgba);
    REQUIRE((one.lerp(zero, 0.5)) == rgba2);
}

TEST_CASE("RGBA_Int Color operator mixin check", "[RGBA_Int]"){
    RGBA_Int rgba1(255, 255, 255, 255);
    RGBA_Int rgba2(127, 127, 127, 255);
    RGBA_Int rgba3(0, 0, 0, 255);
    
    REQUIRE(rgba1.adjust_brightness(0.5).almost_equal(rgba2));
    
    RGBA_Int rgba4(255, 255, 255, 255);
    REQUIRE(rgba4.invert().almost_equal(rgba3));
}

TEST_CASE("RGBA_Int from_srgba with floats", "[RGBA_Int]") {
    auto color = RGBA_Int::from_srgba(0.5f, 0.5f, 0.5f, 1.0f);
    
    REQUIRE(color.R() > 50);
    REQUIRE(color.R() < 65);
    REQUIRE(color.A() == 255);
}

TEST_CASE("RGBA_Int from_srgba with ints", "[RGBA_Int]") {
    auto color = RGBA_Int::from_srgba(255, 128, 64, 255);
    
    REQUIRE(color.R() > 250);
    REQUIRE(color.G() > 0);
    REQUIRE(color.B() > 0);
    REQUIRE(color.A() == 255);
}

TEST_CASE("RGBA_Int from_hexRGBA", "[RGBA_Int]") {
    auto red = RGBA_Int::from_hexRGBA(0xFF0000FF);
    auto green = RGBA_Int::from_hexRGBA(0x00FF00FF);
    auto transparent = RGBA_Int::from_hexRGBA(0xFF000000);
    
    REQUIRE(red.R() > 250);
    REQUIRE(red.A() == 255);
    REQUIRE(green.G() > 250);
    REQUIRE(transparent.A() == 0);
}

TEST_CASE("RGBA_Int from_hexBGRA", "[RGBA_Int]") {
    auto red = RGBA_Int::from_hexBGRA(0x0000FFFF);
    auto blue = RGBA_Int::from_hexBGRA(0xFF0000FF);
    
    REQUIRE(red.R() > 250);
    REQUIRE(blue.B() > 250);
}

TEST_CASE("RGBA_Int from_straight_linear with ints", "[RGBA_Int]") {
    auto color = RGBA_Int::from_straight_linear(255, 128, 64, 128);
    
    REQUIRE(color.R() == 255);
    REQUIRE(color.G() == 128);
    REQUIRE(color.B() == 64);
    REQUIRE(color.A() == 128);
}

TEST_CASE("RGBA_Int from_straight_linear with floats", "[RGBA_Int]") {
    auto color = RGBA_Int::from_straight_linear(1.0f, 0.5f, 0.25f, 0.5f);
    
    REQUIRE(color.R() == 255);
    REQUIRE(color.G() > 125);
    REQUIRE(color.A() > 125);
}

TEST_CASE("RGBA_Int luminance", "[RGBA_Int]") {
    auto white = RGBA_Int::from_straight_linear(255, 255, 255, 255);
    auto black = RGBA_Int::from_straight_linear(0, 0, 0, 255);
    
    REQUIRE(white.luminance() > 0.9f);
    REQUIRE(black.luminance() < 0.1f);
}

TEST_CASE("RGBA_Int to_srgba", "[RGBA_Int]") {
    auto color = RGBA_Int::from_straight_linear(128, 128, 128, 255);
    auto srgba = color.to_srgba();
    
    REQUIRE(srgba[0] > 0.7f);
    REQUIRE(srgba[1] > 0.7f);
    REQUIRE(srgba[2] > 0.7f);
    REQUIRE(srgba[3] == 1.0f);
}

TEST_CASE("RGBA_Int to_straight_linear", "[RGBA_Int]") {
    auto color = RGBA_Int::from_straight_linear(128, 100, 75, 200);
    auto straight = color.to_straight_linear();
    
    REQUIRE(straight[0] == 128);
    REQUIRE(straight[1] == 100);
    REQUIRE(straight[2] == 75);
    REQUIRE(straight[3] == 200);
}


TEST_CASE("LA accessors checks", "[LA]"){
    auto la = LA(1.0f, 1.0f);
    REQUIRE(la.L() == 1.0f);
    REQUIRE(la.A() == 1.0f);

    REQUIRE(la == la);

    la.L() = 0.5f;
    la.A() = 0.5f;

    REQUIRE(la.L() == 0.5f);
    REQUIRE(la.A() == 0.5f);
}

TEST_CASE("LA from_straight_linear with floats", "[LA]") {
    auto color = LA::from_straight_linear(0.8f, 0.5f);
    
    REQUIRE(color.L() == 0.4f);
    REQUIRE(color.A() == 0.5f);
}

TEST_CASE("LA from_straight_linear with ints", "[LA]") {
    auto color = LA::from_straight_linear(200, 128);
    
    REQUIRE(color.A() > 0.49f);
    REQUIRE(color.A() < 0.51f);
}

TEST_CASE("LA from_premultiplied_linear with floats", "[LA]") {
    auto color = LA::from_premultiplied_linear(0.4f, 0.5f);
    
    REQUIRE(color.L() == 0.4f);
    REQUIRE(color.A() == 0.5f);
}

TEST_CASE("LA from_premultiplied_linear with ints", "[LA]") {
    auto color = LA::from_premultiplied_linear(128, 255);
    
    REQUIRE(color.L() > 0.49f);
    REQUIRE(color.L() < 0.51f);
    REQUIRE(color.A() == 1.0f);
}

TEST_CASE("LA luminance", "[LA]") {
    auto white = LA::from_straight_linear(1.0f, 1.0f);
    auto black = LA::from_straight_linear(0.0f, 1.0f);
    
    REQUIRE(white.luminance() == 1.0f);
    REQUIRE(black.luminance() == 0.0f);
}

TEST_CASE("LA to_straight_linear", "[LA]") {
    auto color = LA::from_straight_linear(0.8f, 0.5f);
    auto straight = color.to_straight_linear();
    
    REQUIRE(straight[0] > 0.79f);
    REQUIRE(straight[0] < 0.81f);
    REQUIRE(straight[1] == 0.5f);
}

TEST_CASE("LA to_straight_linear with zero alpha", "[LA]") {
    auto color = LA::from_straight_linear(0.8f, 0.0f);
    auto straight = color.to_straight_linear();
    
    REQUIRE(straight[0] == 0.0f);
    REQUIRE(straight[1] == 0.0f);
}