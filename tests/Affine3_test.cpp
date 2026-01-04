#define NDEBUG
#include <catch2/catch_test_macros.hpp>
#include "../ES_math.hpp"
#include "../Matrix.hpp"
#include "../VectorN.hpp"
#include "../AffineTransform3.hpp"

using namespace ES;

TEST_CASE("AffineTransform3 default constructor", "[AffineTransform3]"){
    AffineTransform3<float> transform;
    
    auto translation = transform.get_translation();
    
    REQUIRE(translation[0] == 0.0f);
    REQUIRE(translation[1] == 0.0f);
    REQUIRE(translation[2] == 0.0f);
}

TEST_CASE("AffineTransform3 from_translation", "[AffineTransform3]"){
    Vector3<float> t(1.0f, 2.0f, 3.0f);
    
    auto transform = AffineTransform3<float>::from_translation(t);
    auto translation = transform.get_translation();
    
    REQUIRE(translation[0] == 1.0f);
    REQUIRE(translation[1] == 2.0f);
    REQUIRE(translation[2] == 3.0f);
}

TEST_CASE("AffineTransform3 from_scale", "[AffineTransform3]"){
    Vector3<float> s(2.0f, 3.0f, 4.0f);
    
    auto transform = AffineTransform3<float>::from_scale(s);
    auto scale = transform.get_scale();
    
    REQUIRE(math::approx_equal(scale[0], 2.0f));
    REQUIRE(math::approx_equal(scale[1], 3.0f));
    REQUIRE(math::approx_equal(scale[2], 4.0f));
}

TEST_CASE("AffineTransform3 transform_point translation", "[AffineTransform3]"){
    Vector3<float> t(1.0f, 2.0f, 3.0f);
    auto transform = AffineTransform3<float>::from_translation(t);
    
    Vector3<float> point(0.0f, 0.0f, 0.0f);
    auto result = transform.transform_point(point);
    
    REQUIRE(result[0] == 1.0f);
    REQUIRE(result[1] == 2.0f);
    REQUIRE(result[2] == 3.0f);
}

TEST_CASE("AffineTransform3 transform_point scale", "[AffineTransform3]"){
    Vector3<float> s(2.0f, 3.0f, 4.0f);
    auto transform = AffineTransform3<float>::from_scale(s);
    
    Vector3<float> point(1.0f, 1.0f, 1.0f);
    auto result = transform.transform_point(point);
    
    REQUIRE(result[0] == 2.0f);
    REQUIRE(result[1] == 3.0f);
    REQUIRE(result[2] == 4.0f);
}

TEST_CASE("AffineTransform3 transform_vector", "[AffineTransform3]"){
    Vector3<float> s(2.0f, 2.0f, 2.0f);
    auto transform = AffineTransform3<float>::from_scale(s);
    
    Vector3<float> vec(1.0f, 2.0f, 3.0f);
    auto result = transform.transform_vector(vec);
    
    REQUIRE(result[0] == 2.0f);
    REQUIRE(result[1] == 4.0f);
    REQUIRE(result[2] == 6.0f);
}

TEST_CASE("AffineTransform3 transform_vector ignores translation", "[AffineTransform3]"){
    Vector3<float> t(10.0f, 20.0f, 30.0f);
    auto transform = AffineTransform3<float>::from_translation(t);
    
    Vector3<float> vec(1.0f, 2.0f, 3.0f);
    auto result = transform.transform_vector(vec);
    
    REQUIRE(result[0] == 1.0f);
    REQUIRE(result[1] == 2.0f);
    REQUIRE(result[2] == 3.0f);
}

TEST_CASE("AffineTransform3 transform_point_inverse", "[AffineTransform3]"){
    Vector3<float> t(1.0f, 2.0f, 3.0f);
    auto transform = AffineTransform3<float>::from_translation(t);
    
    Vector3<float> point(4.0f, 6.0f, 9.0f);
    auto result = transform.transform_point_inverse(point);
    
    REQUIRE(math::approx_equal(result[0], 3.0f));
    REQUIRE(math::approx_equal(result[1], 4.0f));
    REQUIRE(math::approx_equal(result[2], 6.0f));
}

TEST_CASE("AffineTransform3 transform_vector_inverse", "[AffineTransform3]"){
    Vector3<float> s(2.0f, 4.0f, 8.0f);
    auto transform = AffineTransform3<float>::from_scale(s);
    
    Vector3<float> vec(4.0f, 8.0f, 16.0f);
    auto result = transform.transform_vector_inverse(vec);
    
    REQUIRE(math::approx_equal(result[0], 2.0f));
    REQUIRE(math::approx_equal(result[1], 2.0f));
    REQUIRE(math::approx_equal(result[2], 2.0f));
}

