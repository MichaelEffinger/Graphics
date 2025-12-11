#define NDEBUG
#include <catch2/catch_test_macros.hpp>
#include "../ES_math.hpp"
#include "../ColorN.hpp"

using namespace ES;

// ========================================
// RGB TESTS (with full ColorOpsMixin coverage)
// ========================================

// ========== ACCESSOR TESTS ==========

TEST_CASE("RGB accessors R, G, B", "[RGB]") {
    RGB color{0.5f, 0.6f, 0.7f};
    
    REQUIRE(math::approx_equal(color.R(), 0.5f));
    REQUIRE(math::approx_equal(color.G(), 0.6f));
    REQUIRE(math::approx_equal(color.B(), 0.7f));
    
    // Test mutability
    color.R() = 0.1f;
    color.G() = 0.2f;
    color.B() = 0.3f;
    
    REQUIRE(math::approx_equal(color.R(), 0.1f));
    REQUIRE(math::approx_equal(color.G(), 0.2f));
    REQUIRE(math::approx_equal(color.B(), 0.3f));
}

// ========== COLOROPSMIXIN TESTS (tested once on RGB) ==========

TEST_CASE("RGB operator+ adds components", "[RGB]") {
    RGB color1{0.2f, 0.3f, 0.4f};
    RGB color2{0.1f, 0.2f, 0.3f};
    
    auto result = color1 + color2;
    
    REQUIRE(math::approx_equal(result.R(), 0.3f));
    REQUIRE(math::approx_equal(result.G(), 0.5f));
    REQUIRE(math::approx_equal(result.B(), 0.7f));
}

TEST_CASE("RGB operator+= adds components in-place", "[RGB]") {
    RGB color1{0.2f, 0.3f, 0.4f};
    RGB color2{0.1f, 0.2f, 0.3f};
    
    color1 += color2;
    
    REQUIRE(math::approx_equal(color1.R(), 0.3f));
    REQUIRE(math::approx_equal(color1.G(), 0.5f));
    REQUIRE(math::approx_equal(color1.B(), 0.7f));
}

TEST_CASE("RGB operator- subtracts components", "[RGB]") {
    RGB color1{0.5f, 0.6f, 0.7f};
    RGB color2{0.1f, 0.2f, 0.3f};
    
    auto result = color1 - color2;
    
    REQUIRE(math::approx_equal(result.R(), 0.4f));
    REQUIRE(math::approx_equal(result.G(), 0.4f));
    REQUIRE(math::approx_equal(result.B(), 0.4f));
}

TEST_CASE("RGB operator-= subtracts components in-place", "[RGB]") {
    RGB color1{0.5f, 0.6f, 0.7f};
    RGB color2{0.1f, 0.2f, 0.3f};
    
    color1 -= color2;
    
    REQUIRE(math::approx_equal(color1.R(), 0.4f));
    REQUIRE(math::approx_equal(color1.G(), 0.4f));
    REQUIRE(math::approx_equal(color1.B(), 0.4f));
}

TEST_CASE("RGB operator* multiplies components", "[RGB]") {
    RGB color1{0.5f, 0.6f, 0.8f};
    RGB color2{0.2f, 0.5f, 0.5f};
    
    auto result = color1 * color2;
    
    REQUIRE(math::approx_equal(result.R(), 0.1f));
    REQUIRE(math::approx_equal(result.G(), 0.3f));
    REQUIRE(math::approx_equal(result.B(), 0.4f));
}

TEST_CASE("RGB operator*= multiplies components in-place", "[RGB]") {
    RGB color1{0.5f, 0.6f, 0.8f};
    RGB color2{0.2f, 0.5f, 0.5f};
    
    color1 *= color2;
    
    REQUIRE(math::approx_equal(color1.R(), 0.1f));
    REQUIRE(math::approx_equal(color1.G(), 0.3f));
    REQUIRE(math::approx_equal(color1.B(), 0.4f));
}

