#define NDEBUG
#include <catch2/catch_test_macros.hpp>
#include "../ES_math.hpp"
#include "../Quaternion.hpp"
#include "../VectorN.hpp"
#include "../Angle.hpp"

using namespace ES;

TEST_CASE("Quaternion default constructor", "[Quaternion]"){
    Quaternion<float> q;
    
    REQUIRE(q.w() == 0.0f);
    REQUIRE(q.x() == 0.0f);
    REQUIRE(q.y() == 0.0f);
    REQUIRE(q.z() == 0.0f);
}

TEST_CASE("Quaternion constructor with values", "[Quaternion]"){
    Quaternion<float> q(1.0f, 2.0f, 3.0f, 4.0f);
    
    REQUIRE(q.w() == 1.0f);
    REQUIRE(q.x() == 2.0f);
    REQUIRE(q.y() == 3.0f);
    REQUIRE(q.z() == 4.0f);
}

TEST_CASE("Quaternion constructor from axis angle", "[Quaternion]"){
    Vector3<float> axis(0.0f, 0.0f, 1.0f);
    Angle<in_radians, float> angle(math::half_pi<float>);
    
    Quaternion<float> q(axis, angle);
    
    REQUIRE(math::approx_equal(q.w(), std::cos(math::half_pi<float> * 0.5f)));
    REQUIRE(math::approx_equal(q.x(), 0.0f));
    REQUIRE(math::approx_equal(q.y(), 0.0f));
    REQUIRE(math::approx_equal(q.z(), std::sin(math::half_pi<float> * 0.5f)));
}

TEST_CASE("Quaternion constructor from vector", "[Quaternion]"){
    Vector4<float> vec(1.0f, 2.0f, 3.0f, 4.0f);
    
    Quaternion<float> q(vec);
    
    REQUIRE(q.w() == 1.0f);
    REQUIRE(q.x() == 2.0f);
    REQUIRE(q.y() == 3.0f);
    REQUIRE(q.z() == 4.0f);
}

TEST_CASE("Quaternion w accessor", "[Quaternion]"){
    Quaternion<float> q(5.0f, 0.0f, 0.0f, 0.0f);
    
    REQUIRE(q.w() == 5.0f);
}

TEST_CASE("Quaternion x accessor", "[Quaternion]"){
    Quaternion<float> q(0.0f, 6.0f, 0.0f, 0.0f);
    
    REQUIRE(q.x() == 6.0f);
}

TEST_CASE("Quaternion y accessor", "[Quaternion]"){
    Quaternion<float> q(0.0f, 0.0f, 7.0f, 0.0f);
    
    REQUIRE(q.y() == 7.0f);
}

TEST_CASE("Quaternion z accessor", "[Quaternion]"){
    Quaternion<float> q(0.0f, 0.0f, 0.0f, 8.0f);
    
    REQUIRE(q.z() == 8.0f);
}

TEST_CASE("Quaternion w mutable accessor", "[Quaternion]"){
    Quaternion<float> q;
    
    q.w() = 10.0f;
    
    REQUIRE(q.w() == 10.0f);
}

TEST_CASE("Quaternion x mutable accessor", "[Quaternion]"){
    Quaternion<float> q;
    
    q.x() = 11.0f;
    
    REQUIRE(q.x() == 11.0f);
}

TEST_CASE("Quaternion y mutable accessor", "[Quaternion]"){
    Quaternion<float> q;
    
    q.y() = 12.0f;
    
    REQUIRE(q.y() == 12.0f);
}

TEST_CASE("Quaternion z mutable accessor", "[Quaternion]"){
    Quaternion<float> q;
    
    q.z() = 13.0f;
    
    REQUIRE(q.z() == 13.0f);
}

