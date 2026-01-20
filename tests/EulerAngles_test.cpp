#define NDEBUG
#include <catch2/catch_test_macros.hpp>
#include "../ES_math.hpp"
#include "../Angle.hpp"
#include "../EulerAngles.hpp"

using namespace ES;

TEST_CASE("EulerAngles zero constructor", "[EulerAngles]"){
    EulerAngles<in_radians, float> angles = EulerAngles<in_radians,float>::zero();
    
    REQUIRE(angles.yaw().get() == 0.0f);
    REQUIRE(angles.pitch().get() == 0.0f);
    REQUIRE(angles.roll().get() == 0.0f);
}

TEST_CASE("EulerAngles constructor with values", "[EulerAngles]"){
    Angle<in_radians, float> y(1.0f);
    Angle<in_radians, float> p(2.0f);
    Angle<in_radians, float> r(3.0f);
    
    EulerAngles<in_radians, float> angles(y, p, r);
    
    REQUIRE(angles.yaw().get() == 1.0f);
    REQUIRE(angles.pitch().get() == 2.0f);
    REQUIRE(angles.roll().get() == 3.0f);
}

TEST_CASE("EulerAngles yaw accessor", "[EulerAngles]"){
    Angle<in_radians, float> y(1.5f);
    Angle<in_radians, float> p(0.0f);
    Angle<in_radians, float> r(0.0f);
    
    EulerAngles<in_radians, float> angles(y, p, r);
    
    REQUIRE(angles.yaw().get() == 1.5f);
}

TEST_CASE("EulerAngles pitch accessor", "[EulerAngles]"){
    Angle<in_radians, float> y(0.0f);
    Angle<in_radians, float> p(2.5f);
    Angle<in_radians, float> r(0.0f);
    
    EulerAngles<in_radians, float> angles(y, p, r);
    
    REQUIRE(angles.pitch().get() == 2.5f);
}

TEST_CASE("EulerAngles roll accessor", "[EulerAngles]"){
    Angle<in_radians, float> y(0.0f);
    Angle<in_radians, float> p(0.0f);
    Angle<in_radians, float> r(3.5f);
    
    EulerAngles<in_radians, float> angles(y, p, r);
    
    REQUIRE(angles.roll().get() == 3.5f);
}

TEST_CASE("EulerAngles yaw mutable accessor", "[EulerAngles]"){
    EulerAngles<in_radians, float> angles;
    
    angles.yaw() = Angle<in_radians, float>(1.0f);
    
    REQUIRE(angles.yaw().get() == 1.0f);
}

TEST_CASE("EulerAngles pitch mutable accessor", "[EulerAngles]"){
    EulerAngles<in_radians, float> angles;
    
    angles.pitch() = Angle<in_radians, float>(2.0f);
    
    REQUIRE(angles.pitch().get() == 2.0f);
}

TEST_CASE("EulerAngles roll mutable accessor", "[EulerAngles]"){
    EulerAngles<in_radians, float> angles;
    
    angles.roll() = Angle<in_radians, float>(3.0f);
    
    REQUIRE(angles.roll().get() == 3.0f);
}

TEST_CASE("EulerAngles normalize", "[EulerAngles]"){
    Angle<in_radians, float> y(4.0f * math::pi<float>);
    Angle<in_radians, float> p(3.0f * math::pi<float>);
    Angle<in_radians, float> r(5.0f * math::pi<float>);
    
    EulerAngles<in_radians, float> angles(y, p, r);
    auto normalized = angles.normalize();
    
    REQUIRE(normalized.yaw().get() >= -math::pi<float>);
    REQUIRE(normalized.yaw().get() <= math::pi<float>);
    REQUIRE(normalized.pitch().get() >= -math::pi<float>);
    REQUIRE(normalized.pitch().get() <= math::pi<float>);
    REQUIRE(normalized.roll().get() >= -math::pi<float>);
    REQUIRE(normalized.roll().get() <= math::pi<float>);
}

