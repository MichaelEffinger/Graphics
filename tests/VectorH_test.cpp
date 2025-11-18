#define NDEBUG
#include <catch2/catch_test_macros.hpp>
#include "../ES_math.hpp"
#include "../VectorH.hpp"

using namespace ES;

TEST_CASE("VectorH accessors x, y, z, w", "[VectorH]") {
    VectorH<float> v{1.0f, 2.0f, 3.0f, 4.0f};
    
    REQUIRE(math::approx_equal(v.x(), 1.0f));
    REQUIRE(math::approx_equal(v.y(), 2.0f));
    REQUIRE(math::approx_equal(v.z(), 3.0f));
    REQUIRE(math::approx_equal(v.w(), 4.0f));
    
    v.x() = 10.0f;
    v.y() = 20.0f;
    v.z() = 30.0f;
    v.w() = 40.0f;
    
    REQUIRE(math::approx_equal(v.x(), 10.0f));
    REQUIRE(math::approx_equal(v.y(), 20.0f));
    REQUIRE(math::approx_equal(v.z(), 30.0f));
    REQUIRE(math::approx_equal(v.w(), 40.0f));
}


TEST_CASE("VectorH operator+ with two directions (w=0)", "[VectorH]") {
    VectorH<float> dir1{1.0f, 2.0f, 3.0f, 0.0f};
    VectorH<float> dir2{4.0f, 5.0f, 6.0f, 0.0f};
    
    auto result = dir1 + dir2;
    
    REQUIRE(math::approx_equal(result.x(), 5.0f));
    REQUIRE(math::approx_equal(result.y(), 7.0f));
    REQUIRE(math::approx_equal(result.z(), 9.0f));
    REQUIRE(math::approx_equal(result.w(), 0.0f));
}

TEST_CASE("VectorH operator+ with point and direction", "[VectorH]") {
    VectorH<float> point{1.0f, 2.0f, 3.0f, 1.0f};
    VectorH<float> direction{4.0f, 5.0f, 6.0f, 0.0f};
    
    auto result = point + direction;
    
    REQUIRE(math::approx_equal(result.x(), 5.0f));
    REQUIRE(math::approx_equal(result.y(), 7.0f));
    REQUIRE(math::approx_equal(result.z(), 9.0f));
    REQUIRE(math::approx_equal(result.w(), 1.0f));
}

TEST_CASE("VectorH operator+= with directions", "[VectorH]") {
    VectorH<float> dir1{1.0f, 2.0f, 3.0f, 0.0f};
    VectorH<float> dir2{4.0f, 5.0f, 6.0f, 0.0f};
    
    dir1 += dir2;
    
    REQUIRE(math::approx_equal(dir1.x(), 5.0f));
    REQUIRE(math::approx_equal(dir1.y(), 7.0f));
    REQUIRE(math::approx_equal(dir1.z(), 9.0f));
    REQUIRE(math::approx_equal(dir1.w(), 0.0f));
}


TEST_CASE("VectorH operator- with two points (returns direction)", "[VectorH]") {
    VectorH<float> point1{5.0f, 7.0f, 9.0f, 1.0f};
    VectorH<float> point2{1.0f, 2.0f, 3.0f, 1.0f};
    
    auto result = point1 - point2;
    
    REQUIRE(math::approx_equal(result.x(), 4.0f));
    REQUIRE(math::approx_equal(result.y(), 5.0f));
    REQUIRE(math::approx_equal(result.z(), 6.0f));
    REQUIRE(math::approx_equal(result.w(), 0.0f));
}

TEST_CASE("VectorH operator- with point and direction", "[VectorH]") {
    VectorH<float> point{5.0f, 7.0f, 9.0f, 1.0f};
    VectorH<float> direction{1.0f, 2.0f, 3.0f, 0.0f};
    
    auto result = point - direction;
    
    REQUIRE(math::approx_equal(result.x(), 4.0f));
    REQUIRE(math::approx_equal(result.y(), 5.0f));
    REQUIRE(math::approx_equal(result.z(), 6.0f));
    REQUIRE(math::approx_equal(result.w(), 1.0f));
}