TEST_CASE("Quaternion vector", "[Quaternion]"){
    Quaternion<float> q(1.0f, 2.0f, 3.0f, 4.0f);
    
    auto vec = q.vector();
    
    REQUIRE(vec[0] == 1.0f);
    REQUIRE(vec[1] == 2.0f);
    REQUIRE(vec[2] == 3.0f);
    REQUIRE(vec[3] == 4.0f);
}

TEST_CASE("Quaternion operator* hamilton product", "[Quaternion]"){
    Quaternion<float> q1(1.0f, 0.0f, 0.0f, 0.0f);
    Quaternion<float> q2(0.0f, 1.0f, 0.0f, 0.0f);
    
    auto result = q1 * q2;
    
    REQUIRE(result.w() == 0.0f);
    REQUIRE(result.x() == 1.0f);
    REQUIRE(result.y() == 0.0f);
    REQUIRE(result.z() == 0.0f);
}

TEST_CASE("Quaternion operator* identity", "[Quaternion]"){
    Quaternion<float> q(2.0f, 3.0f, 4.0f, 5.0f);
    Quaternion<float> identity = Quaternion<float>::identity();
    
    auto result = q * identity;
    
    REQUIRE(math::approx_equal(result.w(), 2.0f));
    REQUIRE(math::approx_equal(result.x(), 3.0f));
    REQUIRE(math::approx_equal(result.y(), 4.0f));
    REQUIRE(math::approx_equal(result.z(), 5.0f));
}

TEST_CASE("Quaternion operator*=", "[Quaternion]"){
    Quaternion<float> q1(1.0f, 0.0f, 0.0f, 0.0f);
    Quaternion<float> q2(0.0f, 1.0f, 0.0f, 0.0f);
    
    q1 *= q2;
    
    REQUIRE(q1.w() == 0.0f);
    REQUIRE(q1.x() == 1.0f);
    REQUIRE(q1.y() == 0.0f);
    REQUIRE(q1.z() == 0.0f);
}

TEST_CASE("Quaternion length", "[Quaternion]"){
    Quaternion<float> q(1.0f, 0.0f, 0.0f, 0.0f);
    
    float len = q.length();
    
    REQUIRE(math::approx_equal(len, 1.0f));
}

TEST_CASE("Quaternion length_squared", "[Quaternion]"){
    Quaternion<float> q(2.0f, 0.0f, 0.0f, 0.0f);
    
    float len_sq = q.length_squared();
    
    REQUIRE(math::approx_equal(len_sq, 4.0f));
}

TEST_CASE("Quaternion length with non-unit quaternion", "[Quaternion]"){
    Quaternion<float> q(1.0f, 2.0f, 3.0f, 4.0f);
    
    float len = q.length();
    
    REQUIRE(math::approx_equal(len, std::sqrt(30.0f)));
}

TEST_CASE("Quaternion normalize", "[Quaternion]"){
    Quaternion<float> q(2.0f, 0.0f, 0.0f, 0.0f);
    
    auto normalized = q.normalize();
    
    REQUIRE(math::approx_equal(normalized.w(), 1.0f));
    REQUIRE(math::approx_equal(normalized.x(), 0.0f));
    REQUIRE(math::approx_equal(normalized.y(), 0.0f));
    REQUIRE(math::approx_equal(normalized.z(), 0.0f));
    REQUIRE(q.w() == 2.0f);
}

TEST_CASE("Quaternion normalize_in_place", "[Quaternion]"){
    Quaternion<float> q(2.0f, 0.0f, 0.0f, 0.0f);
    
    q.normalize_in_place();
    
    REQUIRE(math::approx_equal(q.w(), 1.0f));
    REQUIRE(math::approx_equal(q.x(), 0.0f));
    REQUIRE(math::approx_equal(q.y(), 0.0f));
    REQUIRE(math::approx_equal(q.z(), 0.0f));
}

TEST_CASE("Quaternion normalize unit length", "[Quaternion]"){
    Quaternion<float> q(1.0f, 2.0f, 3.0f, 4.0f);
    
    auto normalized = q.normalize();
    
    REQUIRE(math::approx_equal(normalized.length(), 1.0f));
}

