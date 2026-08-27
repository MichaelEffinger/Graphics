#include "constexpr_require.hpp"
#include "../ES_math.hpp"
#include "../Quaternion.hpp"
#include "../VectorN.hpp"

using namespace ES;

// ---------------------------------------------------------------------
// constructors: default, (W,X,Y,Z), from VectorN<T,4>
// ---------------------------------------------------------------------
constexpr bool test_constructors() {
    Quaternion<float> q1;
    CONSTEXPR_REQUIRE(q1.w() == 0.0f);
    CONSTEXPR_REQUIRE(q1.x() == 0.0f);
    CONSTEXPR_REQUIRE(q1.y() == 0.0f);
    CONSTEXPR_REQUIRE(q1.z() == 0.0f);

    Quaternion<float> q2(1.0f, 2.0f, 3.0f, 4.0f);
    CONSTEXPR_REQUIRE(q2.w() == 1.0f);
    CONSTEXPR_REQUIRE(q2.x() == 2.0f);
    CONSTEXPR_REQUIRE(q2.y() == 3.0f);
    CONSTEXPR_REQUIRE(q2.z() == 4.0f);

    VectorN<float, 4> vec{1.0f, 2.0f, 3.0f, 4.0f};
    Quaternion<float> q3(vec);
    CONSTEXPR_REQUIRE(q3.x() == 1.0f);
    CONSTEXPR_REQUIRE(q3.y() == 2.0f);
    CONSTEXPR_REQUIRE(q3.z() == 3.0f);
    CONSTEXPR_REQUIRE(q3.w() == 4.0f);

    return true;
}
static_assert(test_constructors());

// ---------------------------------------------------------------------
// accessors - w, x, y, z (read and mutate)
// ---------------------------------------------------------------------
constexpr bool test_accessors() {
    Quaternion<float> q(5.0f, 6.0f, 7.0f, 8.0f);
    CONSTEXPR_REQUIRE(q.w() == 5.0f);
    CONSTEXPR_REQUIRE(q.x() == 6.0f);
    CONSTEXPR_REQUIRE(q.y() == 7.0f);
    CONSTEXPR_REQUIRE(q.z() == 8.0f);

    q.w() = 10.0f;
    q.x() = 11.0f;
    q.y() = 12.0f;
    q.z() = 13.0f;
    CONSTEXPR_REQUIRE(q.w() == 10.0f);
    CONSTEXPR_REQUIRE(q.x() == 11.0f);
    CONSTEXPR_REQUIRE(q.y() == 12.0f);
    CONSTEXPR_REQUIRE(q.z() == 13.0f);

    return true;
}
static_assert(test_accessors());

// ---------------------------------------------------------------------
// vector()
// ---------------------------------------------------------------------
constexpr bool test_vector() {
    Quaternion<float> q(1.0f, 2.0f, 3.0f, 4.0f);
    auto vec = q.vector();

    CONSTEXPR_REQUIRE(vec[0] == 1.0f);
    CONSTEXPR_REQUIRE(vec[1] == 2.0f);
    CONSTEXPR_REQUIRE(vec[2] == 3.0f);
    CONSTEXPR_REQUIRE(vec[3] == 4.0f);

    return true;
}
static_assert(test_vector());