TEST_CASE("EulerAngles normalize_in_place", "[EulerAngles]"){
    Angle<in_radians, float> y(4.0f * math::pi<float>);
    Angle<in_radians, float> p(3.0f * math::pi<float>);
    Angle<in_radians, float> r(5.0f * math::pi<float>);
    
    EulerAngles<in_radians, float> angles(y, p, r);
    angles.normalize_in_place();
    
    REQUIRE(angles.yaw().get() >= -math::pi<float>);
    REQUIRE(angles.yaw().get() <= math::pi<float>);
    REQUIRE(angles.pitch().get() >= -math::pi<float>);
    REQUIRE(angles.pitch().get() <= math::pi<float>);
    REQUIRE(angles.roll().get() >= -math::pi<float>);
    REQUIRE(angles.roll().get() <= math::pi<float>);
}

TEST_CASE("EulerAngles canonicalize", "[EulerAngles]"){
    Angle<in_radians, float> y(4.0f * math::pi<float>);
    Angle<in_radians, float> p(2.0f * math::pi<float>);
    Angle<in_radians, float> r(5.0f * math::pi<float>);
    
    EulerAngles<in_radians, float> angles(y, p, r);
    auto canonical = angles.canonicalize();
    
    REQUIRE(canonical.pitch().get() >= -math::half_pi<float>);
    REQUIRE(canonical.pitch().get() <= math::half_pi<float>);
    REQUIRE(canonical.yaw().get() >= -math::pi<float>);
    REQUIRE(canonical.yaw().get() <= math::pi<float>);
}

TEST_CASE("EulerAngles canonicalize_in_place", "[EulerAngles]"){
    Angle<in_radians, float> y(4.0f * math::pi<float>);
    Angle<in_radians, float> p(2.0f * math::pi<float>);
    Angle<in_radians, float> r(5.0f * math::pi<float>);
    
    EulerAngles<in_radians, float> angles(y, p, r);
    angles.canonicalize_in_place();
    
    REQUIRE(angles.pitch().get() >= -math::half_pi<float>);
    REQUIRE(angles.pitch().get() <= math::half_pi<float>);
    REQUIRE(angles.yaw().get() >= -math::pi<float>);
    REQUIRE(angles.yaw().get() <= math::pi<float>);
}

TEST_CASE("EulerAngles sin_yaw", "[EulerAngles]"){
    Angle<in_radians, float> y(math::half_pi<float>);
    Angle<in_radians, float> p(0.0f);
    Angle<in_radians, float> r(0.0f);
    
    EulerAngles<in_radians, float> angles(y, p, r);
    
    REQUIRE(math::approx_equal(angles.sin_yaw(), 1.0f));
}

TEST_CASE("EulerAngles cos_yaw", "[EulerAngles]"){
    Angle<in_radians, float> y(0.0f);
    Angle<in_radians, float> p(0.0f);
    Angle<in_radians, float> r(0.0f);
    
    EulerAngles<in_radians, float> angles(y, p, r);
    
    REQUIRE(math::approx_equal(angles.cos_yaw(), 1.0f));
}

TEST_CASE("EulerAngles sin_pitch", "[EulerAngles]"){
    Angle<in_radians, float> y(0.0f);
    Angle<in_radians, float> p(math::half_pi<float>);
    Angle<in_radians, float> r(0.0f);
    
    EulerAngles<in_radians, float> angles(y, p, r);
    
    REQUIRE(math::approx_equal(angles.sin_pitch(), 1.0f));
}

TEST_CASE("EulerAngles cos_pitch", "[EulerAngles]"){
    Angle<in_radians, float> y(0.0f);
    Angle<in_radians, float> p(0.0f);
    Angle<in_radians, float> r(0.0f);
    
    EulerAngles<in_radians, float> angles(y, p, r);
    
    REQUIRE(math::approx_equal(angles.cos_pitch(), 1.0f));
}

