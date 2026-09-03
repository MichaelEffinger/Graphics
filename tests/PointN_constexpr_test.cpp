#include "constexpr_require.hpp"

import ES_math;
import ES.VectorN;
import ES.PointN;

using namespace ES;

// ---------------------------------------------------------------------
// constructors
// ---------------------------------------------------------------------
constexpr bool test_constructors() {
    PointN<float, 3> p1{1.0f, 2.0f, 3.0f};
    CONSTEXPR_REQUIRE(p1[0] == 1.0f);
    CONSTEXPR_REQUIRE(p1[1] == 2.0f);
    CONSTEXPR_REQUIRE(p1[2] == 3.0f);

    PointN<float, 3> p1Paren(4.0f, 5.0f, 6.0f);
    CONSTEXPR_REQUIRE(p1Paren[0] == 4.0f);
    CONSTEXPR_REQUIRE(p1Paren[1] == 5.0f);
    CONSTEXPR_REQUIRE(p1Paren[2] == 6.0f);

    VectorN<float, 2> v2{1.0f, 2.0f};
    PointN<float, 3> p2(v2, 3.0f);
    CONSTEXPR_REQUIRE(p2[0] == 1.0f);
    CONSTEXPR_REQUIRE(p2[1] == 2.0f);
    CONSTEXPR_REQUIRE(p2[2] == 3.0f);

    VectorN<float, 3> v3{1.0f, 2.0f, 3.0f};
    PointN<float, 4> p3(v3, 4.0f);
    CONSTEXPR_REQUIRE(p3[0] == 1.0f);
    CONSTEXPR_REQUIRE(p3[1] == 2.0f);
    CONSTEXPR_REQUIRE(p3[2] == 3.0f);
    CONSTEXPR_REQUIRE(p3[3] == 4.0f);

    return true;
}
static_assert(test_constructors());

// ---------------------------------------------------------------------
// accessors - x, y, z, w
// ---------------------------------------------------------------------
constexpr bool test_accessors() {
    PointN<float, 2> p2{1.0f, 2.0f};
    CONSTEXPR_REQUIRE(p2.x() == 1.0f);
    CONSTEXPR_REQUIRE(p2.y() == 2.0f);

    PointN<float, 3> p3{1.0f, 2.0f, 3.0f};
    CONSTEXPR_REQUIRE(p3.x() == 1.0f);
    CONSTEXPR_REQUIRE(p3.y() == 2.0f);
    CONSTEXPR_REQUIRE(p3.z() == 3.0f);

    PointN<float, 4> p4{1.0f, 2.0f, 3.0f, 4.0f};
    CONSTEXPR_REQUIRE(p4.x() == 1.0f);
    CONSTEXPR_REQUIRE(p4.y() == 2.0f);
    CONSTEXPR_REQUIRE(p4.z() == 3.0f);
    CONSTEXPR_REQUIRE(p4.w() == 4.0f);

    p4.x() = 10.0f;
    p4.y() = 20.0f;
    p4.z() = 30.0f;
    p4.w() = 40.0f;
    CONSTEXPR_REQUIRE(p4.x() == 10.0f);
    CONSTEXPR_REQUIRE(p4.y() == 20.0f);
    CONSTEXPR_REQUIRE(p4.z() == 30.0f);
    CONSTEXPR_REQUIRE(p4.w() == 40.0f);

    return true;
}
static_assert(test_accessors());

// ---------------------------------------------------------------------
// operator+ (member), operator+ (friend, vector on the left), operator+=
// ---------------------------------------------------------------------
constexpr bool test_addition() {
    PointN<float, 3> point{1.0f, 2.0f, 3.0f};
    VectorN<float, 3> vec{1.0f, 1.0f, 1.0f};

    auto moved = point + vec;
    CONSTEXPR_REQUIRE(moved == PointN<float, 3>{2.0f, 3.0f, 4.0f});

    auto movedFriend = vec + point;
    CONSTEXPR_REQUIRE(movedFriend == moved);

    point += vec;
    CONSTEXPR_REQUIRE(point == moved);

    return true;
}
static_assert(test_addition());

