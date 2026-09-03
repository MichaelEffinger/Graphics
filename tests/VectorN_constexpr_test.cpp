#include "constexpr_require.hpp"

import ES_math;
import ES.VectorN;

using namespace ES;

// ---------------------------------------------------------------------
// VectorN constructors
// ---------------------------------------------------------------------
constexpr bool test_constructors() {
    Vector3<float> v1{1.0f, 2.0f, 3.0f};
    CONSTEXPR_REQUIRE(v1[0] == 1.0f);
    CONSTEXPR_REQUIRE(v1[1] == 2.0f);
    CONSTEXPR_REQUIRE(v1[2] == 3.0f);

    VectorN<float, 4>(5.0f, 4.0f, 5, 4);
    VectorN<float, 3>(5, 4, 3);

    Vector3<float>(3, 2, 1);

    Vector2<float> v2{4.0f, 5.0f};
    Vector3<float> v3{4.0f, 5.0f, 6.0f};
    Vector3<float> v3parameter(4.0f, 5.0f, 3.0f);
    Vector3<float> v3Int(4, 5, 6);

    CONSTEXPR_REQUIRE(v3 == v3Int);

    Vector3<float> v35(v2, 6.0f);
    VectorN<float, 10> v10(v3, 4, 5, 6, 7, 8, 9, 10);

    CONSTEXPR_REQUIRE(v35[0] == 4.0f);
    CONSTEXPR_REQUIRE(v35[1] == 5.0f);
    CONSTEXPR_REQUIRE(v35[2] == 6.0f);

    auto v4 = Vector3<float>{1.0f, 2.0f, 3.0f};
    auto v5 = Vector4<float>(v4, 4.0f);
    CONSTEXPR_REQUIRE(v5[3] == 4.0f);

    return true;
}
static_assert(test_constructors());

// ---------------------------------------------------------------------
// accessors - x, y, z, w
// ---------------------------------------------------------------------
constexpr bool test_accessors() {
    Vector4<float> v{1.0f, 2.0f, 3.0f, 4.0f};

    CONSTEXPR_REQUIRE(v.x() == 1.0f);
    CONSTEXPR_REQUIRE(v.y() == 2.0f);
    CONSTEXPR_REQUIRE(v.z() == 3.0f);
    CONSTEXPR_REQUIRE(v.w() == 4.0f);

    v.x() = 10.0f;
    v.y() = 20.0f;
    v.z() = 30.0f;
    v.w() = 40.0f;

    CONSTEXPR_REQUIRE(v.x() == 10.0f);
    CONSTEXPR_REQUIRE(v.y() == 20.0f);
    CONSTEXPR_REQUIRE(v.z() == 30.0f);
    CONSTEXPR_REQUIRE(v.w() == 40.0f);

    return true;
}
static_assert(test_accessors());

// ---------------------------------------------------------------------
// arithmetic operators
// ---------------------------------------------------------------------
constexpr bool test_arithmetic_operators() {
    Vector3<float> a{1.0f, 2.0f, 3.0f};
    Vector3<float> b{3.0f, 2.0f, 1.0f};
    Vector3<float> zero(0, 0, 0);

    CONSTEXPR_REQUIRE(b.data() == b.data());
    auto c = a + b;
    CONSTEXPR_REQUIRE(c[0] == 4.0f);
    CONSTEXPR_REQUIRE(c[1] == 4.0f);
    CONSTEXPR_REQUIRE(c[2] == 4.0f);
    CONSTEXPR_REQUIRE(c.at(0) == 4.0f);
    CONSTEXPR_REQUIRE(c.at(1) == 4.0f);
    CONSTEXPR_REQUIRE(c.at(2) == 4.0f);
    CONSTEXPR_REQUIRE(c.x() == 4.0f);
    CONSTEXPR_REQUIRE(c.y() == 4.0f);
    CONSTEXPR_REQUIRE(c.z() == 4.0f);

    auto d = c - b;
    CONSTEXPR_REQUIRE(a == d);

    a += b;
    CONSTEXPR_REQUIRE(a == c);

    auto e = -b;
    CONSTEXPR_REQUIRE(e[0] == -3.0f);
    CONSTEXPR_REQUIRE(e[1] == -2.0f);
    CONSTEXPR_REQUIRE(e[2] == -1.0f);

    auto f = b * 2.0f;
    CONSTEXPR_REQUIRE(f[0] == 6.0f);
    CONSTEXPR_REQUIRE(f[1] == 4.0f);
    CONSTEXPR_REQUIRE(f[2] == 2.0f);

    f /= 2.0f;
    CONSTEXPR_REQUIRE(f == b);

#ifdef NDEBUG
    auto g = e / 0;
    CONSTEXPR_REQUIRE(g == zero);
#endif

    return true;
}
static_assert(test_arithmetic_operators());