TEST_CASE("VectorH operator-= with directions", "[VectorH]") {
    VectorH<float> dir1{5.0f, 7.0f, 9.0f, 0.0f};
    VectorH<float> dir2{1.0f, 2.0f, 3.0f, 0.0f};
    
    dir1 -= dir2;
    
    REQUIRE(math::approx_equal(dir1.x(), 4.0f));
    REQUIRE(math::approx_equal(dir1.y(), 5.0f));
    REQUIRE(math::approx_equal(dir1.z(), 6.0f));
    REQUIRE(math::approx_equal(dir1.w(), 0.0f));
}

TEST_CASE("VectorH operator* with scalar", "[VectorH]") {
    VectorH<float> v{2.0f, 3.0f, 4.0f, 1.0f};
    
    auto result = v * 2.0f;
    
    REQUIRE(math::approx_equal(result.x(), 4.0f));
    REQUIRE(math::approx_equal(result.y(), 6.0f));
    REQUIRE(math::approx_equal(result.z(), 8.0f));
    REQUIRE(math::approx_equal(result.w(), 2.0f));
}

TEST_CASE("VectorH operator*= with scalar", "[VectorH]") {
    VectorH<float> v{2.0f, 3.0f, 4.0f, 0.0f};
    
    v *= 3.0f;
    
    REQUIRE(math::approx_equal(v.x(), 6.0f));
    REQUIRE(math::approx_equal(v.y(), 9.0f));
    REQUIRE(math::approx_equal(v.z(), 12.0f));
    REQUIRE(math::approx_equal(v.w(), 0.0f));
}

TEST_CASE("VectorH operator/ with scalar", "[VectorH]") {
    VectorH<float> v{6.0f, 9.0f, 12.0f, 3.0f};
    
    auto result = v / 3.0f;
    
    REQUIRE(math::approx_equal(result.x(), 2.0f));
    REQUIRE(math::approx_equal(result.y(), 3.0f));
    REQUIRE(math::approx_equal(result.z(), 4.0f));
    REQUIRE(math::approx_equal(result.w(), 1.0f));
}

TEST_CASE("VectorH operator/= with scalar", "[VectorH]") {
    VectorH<float> v{6.0f, 9.0f, 12.0f, 0.0f};
    
    v /= 3.0f;
    
    REQUIRE(math::approx_equal(v.x(), 2.0f));
    REQUIRE(math::approx_equal(v.y(), 3.0f));
    REQUIRE(math::approx_equal(v.z(), 4.0f));
    REQUIRE(math::approx_equal(v.w(), 0.0f));
}

#ifdef NDEBUG
TEST_CASE("VectorH operator/ division by zero", "[VectorH]") {
    VectorH<float> v{6.0f, 9.0f, 12.0f, 1.0f};
    
    auto result = v / 0.0f;
    
    REQUIRE(math::approx_equal(result.x(), 0.0f));
    REQUIRE(math::approx_equal(result.y(), 0.0f));
    REQUIRE(math::approx_equal(result.z(), 0.0f));
    REQUIRE(math::approx_equal(result.w(), 1.0f)); 
}

TEST_CASE("VectorH operator/= division by zero", "[VectorH]") {
    VectorH<float> v{6.0f, 9.0f, 12.0f, 1.0f};
    
    v /= 0.0f;
    
    REQUIRE(math::approx_equal(v.x(), 0.0f));
    REQUIRE(math::approx_equal(v.y(), 0.0f));
    REQUIRE(math::approx_equal(v.z(), 0.0f));
    REQUIRE(math::approx_equal(v.w(), 1.0f));
}
#endif

TEST_CASE("VectorH hadamardProduct with directions", "[VectorH]") {
    VectorH<float> dir1{2.0f, 3.0f, 4.0f, 0.0f};
    VectorH<float> dir2{5.0f, 6.0f, 7.0f, 0.0f};
    
    auto result = dir1.hadamardProduct(dir2);
    
    REQUIRE(math::approx_equal(result.x(), 10.0f));
    REQUIRE(math::approx_equal(result.y(), 18.0f));
    REQUIRE(math::approx_equal(result.z(), 28.0f));
    REQUIRE(math::approx_equal(result.w(), 0.0f));
}