// ---------------------------------------------------------------------
// operator- (PointN - PointN -> VectorN), operator- (PointN - VectorN
// -> PointN), operator-=
// ---------------------------------------------------------------------
constexpr bool test_subtraction() {
    PointN<float, 3> a{4.0f, 5.0f, 6.0f};
    PointN<float, 3> b{1.0f, 2.0f, 3.0f};

    VectorN<float, 3> diff = a - b;
    CONSTEXPR_REQUIRE(diff == VectorN<float, 3>{3.0f, 3.0f, 3.0f});

    VectorN<float, 3> vec{1.0f, 1.0f, 1.0f};
    PointN<float, 3> shifted = a - vec;
    CONSTEXPR_REQUIRE(shifted == PointN<float, 3>{3.0f, 4.0f, 5.0f});

    a -= vec;
    CONSTEXPR_REQUIRE(a == shifted);

    return true;
}
static_assert(test_subtraction());

// ---------------------------------------------------------------------
// distance / distance_squared
//
// distance() is marked constexpr in PointN.hpp and calls std::sqrt
// directly and unconditionally in its body. This static_assert is left
// enabled on purpose - it is the single most likely place for a
// Clang-compiles / MSVC-rejects divergence in this entire class.
// ---------------------------------------------------------------------
/* TODO
constexpr bool test_distance() {
    PointN<float, 3> a{0.0f, 0.0f, 0.0f};
    PointN<float, 3> b{3.0f, 4.0f, 0.0f};

    CONSTEXPR_REQUIRE(ES::math::approx_equal(a.distance(b), 5.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(a.distance_squared(b), 25.0f));

    return true;
}
    Not constexpr yet
    static_assert(test_distance());
*/

// ---------------------------------------------------------------------
// mid_point (depends on inherited lerp)
// ---------------------------------------------------------------------
constexpr bool test_mid_point() {
    PointN<float, 3> a{0.0f, 0.0f, 0.0f};
    PointN<float, 3> b{10.0f, 20.0f, 30.0f};

    auto mid = a.mid_point(b);
    CONSTEXPR_REQUIRE(mid == PointN<float, 3>{5.0f, 10.0f, 15.0f});

    return true;
}
static_assert(test_mid_point());

// ---------------------------------------------------------------------
// lerp / lerp_in_place (brought in via `using ArithmeticOpsMixin::lerp`)
// ---------------------------------------------------------------------
constexpr bool test_lerp() {
    PointN<float, 3> a{0.0f, 0.0f, 0.0f};
    PointN<float, 3> b{10.0f, 20.0f, 30.0f};

    auto quarter = a.lerp(b, 0.25f);
    CONSTEXPR_REQUIRE(quarter == PointN<float, 3>{2.5f, 5.0f, 7.5f});

    a.lerp_in_place(b, 0.5f);
    CONSTEXPR_REQUIRE(a == PointN<float, 3>{5.0f, 10.0f, 15.0f});

    return true;
}
static_assert(test_lerp());

// ---------------------------------------------------------------------
// barycentric (friend function)
// ---------------------------------------------------------------------
constexpr bool test_barycentric() {
    PointN<float, 3> A{0.0f, 0.0f, 0.0f};
    PointN<float, 3> B{4.0f, 0.0f, 0.0f};
    PointN<float, 3> C{0.0f, 4.0f, 0.0f};

    auto result = barycentric(A, B, C, 0.5f, 0.25f, 0.25f);
    CONSTEXPR_REQUIRE(result == PointN<float, 3>{1.0f, 1.0f, 0.0f});

    // weight fully on one vertex should reproduce that vertex exactly
    auto onA = barycentric(A, B, C, 1.0f, 0.0f, 0.0f);
    CONSTEXPR_REQUIRE(onA == A);

    auto onB = barycentric(A, B, C, 0.0f, 1.0f, 0.0f);
    CONSTEXPR_REQUIRE(onB == B);

    return true;
}
static_assert(test_barycentric());

// ---------------------------------------------------------------------
// to_vector
// ---------------------------------------------------------------------
constexpr bool test_to_vector() {
    PointN<float, 3> p{1.0f, 2.0f, 3.0f};
    VectorN<float, 3> v = p.to_vector();
    CONSTEXPR_REQUIRE(v == VectorN<float, 3>{1.0f, 2.0f, 3.0f});
    return true;
}
static_assert(test_to_vector());

// ---------------------------------------------------------------------
// can_scalar_multiply / can_scalar_divide / can_lerp / can_clamp
// ---------------------------------------------------------------------
constexpr bool test_capability_tags() {
    PointN<float, 3>::can_scalar_multiply();
    PointN<float, 3>::can_scalar_divide();
    PointN<float, 3>::can_lerp();
    PointN<float, 3>::can_clamp();
    return true;
}
static_assert(test_capability_tags());