// ---------------------------------------------------------------------
// scalar multiplication with scalar on left
// ---------------------------------------------------------------------
constexpr bool test_scalar_mult_left() {
    Vector3<float> v{1.0f, 2.0f, 3.0f};
    auto result = 2.0f * v;
    CONSTEXPR_REQUIRE(result[0] == 2.0f);
    CONSTEXPR_REQUIRE(result[1] == 4.0f);
    CONSTEXPR_REQUIRE(result[2] == 6.0f);
    return true;
}
static_assert(test_scalar_mult_left());

// ---------------------------------------------------------------------
// -= operator
// ---------------------------------------------------------------------
constexpr bool test_minus_equals() {
    Vector3<float> a{5.0f, 6.0f, 7.0f};
    Vector3<float> b{1.0f, 2.0f, 3.0f};
    a -= b;
    CONSTEXPR_REQUIRE(a == Vector3<float>{4.0f, 4.0f, 4.0f});
    return true;
}
static_assert(test_minus_equals());

// ---------------------------------------------------------------------
// *= operator
// ---------------------------------------------------------------------
constexpr bool test_times_equals() {
    Vector3<float> v{2.0f, 3.0f, 4.0f};
    v *= 3.0f;
    CONSTEXPR_REQUIRE(v == Vector3<float>{6.0f, 9.0f, 12.0f});
    return true;
}
static_assert(test_times_equals());

// ---------------------------------------------------------------------
// dot and cross
// ---------------------------------------------------------------------
constexpr bool test_dot_and_cross() {
    Vector3<float> a{1.0f, 0.0f, 0.0f};
    Vector3<float> b{0.0f, 1.0f, 0.0f};
    CONSTEXPR_REQUIRE(a.dot(b) == 0.0f);
    auto c = a.cross(b);
    CONSTEXPR_REQUIRE(c == Vector3<float>{0.0f, 0.0f, 1.0f});
    Vector3<float> d{0.0f, 1.0f, 0.0f};
    d.cross_in_place(a);
    CONSTEXPR_REQUIRE(d == Vector3<float>{0.0f, 0.0f, -1.0f});
    return true;
}
static_assert(test_dot_and_cross());

// ---------------------------------------------------------------------
// magnitude, normalize, almost_equal, math::approx_equal
// ---------------------------------------------------------------------
/* TODO
constexpr bool test_magnitude_normalize_almost_equal() {
    Vector3<float> a{3.0f, 4.0f, 0.0f};
    Vector3<float> b = a * .1f;
    b /= .1f;

    CONSTEXPR_REQUIRE(a.almost_equal(b) == true);

    float five = 5.0f + 1e-6f;
    CONSTEXPR_REQUIRE(ES::math::approx_equal(five, 5));

    CONSTEXPR_REQUIRE(ES::math::approx_equal(a.magnitude(), 5.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(a.magnitude_squared(), 25.0f));

    auto norm = a.normalize();
    CONSTEXPR_REQUIRE(ES::math::approx_equal(norm.magnitude(), 1.0f));

    a.normalize_in_place();
    CONSTEXPR_REQUIRE(ES::math::approx_equal(a.magnitude(), 1.0f));

    return true;
}
    not constexpr yet
    static_assert(test_magnitude_normalize_almost_equal());
*/