TEST_CASE("Quaternion conjugate", "[Quaternion]"){
    Quaternion<float> q(1.0f, 2.0f, 3.0f, 4.0f);
    
    auto conj = q.conjugate();
    
    REQUIRE(conj.w() == 1.0f);
    REQUIRE(conj.x() == -2.0f);
    REQUIRE(conj.y() == -3.0f);
    REQUIRE(conj.z() == -4.0f);
    REQUIRE(q.w() == 1.0f);
}

TEST_CASE("Quaternion conjugate_in_place", "[Quaternion]"){
    Quaternion<float> q(1.0f, 2.0f, 3.0f, 4.0f);
    
    q.conjugate_in_place();
    
    REQUIRE(q.w() == 1.0f);
    REQUIRE(q.x() == -2.0f);
    REQUIRE(q.y() == -3.0f);
    REQUIRE(q.z() == -4.0f);
}

TEST_CASE("Quaternion inverse", "[Quaternion]"){
    Quaternion<float> q(1.0f, 0.0f, 0.0f, 0.0f);
    
    auto inv = q.inverse();
    
    REQUIRE(math::approx_equal(inv.w(), 1.0f));
    REQUIRE(math::approx_equal(inv.x(), 0.0f));
    REQUIRE(math::approx_equal(inv.y(), 0.0f));
    REQUIRE(math::approx_equal(inv.z(), 0.0f));
}

TEST_CASE("Quaternion inverse_in_place", "[Quaternion]"){
    Quaternion<float> q(1.0f, 0.0f, 0.0f, 0.0f);
    
    q.inverse_in_place();
    
    REQUIRE(math::approx_equal(q.w(), 1.0f));
    REQUIRE(math::approx_equal(q.x(), 0.0f));
    REQUIRE(math::approx_equal(q.y(), 0.0f));
    REQUIRE(math::approx_equal(q.z(), 0.0f));
}

TEST_CASE("Quaternion inverse property", "[Quaternion]"){
    Quaternion<float> q(0.7071f, 0.0f, 0.7071f, 0.0f);
    
    auto inv = q.inverse();
    auto identity_result = q * inv;
    
    REQUIRE(math::approx_equal(identity_result.w(), 1.0f, 0.001f));
    REQUIRE(math::approx_equal(identity_result.x(), 0.0f, 0.001f));
    REQUIRE(math::approx_equal(identity_result.y(), 0.0f, 0.001f));
    REQUIRE(math::approx_equal(identity_result.z(), 0.0f, 0.001f));
}

TEST_CASE("Quaternion dot", "[Quaternion]"){
    Quaternion<float> q1(1.0f, 2.0f, 3.0f, 4.0f);
    Quaternion<float> q2(5.0f, 6.0f, 7.0f, 8.0f);
    
    float result = q1.dot(q2);
    
    REQUIRE(math::approx_equal(result, 70.0f));
}

TEST_CASE("Quaternion dot with self", "[Quaternion]"){
    Quaternion<float> q(1.0f, 2.0f, 3.0f, 4.0f);
    
    float result = q.dot(q);
    
    REQUIRE(math::approx_equal(result, q.length_squared()));
}

TEST_CASE("Quaternion rotate vector around z axis", "[Quaternion]"){
    Vector3<float> axis(0.0f, 0.0f, 1.0f);
    Angle<in_radians, float> angle(math::half_pi<float>);
    Quaternion<float> q(axis, angle);
    
    Vector3<float> vec(1.0f, 0.0f, 0.0f);
    auto rotated = q.rotate(vec);
    
    REQUIRE(math::approx_equal(rotated[0], 0.0f, 0.001f));
    REQUIRE(math::approx_equal(rotated[1], 1.0f, 0.001f));
    REQUIRE(math::approx_equal(rotated[2], 0.0f, 0.001f));
}