TEST_CASE("RGB operator/ divides components", "[RGB]") {
    RGB color1{0.4f, 0.6f, 0.8f};
    RGB color2{0.2f, 0.3f, 0.4f};
    
    auto result = color1 / color2;
    
    REQUIRE(math::approx_equal(result.R(), 2.0f));
    REQUIRE(math::approx_equal(result.G(), 2.0f));
    REQUIRE(math::approx_equal(result.B(), 2.0f));
}

TEST_CASE("RGB operator/= divides components in-place", "[RGB]") {
    RGB color1{0.4f, 0.6f, 0.8f};
    RGB color2{0.2f, 0.3f, 0.4f};
    
    color1 /= color2;
    
    REQUIRE(math::approx_equal(color1.R(), 2.0f));
    REQUIRE(math::approx_equal(color1.G(), 2.0f));
    REQUIRE(math::approx_equal(color1.B(), 2.0f));
}

#ifdef NDEBUG
TEST_CASE("RGB operator/ division by zero component", "[RGB]") {
    RGB color1{0.4f, 0.6f, 0.8f};
    RGB color2{0.0f, 0.3f, 0.4f};
    
    auto result = color1 / color2;
    
    REQUIRE(math::approx_equal(result.R(), 0.0f));
    REQUIRE(math::approx_equal(result.G(), 2.0f));
    REQUIRE(math::approx_equal(result.B(), 2.0f));
}
#endif

TEST_CASE("RGB operator* with scalar", "[RGB]") {
    RGB color{0.2f, 0.4f, 0.6f};
    
    auto result = color * 2.0f;
    
    REQUIRE(math::approx_equal(result.R(), 0.4f));
    REQUIRE(math::approx_equal(result.G(), 0.8f));
    REQUIRE(math::approx_equal(result.B(), 1.2f));
}

TEST_CASE("RGB operator* with scalar on left", "[RGB]") {
    RGB color{0.2f, 0.4f, 0.6f};
    
    auto result = 2.5f * color;
    
    REQUIRE(math::approx_equal(result.R(), 0.5f));
    REQUIRE(math::approx_equal(result.G(), 1.0f));
    REQUIRE(math::approx_equal(result.B(), 1.5f));
}

TEST_CASE("RGB operator*= with scalar", "[RGB]") {
    RGB color{0.2f, 0.4f, 0.6f};
    
    color *= 3.0f;
    
    REQUIRE(math::approx_equal(color.R(), 0.6f));
    REQUIRE(math::approx_equal(color.G(), 1.2f));
    REQUIRE(math::approx_equal(color.B(), 1.8f));
}

TEST_CASE("RGB operator/ with scalar", "[RGB]") {
    RGB color{0.6f, 0.9f, 1.2f};
    
    auto result = color / 3.0f;
    
    REQUIRE(math::approx_equal(result.R(), 0.2f));
    REQUIRE(math::approx_equal(result.G(), 0.3f));
    REQUIRE(math::approx_equal(result.B(), 0.4f));
}

TEST_CASE("RGB operator/= with scalar", "[RGB]") {
    RGB color{0.8f, 1.2f, 1.6f};
    
    color /= 4.0f;
    
    REQUIRE(math::approx_equal(color.R(), 0.2f));
    REQUIRE(math::approx_equal(color.G(), 0.3f));
    REQUIRE(math::approx_equal(color.B(), 0.4f));
}

#ifdef NDEBUG
TEST_CASE("RGB operator/ scalar division by zero", "[RGB]") {
    RGB color{0.6f, 0.9f, 1.2f};
    
    auto result = color / 0.0f;
    
    REQUIRE(math::approx_equal(result.R(), 0.0f));
    REQUIRE(math::approx_equal(result.G(), 0.0f));
    REQUIRE(math::approx_equal(result.B(), 0.0f));
}
#endif