TEST_CASE("VectorH hadamardProduct_in_place with directions", "[VectorH]") {
    VectorH<float> dir1{2.0f, 3.0f, 4.0f, 0.0f};
    VectorH<float> dir2{5.0f, 6.0f, 7.0f, 0.0f};
    
    dir1.hadamardProduct_in_place(dir2);
    
    REQUIRE(math::approx_equal(dir1.x(), 10.0f));
    REQUIRE(math::approx_equal(dir1.y(), 18.0f));
    REQUIRE(math::approx_equal(dir1.z(), 28.0f));
    REQUIRE(math::approx_equal(dir1.w(), 0.0f));
}


TEST_CASE("VectorH hadamardDivide with directions", "[VectorH]") {
    VectorH<float> dir1{10.0f, 18.0f, 28.0f, 0.0f};
    VectorH<float> dir2{2.0f, 3.0f, 4.0f, 0.0f};
    
    auto result = dir1.hadamardDivide(dir2);
    
    REQUIRE(math::approx_equal(result.x(), 5.0f));
    REQUIRE(math::approx_equal(result.y(), 6.0f));
    REQUIRE(math::approx_equal(result.z(), 7.0f));
    REQUIRE(math::approx_equal(result.w(), 0.0f));
}

TEST_CASE("VectorH hadamardDivide_in_place with directions", "[VectorH]") {
    VectorH<float> dir1{10.0f, 18.0f, 28.0f, 0.0f};
    VectorH<float> dir2{2.0f, 3.0f, 4.0f, 0.0f};
    
    dir1.hadamardDivide_in_place(dir2);
    
    REQUIRE(math::approx_equal(dir1.x(), 5.0f));
    REQUIRE(math::approx_equal(dir1.y(), 6.0f));
    REQUIRE(math::approx_equal(dir1.z(), 7.0f));
    REQUIRE(math::approx_equal(dir1.w(), 0.0f));
}


TEST_CASE("VectorH dot product with directions", "[VectorH]") {
    VectorH<float> dir1{1.0f, 2.0f, 3.0f, 0.0f};
    VectorH<float> dir2{4.0f, 5.0f, 6.0f, 0.0f};
    
    float result = dir1.dot(dir2);
    
    REQUIRE(math::approx_equal(result, 32.0f));
}

TEST_CASE("VectorH dot product with perpendicular directions", "[VectorH]") {
    VectorH<float> dir1{1.0f, 0.0f, 0.0f, 0.0f};
    VectorH<float> dir2{0.0f, 1.0f, 0.0f, 0.0f};
    
    float result = dir1.dot(dir2);
    
    REQUIRE(math::approx_equal(result, 0.0f));
}


TEST_CASE("VectorH cross product with directions", "[VectorH]") {
    VectorH<float> dir1{1.0f, 0.0f, 0.0f, 0.0f};
    VectorH<float> dir2{0.0f, 1.0f, 0.0f, 0.0f};
    
    auto result = dir1.cross(dir2);
    
    REQUIRE(math::approx_equal(result.x(), 0.0f));
    REQUIRE(math::approx_equal(result.y(), 0.0f));
    REQUIRE(math::approx_equal(result.z(), 1.0f));
    REQUIRE(math::approx_equal(result.w(), 0.0f));
}

TEST_CASE("VectorH cross_in_place with directions", "[VectorH]") {
    VectorH<float> dir1{1.0f, 0.0f, 0.0f, 0.0f};
    VectorH<float> dir2{0.0f, 1.0f, 0.0f, 0.0f};
    
    dir1.cross_in_place(dir2);
    
    REQUIRE(math::approx_equal(dir1.x(), 0.0f));
    REQUIRE(math::approx_equal(dir1.y(), 0.0f));
    REQUIRE(math::approx_equal(dir1.z(), 1.0f));
    REQUIRE(math::approx_equal(dir1.w(), 0.0f));
}

TEST_CASE("VectorH magnitude with direction", "[VectorH]") {
    VectorH<float> dir{3.0f, 4.0f, 0.0f, 0.0f};
    
    float mag = dir.magnitude();
    
    REQUIRE(math::approx_equal(mag, 5.0f));
}