TEST_CASE("AffineTransform3 operator* composition", "[AffineTransform3]"){
    Vector3<float> t1(1.0f, 0.0f, 0.0f);
    Vector3<float> t2(0.0f, 2.0f, 0.0f);
    
    auto transform1 = AffineTransform3<float>::from_translation(t1);
    auto transform2 = AffineTransform3<float>::from_translation(t2);
    
    auto combined = transform1 * transform2;
    
    Vector3<float> point(0.0f, 0.0f, 0.0f);
    auto result = combined.transform_point(point);
    
    REQUIRE(math::approx_equal(result[0], 1.0f));
    REQUIRE(math::approx_equal(result[1], 2.0f));
    REQUIRE(math::approx_equal(result[2], 0.0f));
}

TEST_CASE("AffineTransform3 operator* scale then translate", "[AffineTransform3]"){
    Vector3<float> s(2.0f, 2.0f, 2.0f);
    Vector3<float> t(1.0f, 1.0f, 1.0f);
    
    auto scale_transform = AffineTransform3<float>::from_scale(s);
    auto translate_transform = AffineTransform3<float>::from_translation(t);
    
    auto combined = translate_transform * scale_transform;
    
    Vector3<float> point(1.0f, 1.0f, 1.0f);
    auto result = combined.transform_point(point);
    
    REQUIRE(math::approx_equal(result[0], 3.0f));
    REQUIRE(math::approx_equal(result[1], 3.0f));
    REQUIRE(math::approx_equal(result[2], 3.0f));
}

TEST_CASE("AffineTransform3 operator*=", "[AffineTransform3]"){
    Vector3<float> t1(1.0f, 0.0f, 0.0f);
    Vector3<float> t2(0.0f, 2.0f, 0.0f);
    
    auto transform1 = AffineTransform3<float>::from_translation(t1);
    auto transform2 = AffineTransform3<float>::from_translation(t2);
    
    transform1 *= transform2;
    
    Vector3<float> point(0.0f, 0.0f, 0.0f);
    auto result = transform1.transform_point(point);
    
    REQUIRE(math::approx_equal(result[0], 1.0f));
    REQUIRE(math::approx_equal(result[1], 2.0f));
    REQUIRE(math::approx_equal(result[2], 0.0f));
}

TEST_CASE("AffineTransform3 inverse translation", "[AffineTransform3]"){
    Vector3<float> t(1.0f, 2.0f, 3.0f);
    auto transform = AffineTransform3<float>::from_translation(t);
    
    auto inv = transform.inverse();
    auto identity_transform = transform * inv;
    
    Vector3<float> point(5.0f, 5.0f, 5.0f);
    auto result = identity_transform.transform_point(point);
    
    REQUIRE(math::approx_equal(result[0], 5.0f));
    REQUIRE(math::approx_equal(result[1], 5.0f));
    REQUIRE(math::approx_equal(result[2], 5.0f));
}

TEST_CASE("AffineTransform3 inverse scale", "[AffineTransform3]"){
    Vector3<float> s(2.0f, 3.0f, 4.0f);
    auto transform = AffineTransform3<float>::from_scale(s);
    
    auto inv = transform.inverse();
    auto identity_transform = transform * inv;
    
    Vector3<float> point(5.0f, 6.0f, 7.0f);
    auto result = identity_transform.transform_point(point);
    
    REQUIRE(math::approx_equal(result[0], 5.0f));
    REQUIRE(math::approx_equal(result[1], 6.0f));
    REQUIRE(math::approx_equal(result[2], 7.0f));
}

TEST_CASE("AffineTransform3 get_translation", "[AffineTransform3]"){
    Vector3<float> t(4.0f, 5.0f, 6.0f);
    auto transform = AffineTransform3<float>::from_translation(t);
    
    auto translation = transform.get_translation();
    
    REQUIRE(translation[0] == 4.0f);
    REQUIRE(translation[1] == 5.0f);
    REQUIRE(translation[2] == 6.0f);
}

TEST_CASE("AffineTransform3 get_scale uniform", "[AffineTransform3]"){
    Vector3<float> s(5.0f, 5.0f, 5.0f);
    auto transform = AffineTransform3<float>::from_scale(s);
    
    auto scale = transform.get_scale();
    
    REQUIRE(math::approx_equal(scale[0], 5.0f));
    REQUIRE(math::approx_equal(scale[1], 5.0f));
    REQUIRE(math::approx_equal(scale[2], 5.0f));
}