TEST_CASE("Quaternion rotate vector around x axis", "[Quaternion]"){
    Vector3<float> axis(1.0f, 0.0f, 0.0f);
    Angle<in_radians, float> angle(math::half_pi<float>);
    Quaternion<float> q(axis, angle);
    
    Vector3<float> vec(0.0f, 1.0f, 0.0f);
    auto rotated = q.rotate(vec);
    
    REQUIRE(math::approx_equal(rotated[0], 0.0f, 0.001f));
    REQUIRE(math::approx_equal(rotated[1], 0.0f, 0.001f));
    REQUIRE(math::approx_equal(rotated[2], 1.0f, 0.001f));
}

TEST_CASE("Quaternion rotate identity", "[Quaternion]"){
    Quaternion<float> identity = Quaternion<float>::identity();
    
    Vector3<float> vec(1.0f, 2.0f, 3.0f);
    auto rotated = identity.rotate(vec);
    
    REQUIRE(math::approx_equal(rotated[0], 1.0f));
    REQUIRE(math::approx_equal(rotated[1], 2.0f));
    REQUIRE(math::approx_equal(rotated[2], 3.0f));
}

TEST_CASE("Quaternion nlerp", "[Quaternion]"){
    Quaternion<float> q1(1.0f, 0.0f, 0.0f, 0.0f);
    Quaternion<float> q2(0.0f, 1.0f, 0.0f, 0.0f);
    
    auto result = q1.nlerp(q2, 0.5f);
    
    REQUIRE(math::approx_equal(result.length(), 1.0f));
}

TEST_CASE("Quaternion nlerp at t=0", "[Quaternion]"){
    Quaternion<float> q1(1.0f, 0.0f, 0.0f, 0.0f);
    Quaternion<float> q2(0.0f, 1.0f, 0.0f, 0.0f);
    
    auto result = q1.nlerp(q2, 0.0f);
    
    REQUIRE(math::approx_equal(result.w(), q1.normalize().w()));
    REQUIRE(math::approx_equal(result.x(), q1.normalize().x()));
}

TEST_CASE("Quaternion nlerp at t=1", "[Quaternion]"){
    Quaternion<float> q1(1.0f, 0.0f, 0.0f, 0.0f);
    Quaternion<float> q2(0.0f, 1.0f, 0.0f, 0.0f);
    
    auto result = q1.nlerp(q2, 1.0f);
    
    REQUIRE(math::approx_equal(result.w(), q2.normalize().w()));
    REQUIRE(math::approx_equal(result.x(), q2.normalize().x()));
}

TEST_CASE("Quaternion slerp", "[Quaternion]"){
    Quaternion<float> q1(1.0f, 0.0f, 0.0f, 0.0f);
    Quaternion<float> q2(0.0f, 1.0f, 0.0f, 0.0f);
    
    auto result = q1.slerp(q2, 0.5f);
    
    REQUIRE(math::approx_equal(result.length(), 1.0f, 0.001f));
}

TEST_CASE("Quaternion slerp at t=0", "[Quaternion]"){
    Quaternion<float> q1(1.0f, 0.0f, 0.0f, 0.0f);
    Quaternion<float> q2(0.0f, 1.0f, 0.0f, 0.0f);
    
    auto result = q1.slerp(q2, 0.0f);
    
    REQUIRE(math::approx_equal(result.w(), 1.0f, 0.001f));
    REQUIRE(math::approx_equal(result.x(), 0.0f, 0.001f));
}

TEST_CASE("Quaternion slerp at t=1", "[Quaternion]"){
    Quaternion<float> q1(1.0f, 0.0f, 0.0f, 0.0f);
    Quaternion<float> q2(0.0f, 1.0f, 0.0f, 0.0f);
    
    auto result = q1.slerp(q2, 1.0f);
    
    REQUIRE(math::approx_equal(result.w(), 0.0f, 0.001f));
    REQUIRE(math::approx_equal(result.x(), 1.0f, 0.001f));
}