TEST_CASE("VectorH magnitudeSquared with direction", "[VectorH]") {
    VectorH<float> dir{3.0f, 4.0f, 0.0f, 0.0f};
    
    float magSq = dir.magnitudeSquared();
    
    REQUIRE(math::approx_equal(magSq, 25.0f));
}

TEST_CASE("VectorH distance between two points", "[VectorH]") {
    VectorH<float> point1{0.0f, 0.0f, 0.0f, 1.0f};
    VectorH<float> point2{3.0f, 4.0f, 0.0f, 1.0f};
    
    float dist = point1.distance(point2);
    
    REQUIRE(math::approx_equal(dist, 5.0f));
}

TEST_CASE("VectorH distance_squared between two points", "[VectorH]") {
    VectorH<float> point1{0.0f, 0.0f, 0.0f, 1.0f};
    VectorH<float> point2{3.0f, 4.0f, 0.0f, 1.0f};
    
    float distSq = point1.distance_squared(point2);
    
    REQUIRE(math::approx_equal(distSq, 25.0f));
}


TEST_CASE("VectorH normalize direction", "[VectorH]") {
    VectorH<float> dir{3.0f, 4.0f, 0.0f, 0.0f};
    
    auto result = dir.normalize();
    
    REQUIRE(math::approx_equal(result.magnitude(), 1.0f));
    REQUIRE(math::approx_equal(result.x(), 0.6f));
    REQUIRE(math::approx_equal(result.y(), 0.8f));
    REQUIRE(math::approx_equal(result.z(), 0.0f));
    REQUIRE(math::approx_equal(result.w(), 0.0f));
}

TEST_CASE("VectorH normalize_in_place direction", "[VectorH]") {
    VectorH<float> dir{3.0f, 4.0f, 0.0f, 0.0f};
    
    dir.normalize_in_place();
    
    REQUIRE(math::approx_equal(dir.magnitude(), 1.0f));
    REQUIRE(math::approx_equal(dir.x(), 0.6f));
    REQUIRE(math::approx_equal(dir.y(), 0.8f));
    REQUIRE(math::approx_equal(dir.z(), 0.0f));
    REQUIRE(math::approx_equal(dir.w(), 0.0f));
}

TEST_CASE("VectorH homogenize with w != 0", "[VectorH]") {
    VectorH<float> v{6.0f, 9.0f, 12.0f, 3.0f};
    
    auto result = v.homogenize();
    
    REQUIRE(math::approx_equal(result.x(), 2.0f));
    REQUIRE(math::approx_equal(result.y(), 3.0f));
    REQUIRE(math::approx_equal(result.z(), 4.0f));
    REQUIRE(math::approx_equal(result.w(), 1.0f));
}

TEST_CASE("VectorH homogenize with w = 0 (direction)", "[VectorH]") {
    VectorH<float> dir{6.0f, 9.0f, 12.0f, 0.0f};
    
    auto result = dir.homogenize();
    
    REQUIRE(math::approx_equal(result.x(), 6.0f));
    REQUIRE(math::approx_equal(result.y(), 9.0f));
    REQUIRE(math::approx_equal(result.z(), 12.0f));
    REQUIRE(math::approx_equal(result.w(), 0.0f));
}

TEST_CASE("VectorH homogenize_in_place with w != 0", "[VectorH]") {
    VectorH<float> v{6.0f, 9.0f, 12.0f, 3.0f};
    
    v.homogenize_in_place();
    
    REQUIRE(math::approx_equal(v.x(), 2.0f));
    REQUIRE(math::approx_equal(v.y(), 3.0f));
    REQUIRE(math::approx_equal(v.z(), 4.0f));
    REQUIRE(math::approx_equal(v.w(), 1.0f));
}

TEST_CASE("VectorH homogenize_in_place with w = 0", "[VectorH]") {
    VectorH<float> dir{6.0f, 9.0f, 12.0f, 0.0f};
    
    dir.homogenize_in_place();
    
    REQUIRE(math::approx_equal(dir.x(), 6.0f));
    REQUIRE(math::approx_equal(dir.y(), 9.0f));
    REQUIRE(math::approx_equal(dir.z(), 12.0f));
    REQUIRE(math::approx_equal(dir.w(), 0.0f));
}