// ---------------------------------------------------------------------
// operator* (Hamilton product) / operator*=
// ---------------------------------------------------------------------
constexpr bool test_hamilton_product() {
    Quaternion<float> q1(1.0f, 0.0f, 0.0f, 0.0f);
    Quaternion<float> q2(0.0f, 1.0f, 0.0f, 0.0f);

    auto result = q1 * q2;
    CONSTEXPR_REQUIRE(result.w() == 0.0f);
    CONSTEXPR_REQUIRE(result.x() == 1.0f);
    CONSTEXPR_REQUIRE(result.y() == 0.0f);
    CONSTEXPR_REQUIRE(result.z() == 0.0f);

    Quaternion<float> q(2.0f, 3.0f, 4.0f, 5.0f);
    auto withIdentity = q * Quaternion<float>::identity();
    CONSTEXPR_REQUIRE(ES::math::approx_equal(withIdentity.w(), 2.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(withIdentity.x(), 3.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(withIdentity.y(), 4.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(withIdentity.z(), 5.0f));

    q1 *= q2;
    CONSTEXPR_REQUIRE(q1.w() == 0.0f);
    CONSTEXPR_REQUIRE(q1.x() == 1.0f);
    CONSTEXPR_REQUIRE(q1.y() == 0.0f);
    CONSTEXPR_REQUIRE(q1.z() == 0.0f);

    return true;
}
static_assert(test_hamilton_product());

// ---------------------------------------------------------------------
// non-commutativity of the Hamilton product
// ---------------------------------------------------------------------
constexpr bool test_non_commutativity() {
    Quaternion<float> q1(1.0f, 1.0f, 0.0f, 0.0f);
    Quaternion<float> q2(1.0f, 0.0f, 1.0f, 0.0f);

    auto result1 = q1 * q2;
    auto result2 = q2 * q1;

    CONSTEXPR_REQUIRE(result1.w() == result2.w());
    CONSTEXPR_REQUIRE(result1.x() != result2.x() || result1.y() != result2.y() || result1.z() != result2.z());

    return true;
}
static_assert(test_non_commutativity());

// ---------------------------------------------------------------------
// length / length_squared
// ---------------------------------------------------------------------
constexpr bool test_length() {
    Quaternion<float> unit(1.0f, 0.0f, 0.0f, 0.0f);
    CONSTEXPR_REQUIRE(ES::math::approx_equal(unit.length(), 1.0f));

    Quaternion<float> scaled(2.0f, 0.0f, 0.0f, 0.0f);
    CONSTEXPR_REQUIRE(ES::math::approx_equal(scaled.length_squared(), 4.0f));

    Quaternion<float> q(1.0f, 2.0f, 3.0f, 4.0f);
    CONSTEXPR_REQUIRE(ES::math::approx_equal(q.length(), std::sqrt(30.0f)));

    return true;
}
/* TODO
make constexpr
static_assert(test_length());
*/

// ---------------------------------------------------------------------
// normalize / normalize_in_place
// ---------------------------------------------------------------------
constexpr bool test_normalize() {
    Quaternion<float> q(2.0f, 0.0f, 0.0f, 0.0f);

    auto normalized = q.normalize();
    CONSTEXPR_REQUIRE(ES::math::approx_equal(normalized.w(), 1.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(normalized.x(), 0.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(normalized.y(), 0.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(normalized.z(), 0.0f));
    CONSTEXPR_REQUIRE(q.w() == 2.0f); // normalize() must not mutate the original

    q.normalize_in_place();
    CONSTEXPR_REQUIRE(ES::math::approx_equal(q.w(), 1.0f));

    Quaternion<float> nonUnit(1.0f, 2.0f, 3.0f, 4.0f);
    CONSTEXPR_REQUIRE(ES::math::approx_equal(nonUnit.normalize().length(), 1.0f));

    return true;
}
/* TODO
make constexpr
static_assert(test_normalize());
*/

// ---------------------------------------------------------------------
// conjugate / conjugate_in_place
// ---------------------------------------------------------------------
constexpr bool test_conjugate() {
    Quaternion<float> q(1.0f, 2.0f, 3.0f, 4.0f);

    auto conj = q.conjugate();
    CONSTEXPR_REQUIRE(conj.w() == 1.0f);
    CONSTEXPR_REQUIRE(conj.x() == -2.0f);
    CONSTEXPR_REQUIRE(conj.y() == -3.0f);
    CONSTEXPR_REQUIRE(conj.z() == -4.0f);
    CONSTEXPR_REQUIRE(q.w() == 1.0f); // conjugate() must not mutate the original

    q.conjugate_in_place();
    CONSTEXPR_REQUIRE(q.w() == 1.0f);
    CONSTEXPR_REQUIRE(q.x() == -2.0f);
    CONSTEXPR_REQUIRE(q.y() == -3.0f);
    CONSTEXPR_REQUIRE(q.z() == -4.0f);

    return true;
}
static_assert(test_conjugate());

// ---------------------------------------------------------------------
// inverse / inverse_in_place
//
// Unlike length()/normalize(), inverse() divides by length_squared()
// rather than length(), so it never touches std::sqrt at all - a
// genuinely clean constexpr case.
// ---------------------------------------------------------------------
constexpr bool test_inverse() {
    Quaternion<float> q(1.0f, 0.0f, 0.0f, 0.0f);

    auto inv = q.inverse();
    CONSTEXPR_REQUIRE(ES::math::approx_equal(inv.w(), 1.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(inv.x(), 0.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(inv.y(), 0.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(inv.z(), 0.0f));

    q.inverse_in_place();
    CONSTEXPR_REQUIRE(ES::math::approx_equal(q.w(), 1.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(q.x(), 0.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(q.y(), 0.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(q.z(), 0.0f));

    // 0.7071 components are close enough to unit-length here that this
    // holds within the default approx_equal tolerance, same as the
    // Catch2 suite's own "inverse property" test.
    Quaternion<float> q2(0.7071f, 0.0f, 0.7071f, 0.0f);
    auto inv2 = q2.inverse();
    auto identityResult = q2 * inv2;
    CONSTEXPR_REQUIRE(ES::math::approx_equal(identityResult.w(), 1.0f, 0.001f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(identityResult.x(), 0.0f, 0.001f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(identityResult.y(), 0.0f, 0.001f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(identityResult.z(), 0.0f, 0.001f));

    return true;
}
static_assert(test_inverse());

// ---------------------------------------------------------------------
// dot
// ---------------------------------------------------------------------
constexpr bool test_dot() {
    Quaternion<float> q1(1.0f, 2.0f, 3.0f, 4.0f);
    Quaternion<float> q2(5.0f, 6.0f, 7.0f, 8.0f);

    CONSTEXPR_REQUIRE(ES::math::approx_equal(q1.dot(q2), 70.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(q1.dot(q1), q1.length_squared()));

    return true;
}
static_assert(test_dot());

// ---------------------------------------------------------------------
// rotate
// ---------------------------------------------------------------------
constexpr bool test_rotate() {
    // cos(pi/4) == sin(pi/4) == 0.70710678...
    Quaternion<float> q(0.70710678f, 0.0f, 0.0f, 0.70710678f);

    VectorN<float, 3> vec{1.0f, 0.0f, 0.0f};
    auto rotated = q.rotate(vec);

    CONSTEXPR_REQUIRE(ES::math::approx_equal(rotated[0], 0.0f, 0.001f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(rotated[1], 1.0f, 0.001f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(rotated[2], 0.0f, 0.001f));

    // identity quaternion must leave the vector unchanged
    auto identityRotated = Quaternion<float>::identity().rotate(VectorN<float, 3>{1.0f, 2.0f, 3.0f});
    CONSTEXPR_REQUIRE(ES::math::approx_equal(identityRotated[0], 1.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(identityRotated[1], 2.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(identityRotated[2], 3.0f));

    return true;
}
static_assert(test_rotate());

// ---------------------------------------------------------------------
// identity (static factory)
// ---------------------------------------------------------------------
constexpr bool test_identity() {
    auto identity = Quaternion<float>::identity();
    CONSTEXPR_REQUIRE(identity.w() == 1.0f);
    CONSTEXPR_REQUIRE(identity.x() == 0.0f);
    CONSTEXPR_REQUIRE(identity.y() == 0.0f);
    CONSTEXPR_REQUIRE(identity.z() == 0.0f);
    return true;
}
static_assert(test_identity());

// ---------------------------------------------------------------------
// inherited arithmetic: component add/subtract, negate, scalar
// multiply/divide, lerp - brought in from ArithmeticOpsMixin
// ---------------------------------------------------------------------
constexpr bool test_component_addition() {
    Quaternion<float> q1(1.0f, 2.0f, 3.0f, 4.0f);
    Quaternion<float> q2(5.0f, 6.0f, 7.0f, 8.0f);

    auto result = q1 + q2;
    CONSTEXPR_REQUIRE(result.w() == 6.0f);
    CONSTEXPR_REQUIRE(result.x() == 8.0f);
    CONSTEXPR_REQUIRE(result.y() == 10.0f);
    CONSTEXPR_REQUIRE(result.z() == 12.0f);

    q1 += q2;
    CONSTEXPR_REQUIRE(q1 == result);

    return true;
}
static_assert(test_component_addition());

constexpr bool test_component_subtraction() {
    Quaternion<float> q1(5.0f, 6.0f, 7.0f, 8.0f);
    Quaternion<float> q2(1.0f, 2.0f, 3.0f, 4.0f);

    auto result = q1 - q2;
    CONSTEXPR_REQUIRE(result.w() == 4.0f);
    CONSTEXPR_REQUIRE(result.x() == 4.0f);
    CONSTEXPR_REQUIRE(result.y() == 4.0f);
    CONSTEXPR_REQUIRE(result.z() == 4.0f);

    q1 -= q2;
    CONSTEXPR_REQUIRE(q1 == result);

    return true;
}
static_assert(test_component_subtraction());

constexpr bool test_negate() {
    Quaternion<float> q(1.0f, 2.0f, 3.0f, 4.0f);
    auto result = -q;

    CONSTEXPR_REQUIRE(result.w() == -1.0f);
    CONSTEXPR_REQUIRE(result.x() == -2.0f);
    CONSTEXPR_REQUIRE(result.y() == -3.0f);
    CONSTEXPR_REQUIRE(result.z() == -4.0f);

    return true;
}
static_assert(test_negate());

constexpr bool test_scalar_multiplication() {
    Quaternion<float> q(1.0f, 2.0f, 3.0f, 4.0f);

    auto result = q * 2.0f;
    CONSTEXPR_REQUIRE(result.w() == 2.0f);
    CONSTEXPR_REQUIRE(result.x() == 4.0f);
    CONSTEXPR_REQUIRE(result.y() == 6.0f);
    CONSTEXPR_REQUIRE(result.z() == 8.0f);

    q *= 2.0f;
    CONSTEXPR_REQUIRE(q == result);

    return true;
}
static_assert(test_scalar_multiplication());

constexpr bool test_scalar_division() {
    Quaternion<float> q(2.0f, 4.0f, 6.0f, 8.0f);

    auto result = q / 2.0f;
    CONSTEXPR_REQUIRE(result.w() == 1.0f);
    CONSTEXPR_REQUIRE(result.x() == 2.0f);
    CONSTEXPR_REQUIRE(result.y() == 3.0f);
    CONSTEXPR_REQUIRE(result.z() == 4.0f);

    q /= 2.0f;
    CONSTEXPR_REQUIRE(q == result);

    return true;
}
static_assert(test_scalar_division());

constexpr bool test_lerp() {
    Quaternion<float> q1(0.0f, 0.0f, 0.0f, 0.0f);
    Quaternion<float> q2(2.0f, 4.0f, 6.0f, 8.0f);

    auto result = q1.lerp(q2, 0.5f);
    CONSTEXPR_REQUIRE(result.w() == 1.0f);
    CONSTEXPR_REQUIRE(result.x() == 2.0f);
    CONSTEXPR_REQUIRE(result.y() == 3.0f);
    CONSTEXPR_REQUIRE(result.z() == 4.0f);

    q1.lerp_in_place(q2, 0.5f);
    CONSTEXPR_REQUIRE(q1 == result);

    return true;
}
static_assert(test_lerp());

// ---------------------------------------------------------------------
// equality
// ---------------------------------------------------------------------
constexpr bool test_equality() {
    Quaternion<float> q1(1.0f, 2.0f, 3.0f, 4.0f);
    Quaternion<float> q2(1.0f, 2.0f, 3.0f, 4.0f);
    CONSTEXPR_REQUIRE(q1 == q2);
    return true;
}
static_assert(test_equality());

// ---------------------------------------------------------------------
// can_scalar_multiply / can_scalar_divide / can_component_add /
// can_component_subtract / can_negate / can_lerp
// ---------------------------------------------------------------------
constexpr bool test_capability_tags() {
    Quaternion<float>::can_scalar_multiply();
    Quaternion<float>::can_scalar_divide();
    Quaternion<float>::can_component_add();
    Quaternion<float>::can_component_subtract();
    Quaternion<float>::can_negate();
    Quaternion<float>::can_lerp();
    return true;
}
static_assert(test_capability_tags());