// ---------------------------------------------------------------------
// hadamard and scalar operations
// ---------------------------------------------------------------------
constexpr bool test_hadamard() {
    Vector3<float> a{1.0f, 2.0f, 3.0f};
    Vector3<float> b{2.0f, 3.0f, 4.0f};
    Vector3<float> adouble = a * 2;
    Vector3<float> asave = a;
    Vector3<float> zero(0.0f, 0.0f, 0.0f);

    auto c = a.hadamard_product(b);
    CONSTEXPR_REQUIRE(c == Vector3<float>{2.0f, 6.0f, 12.0f});

    a.hadamard_product_in_place(b);
    CONSTEXPR_REQUIRE(a == c);

    auto d = adouble.hadamard_divide(asave);
    CONSTEXPR_REQUIRE(d == Vector3<float>{2.0f, 2.0f, 2.0f});

    CONSTEXPR_REQUIRE(d == adouble.hadamard_divide_in_place(asave));

#ifdef NDEBUG
    a.hadamard_divide_in_place(zero);
    CONSTEXPR_REQUIRE(a == zero);
#endif

    return true;
}
static_assert(test_hadamard());

// ---------------------------------------------------------------------
// angle between vectors
// ---------------------------------------------------------------------
/* TODO
constexpr bool test_angle() {
    Vector3<float> a{1.0f, 0.0f, 0.0f};
    Vector3<float> b{0.0f, 1.0f, 0.0f};

    AngleRad angle = a.angle(b);
    (void)angle;

    Vector3<float> parallel{2.0f, 0.0f, 0.0f};
    angle = a.angle(parallel);
    CONSTEXPR_REQUIRE(ES::math::approx_equal(angle.get(), 0.0f));

    Vector3<float> opposite{-1.0f, 0.0f, 0.0f};
    angle = a.angle(opposite);
    constexpr float pi = ES::AngleRad(ES::AngleDeg{180.f}).get();
    CONSTEXPR_REQUIRE(ES::math::approx_equal(angle.get(), pi));

    return true;
}
 not constexpr yet
 static_assert(test_angle());
*/

// ---------------------------------------------------------------------
// << operator (vector concatenation, not stream insertion)
// ---------------------------------------------------------------------
constexpr bool test_shift_operator() {
    Vector2<float> v2{1.0f, 2.0f};
    auto v3 = v2 << 3.0f;
    CONSTEXPR_REQUIRE(v3 == Vector3<float>{1.0f, 2.0f, 3.0f});
    return true;
}
static_assert(test_shift_operator());

// ---------------------------------------------------------------------
// type conversion
// ---------------------------------------------------------------------
constexpr bool test_type_conversion() {
    Vector3<float> v3{1.0f, 2.0f, 3.0f};

    Vector2<float> v2 = static_cast<VectorN<float, 2>>(v3);
    CONSTEXPR_REQUIRE(v2 == Vector2<float>{1.0f, 2.0f});

    auto v5 = static_cast<VectorN<float, 5>>(v3);
    CONSTEXPR_REQUIRE(v5[0] == 1.0f);
    CONSTEXPR_REQUIRE(v5[1] == 2.0f);
    CONSTEXPR_REQUIRE(v5[2] == 3.0f);
    CONSTEXPR_REQUIRE(v5[3] == 0.0f);
    CONSTEXPR_REQUIRE(v5[4] == 0.0f);

    return true;
}
static_assert(test_type_conversion());

// ---------------------------------------------------------------------
// clamp and almost_equal
// ---------------------------------------------------------------------
constexpr bool test_clamp_and_almost_equal() {
    Vector3<float> v{1.0f, -1.0f, 5.0f};
    auto clamped = v.clamp(-0.5f, 2.0f);
    CONSTEXPR_REQUIRE(clamped == Vector3<float>{1.0f, -0.5f, 2.0f});

    v.clamp_in_place(-1.0f, 1.0f);
    CONSTEXPR_REQUIRE(v == Vector3<float>{1.0f, -1.0f, 1.0f});

    Vector3<float> a{0.0f, 0.1f, 0.2f};
    Vector3<float> b{0.0f, 0.1f, 0.2f};
    CONSTEXPR_REQUIRE(a.almost_equal(b, 0.01f));

    return true;
}
static_assert(test_clamp_and_almost_equal());