TEST_CASE("VectorH slerp between two directions", "[VectorH]") {
    VectorH<float> dir1{1.0f, 0.0f, 0.0f, 0.0f};
    VectorH<float> dir2{0.0f, 1.0f, 0.0f, 0.0f};
    
    auto result = dir1.slerp(dir2, 0.5f);
    
    REQUIRE(result.x() > 0.0f);
    REQUIRE(result.y() > 0.0f);
    REQUIRE(math::approx_equal(result.w(), 0.0f));
}


TEST_CASE("VectorH reflect with unit normal", "[VectorH]") {
    VectorH<float> dir{1.0f, -1.0f, 0.0f, 0.0f};
    dir.normalize_in_place();
    VectorH<float> normal{0.0f, 1.0f, 0.0f, 0.0f};
    
    auto result = dir.reflect(normal);

    REQUIRE(result.y() > 0.0f); 
    REQUIRE(math::approx_equal(result.w(), 0.0f));
}

TEST_CASE("VectorH reflect_in_place with unit normal", "[VectorH]") {
    VectorH<float> dir{1.0f, -1.0f, 0.0f, 0.0f};
    dir.normalize_in_place();
    VectorH<float> normal{0.0f, 1.0f, 0.0f, 0.0f};
    
    dir.reflect_in_place(normal);
    
    REQUIRE(dir.y() > 0.0f);
    REQUIRE(math::approx_equal(dir.w(), 0.0f));
}

TEST_CASE("VectorH reflect_safe with non-unit normal", "[VectorH]") {
    VectorH<float> dir{1.0f, -1.0f, 0.0f, 0.0f};
    VectorH<float> normal{0.0f, 5.0f, 0.0f, 0.0f};
    
    auto result = dir.reflect_safe(normal);
    
    REQUIRE(result.magnitude() > 0.0f);
    REQUIRE(math::approx_equal(result.w(), 0.0f));
}

TEST_CASE("VectorH reflect_in_place_safe with non-unit normal", "[VectorH]") {
    VectorH<float> dir{1.0f, -1.0f, 0.0f, 0.0f};
    VectorH<float> normal{0.0f, 5.0f, 0.0f, 0.0f}; 
    
    dir.reflect_in_place_safe(normal);
    
    REQUIRE(dir.magnitude() > 0.0f);
    REQUIRE(math::approx_equal(dir.w(), 0.0f));
}


TEST_CASE("VectorH lerp between two directions", "[VectorH]") {
    VectorH<float> dir1{0.0f, 0.0f, 0.0f, 0.0f};
    VectorH<float> dir2{10.0f, 20.0f, 30.0f, 0.0f};
    
    auto result = dir1.lerp(dir2, 0.5f);
    
    REQUIRE(math::approx_equal(result.x(), 5.0f));
    REQUIRE(math::approx_equal(result.y(), 10.0f));
    REQUIRE(math::approx_equal(result.z(), 15.0f));
    REQUIRE(math::approx_equal(result.w(), 0.0f));
}

TEST_CASE("VectorH lerp between two points", "[VectorH]") {
    VectorH<float> point1{0.0f, 0.0f, 0.0f, 1.0f};
    VectorH<float> point2{10.0f, 20.0f, 30.0f, 1.0f};
    
    auto result = point1.lerp(point2, 0.25f);
    
    REQUIRE(math::approx_equal(result.x(), 2.5f));
    REQUIRE(math::approx_equal(result.y(), 5.0f));
    REQUIRE(math::approx_equal(result.z(), 7.5f));
    REQUIRE(math::approx_equal(result.w(), 1.0f));
}

TEST_CASE("VectorH lerp_in_place between directions", "[VectorH]") {
    VectorH<float> dir1{0.0f, 0.0f, 0.0f, 0.0f};
    VectorH<float> dir2{10.0f, 20.0f, 30.0f, 0.0f};
    
    dir1.lerp_in_place(dir2, 0.75f);
    
    REQUIRE(math::approx_equal(dir1.x(), 7.5f));
    REQUIRE(math::approx_equal(dir1.y(), 15.0f));
    REQUIRE(math::approx_equal(dir1.z(), 22.5f));
    REQUIRE(math::approx_equal(dir1.w(), 0.0f));
}