TEST_CASE("EulerAngles sin_roll", "[EulerAngles]"){
    Angle<in_radians, float> y(0.0f);
    Angle<in_radians, float> p(0.0f);
    Angle<in_radians, float> r(math::half_pi<float>);
    
    EulerAngles<in_radians, float> angles(y, p, r);
    
    REQUIRE(math::approx_equal(angles.sin_roll(), 1.0f));
}

TEST_CASE("EulerAngles cos_roll", "[EulerAngles]"){
    Angle<in_radians, float> y(0.0f);
    Angle<in_radians, float> p(0.0f);
    Angle<in_radians, float> r(0.0f);
    
    EulerAngles<in_radians, float> angles(y, p, r);
    
    REQUIRE(math::approx_equal(angles.cos_roll(), 1.0f));
}

TEST_CASE("EulerAngles component addition", "[EulerAngles]"){
    Angle<in_radians, float> y1(1.0f);
    Angle<in_radians, float> p1(2.0f);
    Angle<in_radians, float> r1(3.0f);
    
    Angle<in_radians, float> y2(0.5f);
    Angle<in_radians, float> p2(1.0f);
    Angle<in_radians, float> r2(1.5f);
    
    EulerAngles<in_radians, float> angles1(y1, p1, r1);
    EulerAngles<in_radians, float> angles2(y2, p2, r2);
    
    auto result = angles1 + angles2;
    
    REQUIRE(math::approx_equal(result.yaw().get(), 1.5f));
    REQUIRE(math::approx_equal(result.pitch().get(), 3.0f));
    REQUIRE(math::approx_equal(result.roll().get(), 4.5f));
}

TEST_CASE("EulerAngles component subtraction", "[EulerAngles]"){
    Angle<in_radians, float> y1(2.0f);
    Angle<in_radians, float> p1(3.0f);
    Angle<in_radians, float> r1(4.0f);
    
    Angle<in_radians, float> y2(1.0f);
    Angle<in_radians, float> p2(1.0f);
    Angle<in_radians, float> r2(1.0f);
    
    EulerAngles<in_radians, float> angles1(y1, p1, r1);
    EulerAngles<in_radians, float> angles2(y2, p2, r2);
    
    auto result = angles1 - angles2;
    
    REQUIRE(math::approx_equal(result.yaw().get(), 1.0f));
    REQUIRE(math::approx_equal(result.pitch().get(), 2.0f));
    REQUIRE(math::approx_equal(result.roll().get(), 3.0f));
}

TEST_CASE("EulerAngles scalar multiplication", "[EulerAngles]"){
    Angle<in_radians, float> y(1.0f);
    Angle<in_radians, float> p(2.0f);
    Angle<in_radians, float> r(3.0f);
    
    EulerAngles<in_radians, float> angles(y, p, r);
    
    auto result = angles * 2.0f;
    
    REQUIRE(math::approx_equal(result.yaw().get(), 2.0f));
    REQUIRE(math::approx_equal(result.pitch().get(), 4.0f));
    REQUIRE(math::approx_equal(result.roll().get(), 6.0f));
}

TEST_CASE("EulerAngles scalar division", "[EulerAngles]"){
    Angle<in_radians, float> y(2.0f);
    Angle<in_radians, float> p(4.0f);
    Angle<in_radians, float> r(6.0f);
    
    EulerAngles<in_radians, float> angles(y, p, r);
    
    auto result = angles / 2.0f;
    
    REQUIRE(math::approx_equal(result.yaw().get(), 1.0f));
    REQUIRE(math::approx_equal(result.pitch().get(), 2.0f));
    REQUIRE(math::approx_equal(result.roll().get(), 3.0f));
}