TEST_CASE("Quaternion slerp nearly parallel", "[Quaternion]"){
    Quaternion<float> q1(1.0f, 0.0f, 0.0f, 0.0f);
    Quaternion<float> q2(0.9999f, 0.001f, 0.0f, 0.0f);
    
    auto result = q1.slerp(q2, 0.5f);
    
    REQUIRE(math::approx_equal(result.length(), 1.0f, 0.001f));
}

TEST_CASE("Quaternion identity", "[Quaternion]"){
    auto identity = Quaternion<float>::identity();
    
    REQUIRE(identity.w() == 1.0f);
    REQUIRE(identity.x() == 0.0f);
    REQUIRE(identity.y() == 0.0f);
    REQUIRE(identity.z() == 0.0f);
}

TEST_CASE("Quaternion component addition", "[Quaternion]"){
    Quaternion<float> q1(1.0f, 2.0f, 3.0f, 4.0f);
    Quaternion<float> q2(5.0f, 6.0f, 7.0f, 8.0f);
    
    auto result = q1 + q2;
    
    REQUIRE(result.w() == 6.0f);
    REQUIRE(result.x() == 8.0f);
    REQUIRE(result.y() == 10.0f);
    REQUIRE(result.z() == 12.0f);
}

TEST_CASE("Quaternion component subtraction", "[Quaternion]"){
    Quaternion<float> q1(5.0f, 6.0f, 7.0f, 8.0f);
    Quaternion<float> q2(1.0f, 2.0f, 3.0f, 4.0f);
    
    auto result = q1 - q2;
    
    REQUIRE(result.w() == 4.0f);
    REQUIRE(result.x() == 4.0f);
    REQUIRE(result.y() == 4.0f);
    REQUIRE(result.z() == 4.0f);
}

TEST_CASE("Quaternion scalar multiplication", "[Quaternion]"){
    Quaternion<float> q(1.0f, 2.0f, 3.0f, 4.0f);
    
    auto result = q * 2.0f;
    
    REQUIRE(result.w() == 2.0f);
    REQUIRE(result.x() == 4.0f);
    REQUIRE(result.y() == 6.0f);
    REQUIRE(result.z() == 8.0f);
}

TEST_CASE("Quaternion scalar division", "[Quaternion]"){
    Quaternion<float> q(2.0f, 4.0f, 6.0f, 8.0f);
    
    auto result = q / 2.0f;
    
    REQUIRE(result.w() == 1.0f);
    REQUIRE(result.x() == 2.0f);
    REQUIRE(result.y() == 3.0f);
    REQUIRE(result.z() == 4.0f);
}

TEST_CASE("Quaternion negate", "[Quaternion]"){
    Quaternion<float> q(1.0f, 2.0f, 3.0f, 4.0f);
    
    auto result = -q;
    
    REQUIRE(result.w() == -1.0f);
    REQUIRE(result.x() == -2.0f);
    REQUIRE(result.y() == -3.0f);
    REQUIRE(result.z() == -4.0f);
}

TEST_CASE("Quaternion lerp", "[Quaternion]"){
    Quaternion<float> q1(0.0f, 0.0f, 0.0f, 0.0f);
    Quaternion<float> q2(2.0f, 4.0f, 6.0f, 8.0f);
    
    auto result = q1.lerp(q2, 0.5f);
    
    REQUIRE(result.w() == 1.0f);
    REQUIRE(result.x() == 2.0f);
    REQUIRE(result.y() == 3.0f);
    REQUIRE(result.z() == 4.0f);
}