// ---------------------------------------------------------------------
// projection, reflection, refraction
// ---------------------------------------------------------------------
/* TODO
constexpr bool test_project_reflect_refract() {
    Vector3<float> a{1.0f, 0.0f, 0.0f};
    Vector3<float> b{0.0f, 1.0f, 0.0f};

    auto proj = a.project_onto(b);
    CONSTEXPR_REQUIRE(proj == Vector3<float>{0.0f, 0.0f, 0.0f});

    Vector3<float> n{0.0f, 1.0f, 0.0f};
    auto refl = a.reflect_safe(n);
    CONSTEXPR_REQUIRE(refl == Vector3<float>{1.0f, 0.0f, 0.0f});

    Vector3<float> incoming{0.0f, -1.0f, 0.0f};
    Vector3<float> normal{0.0f, 1.0f, 0.0f};
    auto refr = incoming.refract_safe(normal, 1.0f, 1.5);
    CONSTEXPR_REQUIRE(refr.magnitude() <= 1.0f);

    return true;
}

    not constexpr yet
    static_assert(test_project_reflect_refract());
*/

// ---------------------------------------------------------------------
// lerp
// ---------------------------------------------------------------------
constexpr bool test_lerp() {
    Vector3<float> a{0.0f, 0.0f, 0.0f};
    Vector3<float> b{10.0f, 20.0f, 30.0f};

    auto mid = a.lerp(b, 0.5f);
    CONSTEXPR_REQUIRE(mid == Vector3<float>{5.0f, 10.0f, 15.0f});

    auto start = a.lerp(b, 0.0f);
    CONSTEXPR_REQUIRE(start == a);

    auto end = a.lerp(b, 1.0f);
    CONSTEXPR_REQUIRE(end == b);

    auto quarter = a.lerp(b, 0.25f);
    CONSTEXPR_REQUIRE(quarter == Vector3<float>{2.5f, 5.0f, 7.5f});

    return true;
}
static_assert(test_lerp());

// ---------------------------------------------------------------------
// lerp_in_place
// ---------------------------------------------------------------------
constexpr bool test_lerp_in_place() {
    Vector3<float> a{0.0f, 0.0f, 0.0f};
    Vector3<float> b{10.0f, 20.0f, 30.0f};
    a.lerp_in_place(b, 0.5f);
    CONSTEXPR_REQUIRE(a == Vector3<float>{5.0f, 10.0f, 15.0f});
    return true;
}
static_assert(test_lerp_in_place());

// ---------------------------------------------------------------------
// reflect
// ---------------------------------------------------------------------
/* TODO
constexpr bool test_reflect() {
    Vector3<float> incoming{1.0f, -1.0f, 0.0f};
    incoming.normalize_in_place();
    Vector3<float> normal{0.0f, 1.0f, 0.0f};

    auto reflected = incoming.reflect(normal);

    CONSTEXPR_REQUIRE(ES::math::approx_equal(reflected.y(), incoming.y() * -1.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(reflected.magnitude(), incoming.magnitude()));

    return true;
}
    not constexpr yet
    static_assert(test_reflect());
*/

// ---------------------------------------------------------------------
// reflect_in_place
// ---------------------------------------------------------------------
/* TODO
constexpr bool test_reflect_in_place() {
    Vector3<float> incoming{1.0f, -1.0f, 0.0f};
    incoming.normalize_in_place();
    float orig_y = incoming.y();
    Vector3<float> normal{0.0f, 1.0f, 0.0f};

    incoming.reflect_in_place(normal);

    CONSTEXPR_REQUIRE(ES::math::approx_equal(incoming.y(), orig_y * -1.0f));

    return true;
}
    not constexpr yet
    static_assert(test_reflect_in_place());
*/