TEST_CASE("EulerAngles negate", "[EulerAngles]"){
    Angle<in_radians, float> y(1.0f);
    Angle<in_radians, float> p(2.0f);
    Angle<in_radians, float> r(3.0f);
    
    EulerAngles<in_radians, float> angles(y, p, r);
    
    auto result = -angles;
    
    REQUIRE(math::approx_equal(result.yaw().get(), -1.0f));
    REQUIRE(math::approx_equal(result.pitch().get(), -2.0f));
    REQUIRE(math::approx_equal(result.roll().get(), -3.0f));
}

TEST_CASE("EulerAngles lerp", "[EulerAngles]"){
    Angle<in_radians, float> y1(0.0f);
    Angle<in_radians, float> p1(0.0f);
    Angle<in_radians, float> r1(0.0f);
    
    Angle<in_radians, float> y2(2.0f);
    Angle<in_radians, float> p2(4.0f);
    Angle<in_radians, float> r2(6.0f);
    
    EulerAngles<in_radians, float> angles1(y1, p1, r1);
    EulerAngles<in_radians, float> angles2(y2, p2, r2);
    
    auto result = angles1.lerp(angles2, 0.5f);
    
    REQUIRE(math::approx_equal(result.yaw().get(), 1.0f));
    REQUIRE(math::approx_equal(result.pitch().get(), 2.0f));
    REQUIRE(math::approx_equal(result.roll().get(), 3.0f));
}

TEST_CASE("EulerAngles lerp_in_place", "[EulerAngles]"){
    Angle<in_radians, float> y1(0.0f);
    Angle<in_radians, float> p1(0.0f);
    Angle<in_radians, float> r1(0.0f);
    
    Angle<in_radians, float> y2(2.0f);
    Angle<in_radians, float> p2(4.0f);
    Angle<in_radians, float> r2(6.0f);
    
    EulerAngles<in_radians, float> angles1(y1, p1, r1);
    EulerAngles<in_radians, float> angles2(y2, p2, r2);
    
    angles1.lerp_in_place(angles2, 0.5f);
    
    REQUIRE(math::approx_equal(angles1.yaw().get(), 1.0f));
    REQUIRE(math::approx_equal(angles1.pitch().get(), 2.0f));
    REQUIRE(math::approx_equal(angles1.roll().get(), 3.0f));
}

TEST_CASE("EulerAngles clamp", "[EulerAngles]"){
    Angle<in_radians, float> y(-5.0f);
    Angle<in_radians, float> p(10.0f);
    Angle<in_radians, float> r(3.0f);
    
    EulerAngles<in_radians, float> angles(y, p, r);
    
    Angle<in_radians, float> min_angle(0.0f);
    Angle<in_radians, float> max_angle(5.0f);
    
    auto result = angles.clamp(min_angle, max_angle);
    
    REQUIRE(math::approx_equal(result.yaw().get(), 0.0f));
    REQUIRE(math::approx_equal(result.pitch().get(), 5.0f));
    REQUIRE(math::approx_equal(result.roll().get(), 3.0f));
}

TEST_CASE("EulerAngles clamp_in_place", "[EulerAngles]"){
    Angle<in_radians, float> y(-5.0f);
    Angle<in_radians, float> p(10.0f);
    Angle<in_radians, float> r(3.0f);
    
    EulerAngles<in_radians, float> angles(y, p, r);
    
    Angle<in_radians, float> min_angle(0.0f);
    Angle<in_radians, float> max_angle(5.0f);
    
    angles.clamp_in_place(min_angle, max_angle);
    
    REQUIRE(math::approx_equal(angles.yaw().get(), 0.0f));
    REQUIRE(math::approx_equal(angles.pitch().get(), 5.0f));
    REQUIRE(math::approx_equal(angles.roll().get(), 3.0f));
}

TEST_CASE("EulerAngles in_degree default constructor", "[EulerAngles]"){
    EulerAngles<in_degrees, float> angles;
    
    REQUIRE(angles.yaw().get() == 0.0f);
    REQUIRE(angles.pitch().get() == 0.0f);
    REQUIRE(angles.roll().get() == 0.0f);
}