TEST_CASE("VectorH clamp values", "[VectorH]") {
    VectorH<float> v{-5.0f, 0.5f, 10.0f, 1.0f};
    
    auto result = v.clamp(0.0f, 5.0f);
    
    REQUIRE(math::approx_equal(result.x(), 0.0f));
    REQUIRE(math::approx_equal(result.y(), 0.5f));
    REQUIRE(math::approx_equal(result.z(), 5.0f));
    REQUIRE(math::approx_equal(result.w(), 1.0f));
}

TEST_CASE("VectorH clamp_in_place values", "[VectorH]") {
    VectorH<float> v{-5.0f, 0.5f, 10.0f, 0.0f};
    
    v.clamp_in_place(-1.0f, 1.0f);
    
    REQUIRE(math::approx_equal(v.x(), -1.0f));
    REQUIRE(math::approx_equal(v.y(), 0.5f));
    REQUIRE(math::approx_equal(v.z(), 1.0f));
    REQUIRE(math::approx_equal(v.w(), 0.0f));
}


TEST_CASE("VectorH refract between media", "[VectorH]") {
    VectorH<float> incident{0.0f, -1.0f, 0.0f, 0.0f};
    VectorH<float> normal{0.0f, 1.0f, 0.0f, 0.0f};
    
    auto result = incident.refract(normal, 1.0f, 1.5f);
    
    REQUIRE(result.magnitude() >= 0.0f);
    REQUIRE(math::approx_equal(result.w(), 0.0f));
}

TEST_CASE("VectorH refract_in_place between media", "[VectorH]") {
    VectorH<float> incident{0.0f, -1.0f, 0.0f, 0.0f};
    VectorH<float> normal{0.0f, 1.0f, 0.0f, 0.0f};
    
    incident.refract_in_place(normal, 1.0f, 1.5f);
    
    REQUIRE(incident.magnitude() >= 0.0f);
    REQUIRE(math::approx_equal(incident.w(), 0.0f));
}

TEST_CASE("VectorH refract_safe with non-unit vectors", "[VectorH]") {
    VectorH<float> incident{0.0f, -5.0f, 0.0f, 0.0f}; 
    VectorH<float> normal{0.0f, 3.0f, 0.0f, 0.0f};
    
    auto result = incident.refract_safe(normal, 1.0f, 1.5f);
    
    REQUIRE(result.magnitude() >= 0.0f);
    REQUIRE(math::approx_equal(result.w(), 0.0f));
}

TEST_CASE("VectorH refract_in_place_safe with non-unit vectors", "[VectorH]") {
    VectorH<float> incident{0.0f, -5.0f, 0.0f, 0.0f};
    VectorH<float> normal{0.0f, 3.0f, 0.0f, 0.0f};    
    incident.refract_in_place_safe(normal, 1.0f, 1.5f);
    
    REQUIRE(incident.magnitude() >= 0.0f);
    REQUIRE(math::approx_equal(incident.w(), 0.0f));
}

TEST_CASE("VectorH refract total internal reflection", "[VectorH]") {
    VectorH<float> incident{0.9f, -0.1f, 0.0f, 0.0f};
    incident.normalize_in_place();
    VectorH<float> normal{0.0f, 1.0f, 0.0f, 0.0f};
    
    auto result = incident.refract(normal, 1.5f, 1.0f);
    
    REQUIRE(result.magnitude() >= 0.0f);
}


TEST_CASE("VectorH project_onto direction onto another direction", "[VectorH]") {
    VectorH<float> dir1{3.0f, 4.0f, 0.0f, 0.0f};
    VectorH<float> dir2{1.0f, 0.0f, 0.0f, 0.0f};
    
    auto result = dir1.project_onto(dir2);
    
    REQUIRE(math::approx_equal(result.x(), 3.0f));
    REQUIRE(math::approx_equal(result.y(), 0.0f));
    REQUIRE(math::approx_equal(result.z(), 0.0f));
    REQUIRE(math::approx_equal(result.w(), 0.0f));
}