// ---------------------------------------------------------------------
// reflect_safe
// ---------------------------------------------------------------------
/* TODO
constexpr bool test_reflect_safe() {
    Vector3<float> incoming{1.0f, -1.0f, 0.0f};
    Vector3<float> normal{0.0f, 5.0f, 0.0f};

    auto reflected = incoming.reflect_safe(normal);

    CONSTEXPR_REQUIRE(reflected.magnitude() > 0.0f);

    return true;
}
    not constexpr yet
    static_assert(test_reflect_safe());
*/

// ---------------------------------------------------------------------
// reflect_in_place_safe
// ---------------------------------------------------------------------
/* TODO
constexpr bool test_reflect_in_place_safe() {
    Vector3<float> incoming{1.0f, -1.0f, 0.0f};
    Vector3<float> normal{0.0f, 5.0f, 0.0f};

    incoming.reflect_in_place_safe(normal);

    CONSTEXPR_REQUIRE(incoming.magnitude() > 0.0f);

    return true;
}
    not constexpr yet
    static_assert(test_reflect_in_place_safe());
*/

// ---------------------------------------------------------------------
// refract
// ---------------------------------------------------------------------
/* TODO
constexpr bool test_refract() {
    Vector3<float> incoming{0.0f, -1.0f, 0.0f};
    Vector3<float> normal{0.0f, 1.0f, 0.0f};

    auto refracted = incoming.refract(normal, 1.0f, 1.5f);
    CONSTEXPR_REQUIRE(refracted.magnitude() > 0.0f);

    Vector3<float> steep{0.9f, -0.1f, 0.0f};
    steep.normalize_in_place();
    auto tir = steep.refract(normal, 1.5f, 1.0f);
    CONSTEXPR_REQUIRE(tir.magnitude_squared() >= 0.0f);

    return true;
}
    not constexpr yet
    static_assert(test_refract());
*/

// ---------------------------------------------------------------------
// refract_in_place
// ---------------------------------------------------------------------
/* TODO
constexpr bool test_refract_in_place() {
    Vector3<float> incoming{0.0f, -1.0f, 0.0f};
    Vector3<float> normal{0.0f, 1.0f, 0.0f};

    incoming.refract_in_place(normal, 1.0f, 1.5f);

    CONSTEXPR_REQUIRE(incoming.magnitude() > 0.0f);

    return true;
}
    not constexpr yet
    static_assert(test_refract_in_place());
*/

// ---------------------------------------------------------------------
// refract_safe
// ---------------------------------------------------------------------
/* TODO
constexpr bool test_refract_safe() {
    Vector3<float> incoming{0.0f, -5.0f, 0.0f};
    Vector3<float> normal{0.0f, 3.0f, 0.0f};

    auto refracted = incoming.refract_safe(normal, 1.0f, 1.5f);

    CONSTEXPR_REQUIRE(refracted.magnitude() >= 0.0f);

    return true;
}
    not constexpr yet
static_assert(test_refract_safe());
*/

// ---------------------------------------------------------------------
// refract_in_place_safe
// ---------------------------------------------------------------------
/* TODO
constexpr bool test_refract_in_place_safe() {
    Vector3<float> incoming{0.0f, -5.0f, 0.0f};
    Vector3<float> normal{0.0f, 3.0f, 0.0f};

    incoming.refract_in_place_safe(normal, 1.0f, 1.5f);

    CONSTEXPR_REQUIRE(incoming.magnitude() >= 0.0f);

    return true;
}
    not constexpr yet
static_assert(test_refract_in_place_safe());
*/

// ---------------------------------------------------------------------
// distance
// ---------------------------------------------------------------------
/* TODO
constexpr bool test_distance() {
    Vector3<float> a{0.0f, 0.0f, 0.0f};
    Vector3<float> b{3.0f, 4.0f, 0.0f};

    float dist = a.distance(b);

    CONSTEXPR_REQUIRE(ES::math::approx_equal(dist, 5.0f));

    return true;
}
    not constexpr yet
static_assert(test_distance());
*/