TEST_CASE("RGB lerp interpolates between colors", "[RGB]") {
    RGB color1{0.0f, 0.0f, 0.0f};
    RGB color2{1.0f, 1.0f, 1.0f};
    
    auto result = color1.lerp(color2, 0.5f);
    
    REQUIRE(math::approx_equal(result.R(), 0.5f));
    REQUIRE(math::approx_equal(result.G(), 0.5f));
    REQUIRE(math::approx_equal(result.B(), 0.5f));
}

TEST_CASE("RGB lerp at t=0 returns first color", "[RGB]") {
    RGB color1{0.2f, 0.4f, 0.6f};
    RGB color2{0.8f, 0.6f, 0.4f};
    
    auto result = color1.lerp(color2, 0.0f);
    
    REQUIRE(math::approx_equal(result.R(), 0.2f));
    REQUIRE(math::approx_equal(result.G(), 0.4f));
    REQUIRE(math::approx_equal(result.B(), 0.6f));
}

TEST_CASE("RGB lerp at t=1 returns second color", "[RGB]") {
    RGB color1{0.2f, 0.4f, 0.6f};
    RGB color2{0.8f, 0.6f, 0.4f};
    
    auto result = color1.lerp(color2, 1.0f);
    
    REQUIRE(math::approx_equal(result.R(), 0.8f));
    REQUIRE(math::approx_equal(result.G(), 0.6f));
    REQUIRE(math::approx_equal(result.B(), 0.4f));
}

TEST_CASE("RGB lerp_in_place interpolates in-place", "[RGB]") {
    RGB color1{0.0f, 0.0f, 0.0f};
    RGB color2{1.0f, 1.0f, 1.0f};
    
    color1.lerp_in_place(color2, 0.25f);
    
    REQUIRE(math::approx_equal(color1.R(), 0.25f));
    REQUIRE(math::approx_equal(color1.G(), 0.25f));
    REQUIRE(math::approx_equal(color1.B(), 0.25f));
}

TEST_CASE("RGB clamp constrains values to range", "[RGB]") {
    RGB color{-0.2f, 0.5f, 1.5f};
    
    auto result = color.clamp(0.0f, 1.0f);
    
    REQUIRE(math::approx_equal(result.R(), 0.0f));
    REQUIRE(math::approx_equal(result.G(), 0.5f));
    REQUIRE(math::approx_equal(result.B(), 1.0f));
}

TEST_CASE("RGB clamp_in_place constrains values in-place", "[RGB]") {
    RGB color{-0.5f, 0.3f, 2.0f};
    
    color.clamp_in_place(0.0f, 1.0f);
    
    REQUIRE(math::approx_equal(color.R(), 0.0f));
    REQUIRE(math::approx_equal(color.G(), 0.3f));
    REQUIRE(math::approx_equal(color.B(), 1.0f));
}

TEST_CASE("RGB adjust_brightness scales color", "[RGB]") {
    RGB color{0.5f, 0.6f, 0.8f};
    
    auto result = color.adjust_brightness(2.0f);
    
    REQUIRE(math::approx_equal(result.R(), 1.0f));
    REQUIRE(math::approx_equal(result.G(), 1.2f));
    REQUIRE(math::approx_equal(result.B(), 1.6f));
}

TEST_CASE("RGB adjust_brightness_in_place scales color in-place", "[RGB]") {
    RGB color{0.4f, 0.6f, 0.8f};
    
    color.adjust_brightness_in_place(1.5f);
    
    REQUIRE(math::approx_equal(color.R(), 0.6f));
    REQUIRE(math::approx_equal(color.G(), 0.9f));
    REQUIRE(math::approx_equal(color.B(), 1.2f));
}

TEST_CASE("RGB invert inverts color components", "[RGB]") {
    RGB color{0.2f, 0.5f, 0.8f};
    
    auto result = color.invert();
    
    REQUIRE(math::approx_equal(result.R(), 0.8f));
    REQUIRE(math::approx_equal(result.G(), 0.5f));
    REQUIRE(math::approx_equal(result.B(), 0.2f));
}