TEST_CASE("VectorH project_onto_in_place direction onto another", "[VectorH]") {
    VectorH<float> dir1{3.0f, 4.0f, 0.0f, 0.0f};
    VectorH<float> dir2{1.0f, 0.0f, 0.0f, 0.0f};
    
    dir1.project_onto_in_place(dir2);
    
    REQUIRE(math::approx_equal(dir1.x(), 3.0f));
    REQUIRE(math::approx_equal(dir1.y(), 0.0f));
    REQUIRE(math::approx_equal(dir1.z(), 0.0f));
    REQUIRE(math::approx_equal(dir1.w(), 0.0f));
}


TEST_CASE("VectorH point vs direction semantics", "[VectorH]") {
    VectorH<float> point{1.0f, 2.0f, 3.0f, 1.0f};
    VectorH<float> direction{4.0f, 5.0f, 6.0f, 0.0f};
    
    auto result1 = point + direction;
    REQUIRE(math::approx_equal(result1.w(), 1.0f));
    
    auto result2 = direction + point;
    REQUIRE(math::approx_equal(result2.w(), 1.0f));
    
    VectorH<float> point2{2.0f, 3.0f, 4.0f, 1.0f};
    auto result3 = point - point2;
    REQUIRE(math::approx_equal(result3.w(), 0.0f));
    
    auto result4 = point - direction;
    REQUIRE(math::approx_equal(result4.w(), 1.0f));
}

TEST_CASE("VectorH homogenize converts to canonical point form", "[VectorH]") {
    VectorH<float> scaled_point{4.0f, 8.0f, 12.0f, 2.0f};
    
    auto canonical = scaled_point.homogenize();
    
    REQUIRE(math::approx_equal(canonical.x(), 2.0f));
    REQUIRE(math::approx_equal(canonical.y(), 4.0f));
    REQUIRE(math::approx_equal(canonical.z(), 6.0f));
    REQUIRE(math::approx_equal(canonical.w(), 1.0f));
}

TEST_CASE("VectorH magnitude operations only on directions", "[VectorH]") {
    VectorH<float> dir{3.0f, 4.0f, 0.0f, 0.0f};
    
    float mag = dir.magnitude();
    float magSq = dir.magnitudeSquared();
    
    REQUIRE(math::approx_equal(mag, 5.0f));
    REQUIRE(math::approx_equal(magSq, 25.0f));
}

TEST_CASE("VectorH distance operations only on points", "[VectorH]") {
    VectorH<float> p1{0.0f, 0.0f, 0.0f, 1.0f};
    VectorH<float> p2{3.0f, 4.0f, 0.0f, 1.0f};
    
    float dist = p1.distance(p2);
    float distSq = p1.distance_squared(p2);
    
    REQUIRE(math::approx_equal(dist, 5.0f));
    REQUIRE(math::approx_equal(distSq, 25.0f));
}

TEST_CASE("VectorH scalar operations preserve w component semantics", "[VectorH]") {
    VectorH<float> point{2.0f, 4.0f, 6.0f, 1.0f};
    VectorH<float> direction{2.0f, 4.0f, 6.0f, 0.0f};
    
    auto scaled_point = point * 2.0f;
    auto scaled_dir = direction * 2.0f;
    
    REQUIRE(math::approx_equal(scaled_point.w(), 2.0f));
    REQUIRE(math::approx_equal(scaled_dir.w(), 0.0f));
}

TEST_CASE("VectorH division by zero preserves w", "[VectorH]") {
    #ifdef NDEBUG
    VectorH<float> point{6.0f, 9.0f, 12.0f, 1.0f};
    VectorH<float> direction{6.0f, 9.0f, 12.0f, 0.0f};
    
    auto result_point = point / 0.0f;
    auto result_dir = direction / 0.0f;
    
    REQUIRE(math::approx_equal(result_point.w(), 1.0f));
    REQUIRE(math::approx_equal(result_dir.w(), 0.0f));
    #endif
}