TEST_CASE("AffineTransform3 get_scale non-uniform", "[AffineTransform3]"){
    Vector3<float> s(1.0f, 2.0f, 3.0f);
    auto transform = AffineTransform3<float>::from_scale(s);
    
    auto scale = transform.get_scale();
    
    REQUIRE(math::approx_equal(scale[0], 1.0f));
    REQUIRE(math::approx_equal(scale[1], 2.0f));
    REQUIRE(math::approx_equal(scale[2], 3.0f));
}

TEST_CASE("AffineTransform3 to_matrix4", "[AffineTransform3]"){
    Vector3<float> t(1.0f, 2.0f, 3.0f);
    Vector3<float> s(2.0f, 2.0f, 2.0f);
    
    auto scale_transform = AffineTransform3<float>::from_scale(s);
    auto translate_transform = AffineTransform3<float>::from_translation(t);
    auto combined = translate_transform * scale_transform;
    
    auto mat = combined.to_matrix4();
    
    REQUIRE(mat(0,3) == 1.0f);
    REQUIRE(mat(1,3) == 2.0f);
    REQUIRE(mat(2,3) == 3.0f);
    REQUIRE(mat(3,3) == 1.0f);
    REQUIRE(mat(0,0) == 2.0f);
    REQUIRE(mat(1,1) == 2.0f);
    REQUIRE(mat(2,2) == 2.0f);
}

TEST_CASE("AffineTransform3 from_matrix4", "[AffineTransform3]"){
    Matrix<float, 4, 4> mat = Matrix<float, 4, 4>::identity();
    mat(0,3) = 1.0f;
    mat(1,3) = 2.0f;
    mat(2,3) = 3.0f;
    mat(0,0) = 2.0f;
    mat(1,1) = 3.0f;
    mat(2,2) = 4.0f;
    
    auto transform = AffineTransform3<float>::from_matrix4(mat);
    
    auto translation = transform.get_translation();
    auto scale = transform.get_scale();
    
    REQUIRE(translation[0] == 1.0f);
    REQUIRE(translation[1] == 2.0f);
    REQUIRE(translation[2] == 3.0f);
    REQUIRE(math::approx_equal(scale[0], 2.0f));
    REQUIRE(math::approx_equal(scale[1], 3.0f));
    REQUIRE(math::approx_equal(scale[2], 4.0f));
}

TEST_CASE("AffineTransform3 to_matrix4 and back", "[AffineTransform3]"){
    Vector3<float> t(5.0f, 6.0f, 7.0f);
    Vector3<float> s(2.0f, 3.0f, 4.0f);
    
    auto scale_transform = AffineTransform3<float>::from_scale(s);
    auto translate_transform = AffineTransform3<float>::from_translation(t);
    auto original = translate_transform * scale_transform;
    
    auto mat = original.to_matrix4();
    auto reconstructed = AffineTransform3<float>::from_matrix4(mat);
    
    Vector3<float> point(1.0f, 1.0f, 1.0f);
    auto result1 = original.transform_point(point);
    auto result2 = reconstructed.transform_point(point);
    
    REQUIRE(math::approx_equal(result1[0], result2[0]));
    REQUIRE(math::approx_equal(result1[1], result2[1]));
    REQUIRE(math::approx_equal(result1[2], result2[2]));
}

TEST_CASE("AffineTransform3 complex transformation", "[AffineTransform3]"){
    Vector3<float> s(2.0f, 2.0f, 2.0f);
    Vector3<float> t(10.0f, 20.0f, 30.0f);
    
    auto scale_transform = AffineTransform3<float>::from_scale(s);
    auto translate_transform = AffineTransform3<float>::from_translation(t);
    
    auto combined = translate_transform * scale_transform;
    
    Vector3<float> point(1.0f, 2.0f, 3.0f);
    auto result = combined.transform_point(point);
    
    REQUIRE(math::approx_equal(result[0], 12.0f));
    REQUIRE(math::approx_equal(result[1], 24.0f));
    REQUIRE(math::approx_equal(result[2], 36.0f));
}

TEST_CASE("AffineTransform3 identity transform", "[AffineTransform3]"){
    AffineTransform3<float> identity;
    
    Vector3<float> point(5.0f, 7.0f, 9.0f);
    auto result = identity.transform_point(point);
    
    REQUIRE(result[0] == 5.0f);
    REQUIRE(result[1] == 7.0f);
    REQUIRE(result[2] == 9.0f);
}