TEST_CASE("Quaternion lerp_in_place", "[Quaternion]"){
    Quaternion<float> q1(0.0f, 0.0f, 0.0f, 0.0f);
    Quaternion<float> q2(2.0f, 4.0f, 6.0f, 8.0f);
    
    q1.lerp_in_place(q2, 0.5f);
    
    REQUIRE(q1.w() == 1.0f);
    REQUIRE(q1.x() == 2.0f);
    REQUIRE(q1.y() == 3.0f);
    REQUIRE(q1.z() == 4.0f);
}

TEST_CASE("Quaternion component addition in place", "[Quaternion]"){
    Quaternion<float> q1(1.0f, 2.0f, 3.0f, 4.0f);
    Quaternion<float> q2(5.0f, 6.0f, 7.0f, 8.0f);
    
    q1 += q2;
    
    REQUIRE(q1.w() == 6.0f);
    REQUIRE(q1.x() == 8.0f);
    REQUIRE(q1.y() == 10.0f);
    REQUIRE(q1.z() == 12.0f);
}

TEST_CASE("Quaternion component subtraction in place", "[Quaternion]"){
    Quaternion<float> q1(5.0f, 6.0f, 7.0f, 8.0f);
    Quaternion<float> q2(1.0f, 2.0f, 3.0f, 4.0f);
    
    q1 -= q2;
    
    REQUIRE(q1.w() == 4.0f);
    REQUIRE(q1.x() == 4.0f);
    REQUIRE(q1.y() == 4.0f);
    REQUIRE(q1.z() == 4.0f);
}

TEST_CASE("Quaternion scalar multiplication in place", "[Quaternion]"){
    Quaternion<float> q(1.0f, 2.0f, 3.0f, 4.0f);
    
    q *= 2.0f;
    
    REQUIRE(q.w() == 2.0f);
    REQUIRE(q.x() == 4.0f);
    REQUIRE(q.y() == 6.0f);
    REQUIRE(q.z() == 8.0f);
}

TEST_CASE("Quaternion scalar division in place", "[Quaternion]"){
    Quaternion<float> q(2.0f, 4.0f, 6.0f, 8.0f);
    
    q /= 2.0f;
    
    REQUIRE(q.w() == 1.0f);
    REQUIRE(q.x() == 2.0f);
    REQUIRE(q.y() == 3.0f);
    REQUIRE(q.z() == 4.0f);
}

TEST_CASE("Quaternion equality", "[Quaternion]"){
    Quaternion<float> q1(1.0f, 2.0f, 3.0f, 4.0f);
    Quaternion<float> q2(1.0f, 2.0f, 3.0f, 4.0f);
    
    REQUIRE(q1 == q2);
}

TEST_CASE("Quaternion non-commutativity", "[Quaternion]"){
    Quaternion<float> q1(1.0f, 1.0f, 0.0f, 0.0f);
    Quaternion<float> q2(1.0f, 0.0f, 1.0f, 0.0f);
    
    auto result1 = q1 * q2;
    auto result2 = q2 * q1;
    
    REQUIRE(result1.w() == result2.w());
    REQUIRE(result1.x() != result2.x() || result1.y() != result2.y() || result1.z() != result2.z());
}

TEST_CASE("Quaternion rotation composition", "[Quaternion]"){
    Vector3<float> axis_z(0.0f, 0.0f, 1.0f);
    Angle<in_radians, float> angle1(math::half_pi<float> * 0.5f);
    Quaternion<float> q1(axis_z, angle1);
    
    Angle<in_radians, float> angle2(math::half_pi<float> * 0.5f);
    Quaternion<float> q2(axis_z, angle2);
    
    auto combined = q1 * q2;
    
    Vector3<float> vec(1.0f, 0.0f, 0.0f);
    auto rotated = combined.rotate(vec);
    
    REQUIRE(math::approx_equal(rotated[0], 0.0f, 0.001f));
    REQUIRE(math::approx_equal(rotated[1], 1.0f, 0.001f));
    REQUIRE(math::approx_equal(rotated[2], 0.0f, 0.001f));
}