TEST_CASE("VectorH lerp at extremes returns endpoints", "[VectorH]") {
    VectorH<float> p1{0.0f, 0.0f, 0.0f, 1.0f};
    VectorH<float> p2{10.0f, 10.0f, 10.0f, 1.0f};
    
    auto at_start = p1.lerp(p2, 0.0f);
    auto at_end = p1.lerp(p2, 1.0f);
    
    REQUIRE(math::approx_equal(at_start.x(), 0.0f));
    REQUIRE(math::approx_equal(at_start.y(), 0.0f));
    REQUIRE(math::approx_equal(at_start.z(), 0.0f));
    
    REQUIRE(math::approx_equal(at_end.x(), 10.0f));
    REQUIRE(math::approx_equal(at_end.y(), 10.0f));
    REQUIRE(math::approx_equal(at_end.z(), 10.0f));
}

TEST_CASE("VectorH cross product produces perpendicular direction", "[VectorH]") {
    VectorH<float> dir1{1.0f, 0.0f, 0.0f, 0.0f};
    VectorH<float> dir2{0.0f, 1.0f, 0.0f, 0.0f};
    
    auto result = dir1.cross(dir2);
    

    REQUIRE(math::approx_equal(result.dot(dir1), 0.0f));
    REQUIRE(math::approx_equal(result.dot(dir2), 0.0f));
    REQUIRE(math::approx_equal(result.w(), 0.0f)); 
}

TEST_CASE("VectorH normalize preserves direction", "[VectorH]") {
    VectorH<float> dir{3.0f, 4.0f, 0.0f, 0.0f};
    auto original_dir = dir;
    
    auto normalized = dir.normalize();

    float scale = original_dir.magnitude();
    REQUIRE(math::approx_equal(normalized.x() * scale, original_dir.x()));
    REQUIRE(math::approx_equal(normalized.y() * scale, original_dir.y()));
    REQUIRE(math::approx_equal(normalized.magnitude(), 1.0f));
}

TEST_CASE("VectorH constructors and initialization", "[VectorH]") {
    VectorH<float> v1{1.0f, 2.0f, 3.0f, 4.0f};
    REQUIRE(math::approx_equal(v1.x(), 1.0f));
    REQUIRE(math::approx_equal(v1.y(), 2.0f));
    REQUIRE(math::approx_equal(v1.z(), 3.0f));
    REQUIRE(math::approx_equal(v1.w(), 4.0f));
    
    VectorH<float> point{5.0f, 6.0f, 7.0f, 1.0f};
    REQUIRE(math::approx_equal(point.w(), 1.0f));
    
    VectorH<float> direction{5.0f, 6.0f, 7.0f, 0.0f};
    REQUIRE(math::approx_equal(direction.w(), 0.0f));
}

TEST_CASE("VectorH chained operations", "[VectorH]") {
    VectorH<float> dir{1.0f, 2.0f, 3.0f, 0.0f};
    VectorH<float> dir2{1.0f, 1.0f, 1.0f, 0.0f};
    
    dir += dir2;
    dir *= 2.0f;
    dir -= dir2;
    dir /= 2.0f;
    
    REQUIRE(math::approx_equal(dir.x(), 1.5f));
    REQUIRE(math::approx_equal(dir.y(), 2.5f));
    REQUIRE(math::approx_equal(dir.z(), 3.5f));
    REQUIRE(math::approx_equal(dir.w(), 0.0f));
}

TEST_CASE("VectorH symmetry of operations", "[VectorH]") {
    VectorH<float> p1{1.0f, 2.0f, 3.0f, 1.0f};
    VectorH<float> p2{4.0f, 5.0f, 6.0f, 1.0f};
    
    float dist1 = p1.distance(p2);
    float dist2 = p2.distance(p1);
    REQUIRE(math::approx_equal(dist1, dist2));
    
    VectorH<float> d1{1.0f, 2.0f, 3.0f, 0.0f};
    VectorH<float> d2{4.0f, 5.0f, 6.0f, 0.0f};
    float dot1 = d1.dot(d2);
    float dot2 = d2.dot(d1);
    REQUIRE(math::approx_equal(dot1, dot2));
}