TEST_CASE("RGB invert_in_place inverts color in-place", "[RGB]") {
    RGB color{0.3f, 0.6f, 0.9f};
    
    color.invert_in_place();
    
    REQUIRE(math::approx_equal(color.R(), 0.7f));
    REQUIRE(math::approx_equal(color.G(), 0.4f));
    REQUIRE(math::approx_equal(color.B(), 0.1f));
}

TEST_CASE("RGB adjust_saturation reduces saturation", "[RGB]") {
    RGB color{1.0f, 0.0f, 0.0f}; // Pure red
    
    auto result = color.adjust_saturation(0.5f);
    
    // Should move toward gray
    REQUIRE(result.R() < 1.0f);
    REQUIRE(result.G() > 0.0f);
    REQUIRE(result.B() > 0.0f);
}

TEST_CASE("RGB adjust_saturation full desaturation", "[RGB]") {
    RGB color{1.0f, 0.0f, 0.0f}; // Pure red
    
    auto result = color.adjust_saturation(0.0f);
    
    // Should be grayscale with luminance value
    float lum = color.luminance();
    REQUIRE(math::approx_equal(result.R(), lum));
    REQUIRE(math::approx_equal(result.G(), lum));
    REQUIRE(math::approx_equal(result.B(), lum));
}

TEST_CASE("RGB adjust_saturation_in_place reduces saturation in-place", "[RGB]") {
    RGB color{0.0f, 1.0f, 0.0f}; // Pure green
    
    color.adjust_saturation_in_place(0.5f);
    
    // Should move toward gray
    REQUIRE(color.R() > 0.0f);
    REQUIRE(color.G() < 1.0f);
    REQUIRE(color.B() > 0.0f);
}

// ========== RGB-SPECIFIC FUNCTION TESTS ==========

TEST_CASE("RGB from_srgb with float values", "[RGB]") {
    auto color = RGB::from_srgb(0.5f, 0.5f, 0.5f);
    
    // sRGB 0.5 is approximately 0.214 in linear space
    REQUIRE(color.R() > 0.2f);
    REQUIRE(color.R() < 0.25f);
}

TEST_CASE("RGB from_srgb with int values", "[RGB]") {
    auto color = RGB::from_srgb(255, 128, 64);
    
    REQUIRE(color.R() > 0.9f);
    REQUIRE(color.G() > 0.0f);
    REQUIRE(color.B() > 0.0f);
}

TEST_CASE("RGB from_hex creates color from hex value", "[RGB]") {
    auto color = RGB::from_hex(0xFF0000);
    
    REQUIRE(math::approx_equal(color.R(), 1.0f));
    REQUIRE(math::approx_equal(color.G(), 0.0f));
    REQUIRE(math::approx_equal(color.B(), 0.0f));
}

TEST_CASE("RGB from_hex white", "[RGB]") {
    auto color = RGB::from_hex(0xFFFFFF);
    
    REQUIRE(math::approx_equal(color.R(), 1.0f));
    REQUIRE(math::approx_equal(color.G(), 1.0f));
    REQUIRE(math::approx_equal(color.B(), 1.0f));
}

TEST_CASE("RGB from_linear with float values", "[RGB]") {
    auto color = RGB::from_linear(0.5f, 0.6f, 0.7f);
    
    REQUIRE(math::approx_equal(color.R(), 0.5f));
    REQUIRE(math::approx_equal(color.G(), 0.6f));
    REQUIRE(math::approx_equal(color.B(), 0.7f));
}

TEST_CASE("RGB from_linear with int values", "[RGB]") {
    auto color = RGB::from_linear(255, 128, 64);
    
    REQUIRE(math::approx_equal(color.R(), 1.0f));
    REQUIRE(math::approx_equal(color.G(), 128.0f / 255.0f));
    REQUIRE(math::approx_equal(color.B(), 64.0f / 255.0f));
}