TEST_CASE("EulerAngles in_degrees constructor with values", "[EulerAngles]"){
    Angle<in_degrees, float> y(45.0f);
    Angle<in_degrees, float> p(90.0f);
    Angle<in_degrees, float> r(180.0f);
    
    EulerAngles<in_degrees, float> angles(y, p, r);
    
    REQUIRE(angles.yaw().get() == 45.0f);
    REQUIRE(angles.pitch().get() == 90.0f);
    REQUIRE(angles.roll().get() == 180.0f);
}

TEST_CASE("EulerAngles equality", "[EulerAngles]"){
    Angle<in_radians, float> y(1.0f);
    Angle<in_radians, float> p(2.0f);
    Angle<in_radians, float> r(3.0f);
    
    EulerAngles<in_radians, float> angles1(y, p, r);
    EulerAngles<in_radians, float> angles2(y, p, r);
    
    REQUIRE(angles1 == angles2);
}

TEST_CASE("EulerAngles component addition in place", "[EulerAngles]"){
    Angle<in_radians, float> y1(1.0f);
    Angle<in_radians, float> p1(2.0f);
    Angle<in_radians, float> r1(3.0f);
    
    Angle<in_radians, float> y2(0.5f);
    Angle<in_radians, float> p2(1.0f);
    Angle<in_radians, float> r2(1.5f);
    
    EulerAngles<in_radians, float> angles1(y1, p1, r1);
    EulerAngles<in_radians, float> angles2(y2, p2, r2);
    
    angles1 += angles2;
    
    REQUIRE(math::approx_equal(angles1.yaw().get(), 1.5f));
    REQUIRE(math::approx_equal(angles1.pitch().get(), 3.0f));
    REQUIRE(math::approx_equal(angles1.roll().get(), 4.5f));
}

TEST_CASE("EulerAngles component subtraction in place", "[EulerAngles]"){
    Angle<in_radians, float> y1(2.0f);
    Angle<in_radians, float> p1(3.0f);
    Angle<in_radians, float> r1(4.0f);
    
    Angle<in_radians, float> y2(1.0f);
    Angle<in_radians, float> p2(1.0f);
    Angle<in_radians, float> r2(1.0f);
    
    EulerAngles<in_radians, float> angles1(y1, p1, r1);
    EulerAngles<in_radians, float> angles2(y2, p2, r2);
    
    angles1 -= angles2;
    
    REQUIRE(math::approx_equal(angles1.yaw().get(), 1.0f));
    REQUIRE(math::approx_equal(angles1.pitch().get(), 2.0f));
    REQUIRE(math::approx_equal(angles1.roll().get(), 3.0f));
}

TEST_CASE("EulerAngles scalar multiplication in place", "[EulerAngles]"){
    Angle<in_radians, float> y(1.0f);
    Angle<in_radians, float> p(2.0f);
    Angle<in_radians, float> r(3.0f);
    
    EulerAngles<in_radians, float> angles(y, p, r);
    
    angles *= 2.0f;
    
    REQUIRE(math::approx_equal(angles.yaw().get(), 2.0f));
    REQUIRE(math::approx_equal(angles.pitch().get(), 4.0f));
    REQUIRE(math::approx_equal(angles.roll().get(), 6.0f));
}

TEST_CASE("EulerAngles scalar division in place", "[EulerAngles]"){
    Angle<in_radians, float> y(2.0f);
    Angle<in_radians, float> p(4.0f);
    Angle<in_radians, float> r(6.0f);
    
    EulerAngles<in_radians, float> angles(y, p, r);
    
    angles /= 2.0f;
    
    REQUIRE(math::approx_equal(angles.yaw().get(), 1.0f));
    REQUIRE(math::approx_equal(angles.pitch().get(), 2.0f));
    REQUIRE(math::approx_equal(angles.roll().get(), 3.0f));
}