// ---------------------------------------------------------------------
// distance_squared
// ---------------------------------------------------------------------
constexpr bool test_distance_squared() {
    Vector3<float> a{0.0f, 0.0f, 0.0f};
    Vector3<float> b{3.0f, 4.0f, 0.0f};

    float distSq = a.distance_squared(b);

    CONSTEXPR_REQUIRE(ES::math::approx_equal(distSq, 25.0f));

    return true;
}
static_assert(test_distance_squared());

// ---------------------------------------------------------------------
// project_onto
// ---------------------------------------------------------------------
constexpr bool test_project_onto() {
    Vector3<float> a{3.0f, 4.0f, 0.0f};
    Vector3<float> b{1.0f, 0.0f, 0.0f};

    auto proj = a.project_onto(b);

    CONSTEXPR_REQUIRE(proj == Vector3<float>{3.0f, 0.0f, 0.0f});

    return true;
}
static_assert(test_project_onto());

// ---------------------------------------------------------------------
// project_onto_in_place
// ---------------------------------------------------------------------
constexpr bool test_project_onto_in_place() {
    Vector3<float> a{3.0f, 4.0f, 0.0f};
    Vector3<float> b{1.0f, 0.0f, 0.0f};

    a.project_onto_in_place(b);

    CONSTEXPR_REQUIRE(a == Vector3<float>{3.0f, 0.0f, 0.0f});

    return true;
}
static_assert(test_project_onto_in_place());

// ---------------------------------------------------------------------
// slerp
// ---------------------------------------------------------------------
/* TODO
constexpr bool test_slerp() {
    Vector3<float> a{1.0f, 0.0f, 0.0f};
    Vector3<float> b{0.0f, 1.0f, 0.0f};

    auto mid = a.slerp(b, 0.5f);

    CONSTEXPR_REQUIRE(ES::math::approx_equal(mid.magnitude(), 1.0f));
    CONSTEXPR_REQUIRE(mid.x() > 0.0f);
    CONSTEXPR_REQUIRE(mid.y() > 0.0f);

    auto start = a.slerp(b, 0.0f);
    CONSTEXPR_REQUIRE(ES::math::approx_equal(start.x(), a.x()));

    auto end = a.slerp(b, 1.0f);
    CONSTEXPR_REQUIRE(ES::math::approx_equal(end.y(), b.y()));

    return true;
}
    not constexpr yet
static_assert(test_slerp());
*/

// ---------------------------------------------------------------------
// zero - static zero vector creation
// ---------------------------------------------------------------------
constexpr bool test_zero() {
    auto zero3 = Vector3<float>::zero();

    CONSTEXPR_REQUIRE(zero3.x() == 0.0f);
    CONSTEXPR_REQUIRE(zero3.y() == 0.0f);
    CONSTEXPR_REQUIRE(zero3.z() == 0.0f);

    auto zero4 = Vector4<double>::zero();

    CONSTEXPR_REQUIRE(zero4.x() == 0.0);
    CONSTEXPR_REQUIRE(zero4.y() == 0.0);
    CONSTEXPR_REQUIRE(zero4.z() == 0.0);
    CONSTEXPR_REQUIRE(zero4.w() == 0.0);

    return true;
}
static_assert(test_zero());

// ---------------------------------------------------------------------
// comprehensive edge cases
// ---------------------------------------------------------------------
/* TODO
constexpr bool test_edge_cases() {
    Vector3<float> zero{0.0f, 0.0f, 0.0f};
    auto normalized = zero.normalize();
    CONSTEXPR_REQUIRE(normalized == zero);

#ifdef NDEBUG
    Vector3<float> a{1.0f, 0.0f, 0.0f};
    float angle = a.angle(zero).get();
    CONSTEXPR_REQUIRE(angle == 0.0f);
#endif

    return true;
}
    not constexpr yet
static_assert(test_edge_cases());
*/ 