TEST_CASE("RGB luminance for pure colors", "[RGB]") {
    RGB red{1.0f, 0.0f, 0.0f};
    RGB green{0.0f, 1.0f, 0.0f};
    RGB blue{0.0f, 0.0f, 1.0f};
    
    REQUIRE(math::approx_equal(red.luminance(), 0.2126f));
    REQUIRE(math::approx_equal(green.luminance(), 0.7152f));
    REQUIRE(math::approx_equal(blue.luminance(), 0.0722f));
}

TEST_CASE("RGB luminance for white and black", "[RGB]") {
    RGB white{1.0f, 1.0f, 1.0f};
    RGB black{0.0f, 0.0f, 0.0f};
    
    REQUIRE(math::approx_equal(white.luminance(), 1.0f));
    REQUIRE(math::approx_equal(black.luminance(), 0.0f));
}

TEST_CASE("RGB to_srgb converts to sRGB space", "[RGB]") {
    RGB color{0.5f, 0.5f, 0.5f};
    
    auto srgb = color.to_srgb();
    
    // Linear 0.5 should convert to sRGB ~0.735
    REQUIRE(srgb[0] > 0.7f);
    REQUIRE(srgb[0] < 0.75f);
    REQUIRE(math::approx_equal(srgb[0], srgb[1]));
    REQUIRE(math::approx_equal(srgb[1], srgb[2]));
}

TEST_CASE("RGB to_srgb black and white", "[RGB]") {
    RGB black{0.0f, 0.0f, 0.0f};
    RGB white{1.0f, 1.0f, 1.0f};
    
    auto srgb_black = black.to_srgb();
    auto srgb_white = white.to_srgb();
    
    REQUIRE(math::approx_equal(srgb_black[0], 0.0f));
    REQUIRE(math::approx_equal(srgb_black[1], 0.0f));
    REQUIRE(math::approx_equal(srgb_black[2], 0.0f));
    
    REQUIRE(math::approx_equal(srgb_white[0], 1.0f));
    REQUIRE(math::approx_equal(srgb_white[1], 1.0f));
    REQUIRE(math::approx_equal(srgb_white[2], 1.0f));
}

TEST_CASE("RGB is_standard returns true", "[RGB]") {
    REQUIRE(RGB::is_standard() == true);
}

// ========================================
// RGB8 TESTS (only class-specific functions)
// ========================================

TEST_CASE("RGB8 accessors R, G, B", "[RGB8]") {
    RGB8 color{128, 192, 255};
    
    REQUIRE(color.R() == 128);
    REQUIRE(color.G() == 192);
    REQUIRE(color.B() == 255);
    
    // Test mutability
    color.R() = 64;
    color.G() = 128;
    color.B() = 192;
    
    REQUIRE(color.R() == 64);
    REQUIRE(color.G() == 128);
    REQUIRE(color.B() == 192);
}

TEST_CASE("RGB8 from_srgb with float values", "[RGB8]") {
    auto color = RGB8::from_srgb(0.5f, 0.5f, 0.5f);
    
    // Should convert and round to uint8_t
    REQUIRE(color.R() > 0);
    REQUIRE(color.R() < 128);
}

TEST_CASE("RGB8 from_srgb with int values", "[RGB8]") {
    auto color = RGB8::from_srgb(255, 128, 64);
    
    REQUIRE(color.R() > 200);
    REQUIRE(color.G() > 0);
    REQUIRE(color.B() > 0);
}

TEST_CASE("RGB8 from_hex creates color from hex value", "[RGB8]") {
    auto color = RGB8::from_hex(0xFF0000);
    
    REQUIRE(color.R() == 255);
    REQUIRE(color.G() == 0);
    REQUIRE(color.B() == 0);
}

TEST_CASE("RGB8 from_hex blue", "[RGB8]") {
    auto color = RGB8::from_hex(0x0000FF);
    
    REQUIRE(color.R() == 0);
    REQUIRE(color.G() == 0);
    REQUIRE(color.B() == 255);
}

