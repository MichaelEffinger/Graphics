#include "constexpr_require.hpp"
#include "../ES_math.hpp"
#include "../Matrix.hpp"
#include "../Transform.hpp"

import ES.VectorN;

using namespace ES;

// ---------------------------------------------------------------------
// default constructor
// ---------------------------------------------------------------------
constexpr bool test_default_constructor() {
    Transform<float, 3> transform;

    auto mat = transform.matrix();
    auto inv = transform.inverse();

    CONSTEXPR_REQUIRE(mat(0,0) == 1.0f);
    CONSTEXPR_REQUIRE(mat(1,1) == 1.0f);
    CONSTEXPR_REQUIRE(mat(2,2) == 1.0f);
    CONSTEXPR_REQUIRE(inv(0,0) == 1.0f);
    CONSTEXPR_REQUIRE(inv(1,1) == 1.0f);
    CONSTEXPR_REQUIRE(inv(2,2) == 1.0f);

    return true;
}
static_assert(test_default_constructor());

// ---------------------------------------------------------------------
// constructor from matrix (exercises the internal .inverse() call)
// ---------------------------------------------------------------------
constexpr bool test_constructor_from_matrix() {
    Matrix<float, 2, 2> m;
    m(0,0) = 2.0f; m(0,1) = 0.0f;
    m(1,0) = 0.0f; m(1,1) = 2.0f;

    Transform<float, 2> transform(m);

    auto mat = transform.matrix();
    auto inv = transform.inverse();

    CONSTEXPR_REQUIRE(mat(0,0) == 2.0f);
    CONSTEXPR_REQUIRE(mat(1,1) == 2.0f);
    CONSTEXPR_REQUIRE(ES::math::approx_equal(inv(0,0), 0.5f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(inv(1,1), 0.5f));

    return true;
}
static_assert(test_constructor_from_matrix());

// ---------------------------------------------------------------------
// constructor from matrix and inverse (two-argument, no internal
// .inverse() call - just stores what it's given)
// ---------------------------------------------------------------------
constexpr bool test_constructor_from_matrix_and_inverse() {
    Matrix<float, 2, 2> m;
    m(0,0) = 2.0f; m(0,1) = 0.0f;
    m(1,0) = 0.0f; m(1,1) = 2.0f;

    Matrix<float, 2, 2> i;
    i(0,0) = 0.5f; i(0,1) = 0.0f;
    i(1,0) = 0.0f; i(1,1) = 0.5f;

    Transform<float, 2> transform(m, i);

    auto mat = transform.matrix();
    auto inv = transform.inverse();

    CONSTEXPR_REQUIRE(mat(0,0) == 2.0f);
    CONSTEXPR_REQUIRE(inv(0,0) == 0.5f);

    return true;
}
static_assert(test_constructor_from_matrix_and_inverse());

// ---------------------------------------------------------------------
// matrix() accessor
// ---------------------------------------------------------------------
constexpr bool test_matrix_accessor() {
    Matrix<float, 3, 3> m = Matrix<float, 3, 3>::identity();
    m(0,0) = 3.0f;

    Transform<float, 3> transform(m);

    auto mat = transform.matrix();

    CONSTEXPR_REQUIRE(mat(0,0) == 3.0f);
    CONSTEXPR_REQUIRE(mat(1,1) == 1.0f);
    CONSTEXPR_REQUIRE(mat(2,2) == 1.0f);

    return true;
}
static_assert(test_matrix_accessor());

// ---------------------------------------------------------------------
// inverse() accessor
// ---------------------------------------------------------------------
constexpr bool test_inverse_accessor() {
    Matrix<float, 2, 2> m;
    m(0,0) = 4.0f; m(0,1) = 0.0f;
    m(1,0) = 0.0f; m(1,1) = 4.0f;

    Transform<float, 2> transform(m);

    auto inv = transform.inverse();

    CONSTEXPR_REQUIRE(ES::math::approx_equal(inv(0,0), 0.25f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(inv(1,1), 0.25f));

    return true;
}
static_assert(test_inverse_accessor());

// ---------------------------------------------------------------------
// apply
// ---------------------------------------------------------------------
constexpr bool test_apply() {
    Matrix<float, 2, 2> m;
    m(0,0) = 2.0f; m(0,1) = 0.0f;
    m(1,0) = 0.0f; m(1,1) = 3.0f;

    Transform<float, 2> transform(m);

    Vector2<float> vec(1.0f, 1.0f);
    auto result = transform.apply(vec);

    CONSTEXPR_REQUIRE(result[0] == 2.0f);
    CONSTEXPR_REQUIRE(result[1] == 3.0f);

    return true;
}
static_assert(test_apply());

// ---------------------------------------------------------------------
// apply_inverse
// ---------------------------------------------------------------------
constexpr bool test_apply_inverse() {
    Matrix<float, 2, 2> m;
    m(0,0) = 2.0f; m(0,1) = 0.0f;
    m(1,0) = 0.0f; m(1,1) = 4.0f;

    Transform<float, 2> transform(m);

    Vector2<float> vec(4.0f, 8.0f);
    auto result = transform.apply_inverse(vec);

    CONSTEXPR_REQUIRE(ES::math::approx_equal(result[0], 2.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(result[1], 2.0f));

    return true;
}
static_assert(test_apply_inverse());

// ---------------------------------------------------------------------
// operator*
// ---------------------------------------------------------------------
constexpr bool test_operator_multiply() {
    Matrix<float, 2, 2> m1;
    m1(0,0) = 2.0f; m1(0,1) = 0.0f;
    m1(1,0) = 0.0f; m1(1,1) = 2.0f;

    Matrix<float, 2, 2> m2;
    m2(0,0) = 3.0f; m2(0,1) = 0.0f;
    m2(1,0) = 0.0f; m2(1,1) = 3.0f;

    Transform<float, 2> t1(m1);
    Transform<float, 2> t2(m2);

    auto combined = t1 * t2;

    auto mat = combined.matrix();

    CONSTEXPR_REQUIRE(ES::math::approx_equal(mat(0,0), 6.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(mat(1,1), 6.0f));

    return true;
}
static_assert(test_operator_multiply());

// ---------------------------------------------------------------------
// operator* inverse correctness
// ---------------------------------------------------------------------
constexpr bool test_operator_multiply_inverse_correctness() {
    Matrix<float, 2, 2> m1;
    m1(0,0) = 2.0f; m1(0,1) = 0.0f;
    m1(1,0) = 0.0f; m1(1,1) = 2.0f;

    Matrix<float, 2, 2> m2;
    m2(0,0) = 3.0f; m2(0,1) = 0.0f;
    m2(1,0) = 0.0f; m2(1,1) = 3.0f;

    Transform<float, 2> t1(m1);
    Transform<float, 2> t2(m2);

    auto combined = t1 * t2;
    auto inv = combined.inverse();

    CONSTEXPR_REQUIRE(ES::math::approx_equal(inv(0,0), 1.0f/6.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(inv(1,1), 1.0f/6.0f));

    return true;
}
static_assert(test_operator_multiply_inverse_correctness());

// ---------------------------------------------------------------------
// operator*=
// ---------------------------------------------------------------------
constexpr bool test_operator_multiply_equals() {
    Matrix<float, 2, 2> m1;
    m1(0,0) = 2.0f; m1(0,1) = 0.0f;
    m1(1,0) = 0.0f; m1(1,1) = 2.0f;

    Matrix<float, 2, 2> m2;
    m2(0,0) = 3.0f; m2(0,1) = 0.0f;
    m2(1,0) = 0.0f; m2(1,1) = 3.0f;

    Transform<float, 2> t1(m1);
    Transform<float, 2> t2(m2);

    t1 *= t2;

    auto mat = t1.matrix();

    CONSTEXPR_REQUIRE(ES::math::approx_equal(mat(0,0), 6.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(mat(1,1), 6.0f));

    return true;
}
static_assert(test_operator_multiply_equals());

// ---------------------------------------------------------------------
// invert
// ---------------------------------------------------------------------
constexpr bool test_invert() {
    Matrix<float, 2, 2> m;
    m(0,0) = 2.0f; m(0,1) = 0.0f;
    m(1,0) = 0.0f; m(1,1) = 2.0f;

    Transform<float, 2> transform(m);
    auto inverted = transform.invert();

    auto mat = inverted.matrix();
    auto inv = inverted.inverse();

    CONSTEXPR_REQUIRE(ES::math::approx_equal(mat(0,0), 0.5f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(mat(1,1), 0.5f));
    CONSTEXPR_REQUIRE(inv(0,0) == 2.0f);
    CONSTEXPR_REQUIRE(inv(1,1) == 2.0f);

    return true;
}
static_assert(test_invert());

// ---------------------------------------------------------------------
// invert_in_place
// ---------------------------------------------------------------------
constexpr bool test_invert_in_place() {
    Matrix<float, 2, 2> m;
    m(0,0) = 2.0f; m(0,1) = 0.0f;
    m(1,0) = 0.0f; m(1,1) = 2.0f;

    Transform<float, 2> transform(m);
    transform.invert_in_place();

    auto mat = transform.matrix();
    auto inv = transform.inverse();

    CONSTEXPR_REQUIRE(ES::math::approx_equal(mat(0,0), 0.5f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(mat(1,1), 0.5f));
    CONSTEXPR_REQUIRE(inv(0,0) == 2.0f);
    CONSTEXPR_REQUIRE(inv(1,1) == 2.0f);

    return true;
}
static_assert(test_invert_in_place());

// ---------------------------------------------------------------------
// identity static method
// ---------------------------------------------------------------------
constexpr bool test_identity_static() {
    auto identity = Transform<float, 3>::identity();

    auto mat = identity.matrix();

    CONSTEXPR_REQUIRE(mat(0,0) == 1.0f);
    CONSTEXPR_REQUIRE(mat(1,1) == 1.0f);
    CONSTEXPR_REQUIRE(mat(2,2) == 1.0f);
    CONSTEXPR_REQUIRE(mat(0,1) == 0.0f);
    CONSTEXPR_REQUIRE(mat(1,2) == 0.0f);

    return true;
}
static_assert(test_identity_static());

// ---------------------------------------------------------------------
// is_identity true
// ---------------------------------------------------------------------
constexpr bool test_is_identity_true() {
    Transform<float, 3> transform;
    CONSTEXPR_REQUIRE(transform.is_identity() == true);
    return true;
}
static_assert(test_is_identity_true());

// ---------------------------------------------------------------------
// is_identity false
// ---------------------------------------------------------------------
constexpr bool test_is_identity_false() {
    Matrix<float, 3, 3> m = Matrix<float, 3, 3>::identity();
    m(0,0) = 2.0f;

    Transform<float, 3> transform(m);

    CONSTEXPR_REQUIRE(transform.is_identity() == false);

    return true;
}
static_assert(test_is_identity_false());

// ---------------------------------------------------------------------
// translation 2D
// ---------------------------------------------------------------------
constexpr bool test_translation_2d() {
    Vector2<float> t(3.0f, 4.0f);

    auto transform = Transform<float, 3>::translation(t);

    Vector3<float> point(1.0f, 1.0f, 1.0f);
    auto result = transform.apply(point);

    CONSTEXPR_REQUIRE(ES::math::approx_equal(result[0], 4.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(result[1], 5.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(result[2], 1.0f));

    return true;
}
static_assert(test_translation_2d());

// ---------------------------------------------------------------------
// translation 3D
// ---------------------------------------------------------------------
constexpr bool test_translation_3d() {
    Vector3<float> t(2.0f, 3.0f, 4.0f);

    auto transform = Transform<float, 4>::translation(t);

    Vector4<float> point(1.0f, 1.0f, 1.0f, 1.0f);
    auto result = transform.apply(point);

    CONSTEXPR_REQUIRE(ES::math::approx_equal(result[0], 3.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(result[1], 4.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(result[2], 5.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(result[3], 1.0f));

    return true;
}
static_assert(test_translation_3d());

// ---------------------------------------------------------------------
// scale
// ---------------------------------------------------------------------
constexpr bool test_scale() {
    Vector3<float> s(2.0f, 3.0f, 4.0f);

    auto transform = Transform<float, 3>::scale(s);

    Vector3<float> vec(1.0f, 1.0f, 1.0f);
    auto result = transform.apply(vec);

    CONSTEXPR_REQUIRE(result[0] == 2.0f);
    CONSTEXPR_REQUIRE(result[1] == 3.0f);
    CONSTEXPR_REQUIRE(result[2] == 4.0f);

    return true;
}
static_assert(test_scale());

// ---------------------------------------------------------------------
// uniform_scale
// ---------------------------------------------------------------------
constexpr bool test_uniform_scale() {
    auto transform = Transform<float, 3>::uniform_scale(5.0f);

    Vector3<float> vec(1.0f, 2.0f, 3.0f);
    auto result = transform.apply(vec);

    CONSTEXPR_REQUIRE(result[0] == 5.0f);
    CONSTEXPR_REQUIRE(result[1] == 10.0f);
    CONSTEXPR_REQUIRE(result[2] == 15.0f);

    return true;
}
static_assert(test_uniform_scale());

// ---------------------------------------------------------------------
// uniform_scale inverse
// ---------------------------------------------------------------------
constexpr bool test_uniform_scale_inverse() {
    auto transform = Transform<float, 3>::uniform_scale(4.0f);

    Vector3<float> vec(8.0f, 12.0f, 16.0f);
    auto result = transform.apply_inverse(vec);

    CONSTEXPR_REQUIRE(ES::math::approx_equal(result[0], 2.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(result[1], 3.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(result[2], 4.0f));

    return true;
}
static_assert(test_uniform_scale_inverse());

// ---------------------------------------------------------------------
// composition scale then translate
// ---------------------------------------------------------------------
constexpr bool test_composition_scale_then_translate() {
    Vector2<float> t(1.0f, 1.0f);
    Vector3<float> s(2.0f, 2.0f, 1.0f);

    auto scale_transform = Transform<float, 3>::scale(s);
    auto translate_transform = Transform<float, 3>::translation(t);

    auto combined = translate_transform * scale_transform;

    Vector3<float> point(1.0f, 1.0f, 1.0f);
    auto result = combined.apply(point);

    CONSTEXPR_REQUIRE(ES::math::approx_equal(result[0], 3.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(result[1], 3.0f));

    return true;
}
static_assert(test_composition_scale_then_translate());

// ---------------------------------------------------------------------
// apply and apply_inverse round trip
// ---------------------------------------------------------------------
constexpr bool test_apply_apply_inverse_round_trip() {
    Vector3<float> s(2.0f, 3.0f, 4.0f);
    auto transform = Transform<float, 3>::scale(s);

    Vector3<float> original(5.0f, 6.0f, 7.0f);
    auto transformed = transform.apply(original);
    auto back = transform.apply_inverse(transformed);

    CONSTEXPR_REQUIRE(ES::math::approx_equal(back[0], original[0]));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(back[1], original[1]));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(back[2], original[2]));

    return true;
}
static_assert(test_apply_apply_inverse_round_trip());

// ---------------------------------------------------------------------
// composition associativity
// ---------------------------------------------------------------------
constexpr bool test_composition_associativity() {
    Matrix<float, 2, 2> m1;
    m1(0,0) = 2.0f; m1(0,1) = 0.0f;
    m1(1,0) = 0.0f; m1(1,1) = 2.0f;

    Matrix<float, 2, 2> m2;
    m2(0,0) = 3.0f; m2(0,1) = 0.0f;
    m2(1,0) = 0.0f; m2(1,1) = 3.0f;

    Matrix<float, 2, 2> m3;
    m3(0,0) = 4.0f; m3(0,1) = 0.0f;
    m3(1,0) = 0.0f; m3(1,1) = 4.0f;

    Transform<float, 2> t1(m1);
    Transform<float, 2> t2(m2);
    Transform<float, 2> t3(m3);

    auto result1 = (t1 * t2) * t3;
    auto result2 = t1 * (t2 * t3);

    Vector2<float> vec(1.0f, 1.0f);
    auto r1 = result1.apply(vec);
    auto r2 = result2.apply(vec);

    CONSTEXPR_REQUIRE(ES::math::approx_equal(r1[0], r2[0]));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(r1[1], r2[1]));

    return true;
}
static_assert(test_composition_associativity());

// ---------------------------------------------------------------------
// double invert
// ---------------------------------------------------------------------
constexpr bool test_double_invert() {
    Matrix<float, 2, 2> m;
    m(0,0) = 2.0f; m(0,1) = 0.0f;
    m(1,0) = 0.0f; m(1,1) = 3.0f;

    Transform<float, 2> transform(m);
    auto double_inverted = transform.invert().invert();

    auto mat = double_inverted.matrix();

    CONSTEXPR_REQUIRE(ES::math::approx_equal(mat(0,0), 2.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(mat(1,1), 3.0f));

    return true;
}
static_assert(test_double_invert());

// ---------------------------------------------------------------------
// normal_matrix
// ---------------------------------------------------------------------
/*TODO
constexpr bool test_normal_matrix() {
    Vector3<float> s(2.0f, 4.0f, 8.0f);
    auto scaled = Transform<float, 3>::scale(s);
    auto normalMat = scaled.normal_matrix();
    auto normalMatAgain = scaled.normal_matrix();
    CONSTEXPR_REQUIRE(normalMat.almost_equal(normalMatAgain));
    return true;
}

    make constexpr someday
    static_assert(test_normal_matrix());
*/