TEST_CASE("RGB8 from_hex arbitrary color", "[RGB8]") {
    auto color = RGB8::from_hex(0x8040C0);
    
    REQUIRE(color.R() == 0x80);
    REQUIRE(color.G() == 0x40);
    REQUIRE(color.B() == 0xC0);
}

TEST_CASE("RGB8 from_linear with int values", "[RGB8]") {
    auto color = RGB8::from_linear(255, 128, 64);
    
    REQUIRE(color.R() == 255);
    REQUIRE(color.G() == 128);
    REQUIRE(color.B() == 64);
}

TEST_CASE("RGB8 from_linear black and white", "[RGB8]") {
    auto black = RGB8::from_linear(0, 0, 0);
    auto white = RGB8::from_linear(255, 255, 255);
    
    REQUIRE(black.R() == 0);
    REQUIRE(black.G() == 0);
    REQUIRE(black.B() == 0);
    
    REQUIRE(white.R() == 255);
    REQUIRE(white.G() == 255);
    REQUIRE(white.B() == 255);
}

TEST_CASE("RGB8 luminance for pure colors", "[RGB8]") {
    RGB8 red{255, 0, 0};
    RGB8 green{0, 255, 0};
    RGB8 blue{0, 0, 255};
    
    REQUIRE(math::approx_equal(red.luminance(), 0.2126f));
    REQUIRE(math::approx_equal(green.luminance(), 0.7152f));
    REQUIRE(math::approx_equal(blue.luminance(), 0.0722f));
}

TEST_CASE("RGB8 luminance for white and black", "[RGB8]") {
    RGB8 white{255, 255, 255};
    RGB8 black{0, 0, 0};
    
    REQUIRE(math::approx_equal(white.luminance(), 1.0f));
    REQUIRE(math::approx_equal(black.luminance(), 0.0f));
}

TEST_CASE("RGB8 to_srgb converts to sRGB space", "[RGB8]") {
    RGB8 color{128, 128, 128};
    
    auto srgb = color.to_srgb();
    
    // Should convert properly
    REQUIRE(srgb[0] > 0.0f);
    REQUIRE(srgb[0] < 1.0f);
    REQUIRE(math::approx_equal(srgb[0], srgb[1]));
    REQUIRE(math::approx_equal(srgb[1], srgb[2]));
}

TEST_CASE("RGB8 to_srgb black and white", "[RGB8]") {
    RGB8 black{0, 0, 0};
    RGB8 white{255, 255, 255};
    
    auto srgb_black = black.to_srgb();
    auto srgb_white = white.to_srgb();
    
    REQUIRE(math::approx_equal(srgb_black[0], 0.0f));
    REQUIRE(math::approx_equal(srgb_white[0], 1.0f));
}

TEST_CASE("RGB8 is_standard returns true", "[RGB8]") {
    REQUIRE(RGB8::is_standard() == true);
}

// ========== SAMPLE MIXIN OPERATIONS ON RGB8 ==========
// Just a few to verify inheritance works, not full coverage

TEST_CASE("RGB8 basic addition works", "[RGB8]") {
    RGB8 color1{100, 150, 200};
    RGB8 color2{50, 50, 50};
    
    auto result = color1 + color2;
    
    REQUIRE(result.R() == 150);
    REQUIRE(result.G() == 200);
    REQUIRE(result.B() == 250);
}

TEST_CASE("RGB8 scalar multiplication works", "[RGB8]") {
    RGB8 color{10, 20, 30};
    
    auto result = color * 2;
    
    REQUIRE(result.R() == 20);
    REQUIRE(result.G() == 40);
    REQUIRE(result.B() == 60);
}

TEST_CASE("RGB8 clamp works with uint8_t", "[RGB8]") {
    RGB8 color{50, 150, 250};
    
    auto result = color.clamp(100, 200);
    
    REQUIRE(result.R() == 100);
    REQUIRE(result.G() == 150);
    REQUIRE(result.B() == 200);
}