#include "constexpr_require.hpp"
#include "../ES_math.hpp"
#include "../PointN.hpp"

import ES.VectorN;
import ES.VectorH;


using namespace ES;

// ---------------------------------------------------------------------
// constructors - from PointN<T,3> and from VectorN<T,3>
// ---------------------------------------------------------------------
constexpr bool test_constructors() {
    PointN<float,3> p{1.0f, 2.0f, 3.0f};
    VectorH<float> fromPointDefault(p);
    CONSTEXPR_REQUIRE(fromPointDefault.x() == 1.0f);
    CONSTEXPR_REQUIRE(fromPointDefault.y() == 2.0f);
    CONSTEXPR_REQUIRE(fromPointDefault.z() == 3.0f);
    CONSTEXPR_REQUIRE(fromPointDefault.w() == 1.0f); // default w = 1 (point)

    VectorH<float> fromPointExplicit(p, 5.0f);
    CONSTEXPR_REQUIRE(fromPointExplicit.w() == 5.0f);

    VectorN<float,3> d{4.0f, 5.0f, 6.0f};
    VectorH<float> fromDirDefault(d);
    CONSTEXPR_REQUIRE(fromDirDefault.x() == 4.0f);
    CONSTEXPR_REQUIRE(fromDirDefault.y() == 5.0f);
    CONSTEXPR_REQUIRE(fromDirDefault.z() == 6.0f);
    CONSTEXPR_REQUIRE(fromDirDefault.w() == 0.0f); // default w = 0 (direction)

    VectorH<float> fromDirExplicit(d, 9.0f);
    CONSTEXPR_REQUIRE(fromDirExplicit.w() == 9.0f);

    return true;
}
static_assert(test_constructors());

// ---------------------------------------------------------------------
// accessors - x, y, z, w
// ---------------------------------------------------------------------
constexpr bool test_accessors() {
    VectorH<float> v{1.0f, 2.0f, 3.0f, 0.0f};

    CONSTEXPR_REQUIRE(v.x() == 1.0f);
    CONSTEXPR_REQUIRE(v.y() == 2.0f);
    CONSTEXPR_REQUIRE(v.z() == 3.0f);
    CONSTEXPR_REQUIRE(v.w() == 0.0f);

    v.x() = 10.0f;
    v.y() = 20.0f;
    v.z() = 30.0f;
    v.w() = 1.0f;

    CONSTEXPR_REQUIRE(v.x() == 10.0f);
    CONSTEXPR_REQUIRE(v.y() == 20.0f);
    CONSTEXPR_REQUIRE(v.z() == 30.0f);
    CONSTEXPR_REQUIRE(v.w() == 1.0f);

    return true;
}
static_assert(test_accessors());

// ---------------------------------------------------------------------
// operator+ / operator+= (directions only - w must not be 1 on both sides)
// ---------------------------------------------------------------------
constexpr bool test_addition() {
    VectorH<float> dirA{1.0f, 2.0f, 3.0f, 0.0f};
    VectorH<float> dirB{3.0f, 2.0f, 1.0f, 0.0f};

    auto sum = dirA + dirB;
    CONSTEXPR_REQUIRE(sum.x() == 4.0f);
    CONSTEXPR_REQUIRE(sum.y() == 4.0f);
    CONSTEXPR_REQUIRE(sum.z() == 4.0f);
    CONSTEXPR_REQUIRE(sum.w() == 0.0f);

    // point + direction is legal (translating a point)
    VectorH<float> point{0.0f, 0.0f, 0.0f, 1.0f};
    auto moved = point + dirA;
    CONSTEXPR_REQUIRE(moved.x() == 1.0f);
    CONSTEXPR_REQUIRE(moved.y() == 2.0f);
    CONSTEXPR_REQUIRE(moved.z() == 3.0f);

    dirA += dirB;
    CONSTEXPR_REQUIRE(dirA == sum);

    return true;
}
static_assert(test_addition());

// ---------------------------------------------------------------------
// operator- / operator-=
// ---------------------------------------------------------------------
constexpr bool test_subtraction() {
    VectorH<float> pointA{4.0f, 4.0f, 4.0f, 1.0f};
    VectorH<float> pointB{1.0f, 2.0f, 3.0f, 1.0f};

    // point - point => direction, legal per the assert (w() true on lhs)
    auto diff = pointA - pointB;
    CONSTEXPR_REQUIRE(diff.x() == 3.0f);
    CONSTEXPR_REQUIRE(diff.y() == 2.0f);
    CONSTEXPR_REQUIRE(diff.z() == 1.0f);

    VectorH<float> dirA{5.0f, 6.0f, 7.0f, 0.0f};
    VectorH<float> dirB{1.0f, 2.0f, 3.0f, 0.0f};
    dirA -= dirB;
    CONSTEXPR_REQUIRE(dirA == VectorH<float>{4.0f, 4.0f, 4.0f, 0.0f});

    return true;
}
static_assert(test_subtraction());

// ---------------------------------------------------------------------
// scalar operator* / operator*= (member and friend left-hand form)
// ---------------------------------------------------------------------
constexpr bool test_scalar_multiplication() {
    VectorH<float> v{2.0f, 3.0f, 4.0f, 0.0f};

    auto scaled = v * 3.0f;
    CONSTEXPR_REQUIRE(scaled == VectorH<float>{6.0f, 9.0f, 12.0f, 0.0f});

    auto leftScaled = 2.0f * v;
    CONSTEXPR_REQUIRE(leftScaled == VectorH<float>{4.0f, 6.0f, 8.0f, 0.0f});

    v *= 3.0f;
    CONSTEXPR_REQUIRE(v == scaled);

    return true;
}
static_assert(test_scalar_multiplication());

// ---------------------------------------------------------------------
// scalar operator/ / operator/=
// ---------------------------------------------------------------------
constexpr bool test_scalar_division() {
    VectorH<float> v{6.0f, 9.0f, 12.0f, 0.0f};

    auto divided = v / 3.0f;
    CONSTEXPR_REQUIRE(divided == VectorH<float>{2.0f, 3.0f, 4.0f, 0.0f});

    v /= 3.0f;
    CONSTEXPR_REQUIRE(v == divided);

#ifdef NDEBUG
    VectorH<float> zeroDivided = v / 0.0f;
    CONSTEXPR_REQUIRE(zeroDivided == VectorH<float>{0.0f, 0.0f, 0.0f, v.w()});
#endif

    return true;
}
static_assert(test_scalar_division());

// ---------------------------------------------------------------------
// hadamard product / divide (directions only)
// ---------------------------------------------------------------------
constexpr bool test_hadamard() {
    VectorH<float> a{1.0f, 2.0f, 3.0f, 0.0f};
    VectorH<float> b{2.0f, 3.0f, 4.0f, 0.0f};

    auto product = a.hadamard_product(b);
    CONSTEXPR_REQUIRE(product == VectorH<float>{2.0f, 6.0f, 12.0f, 0.0f});

    a.hadamard_product_in_place(b);
    CONSTEXPR_REQUIRE(a == product);

    VectorH<float> doubled{4.0f, 6.0f, 8.0f, 0.0f};
    VectorH<float> orig{2.0f, 3.0f, 4.0f, 0.0f};
    auto divided = doubled.hadamard_divide(orig);
    CONSTEXPR_REQUIRE(divided == VectorH<float>{2.0f, 2.0f, 2.0f, 0.0f});

    CONSTEXPR_REQUIRE(divided == doubled.hadamard_divide_in_place(orig));

    return true;
}
static_assert(test_hadamard());

// ---------------------------------------------------------------------
// dot / cross / cross_in_place (directions only)
// ---------------------------------------------------------------------
constexpr bool test_dot_and_cross() {
    VectorH<float> a{1.0f, 0.0f, 0.0f, 0.0f};
    VectorH<float> b{0.0f, 1.0f, 0.0f, 0.0f};

    CONSTEXPR_REQUIRE(a.dot(b) == 0.0f);

    auto c = a.cross(b);
    CONSTEXPR_REQUIRE(c == VectorH<float>{0.0f, 0.0f, 1.0f, 0.0f});

    VectorH<float> d{0.0f, 1.0f, 0.0f, 0.0f};
    d.cross_in_place(a);
    CONSTEXPR_REQUIRE(d == VectorH<float>{0.0f, 0.0f, -1.0f, 0.0f});

    return true;
}
static_assert(test_dot_and_cross());

// ---------------------------------------------------------------------
// magnitudeSquared
// ---------------------------------------------------------------------
constexpr bool test_magnitude_squared() {
    VectorH<float> v{3.0f, 4.0f, 0.0f, 0.0f};
    CONSTEXPR_REQUIRE(ES::math::approx_equal(v.magnitudeSquared(), 25.0f));
    return true;
}
static_assert(test_magnitude_squared());

/* TODO
constexpr bool test_magnitude_normalize() {
    VectorH<float> a{3.0f, 4.0f, 0.0f, 0.0f};

    CONSTEXPR_REQUIRE(ES::math::approx_equal(a.magnitude(), 5.0f));

    auto norm = a.normalize();
    CONSTEXPR_REQUIRE(ES::math::approx_equal(norm.magnitude(), 1.0f));

    a.normalize_in_place();
    CONSTEXPR_REQUIRE(ES::math::approx_equal(a.magnitude(), 1.0f));

    return true;
}
    not constexpr yet
    static_assert(test_magnitude_normalize());

*/
constexpr bool test_homogenize() {
    VectorH<float> point{2.0f, 4.0f, 6.0f, 2.0f};

    auto homogenized = point.homogenize();
    CONSTEXPR_REQUIRE(homogenized == VectorH<float>{1.0f, 2.0f, 3.0f, 1.0f});

    // w == 0 is a no-op (returns *this unchanged)
    VectorH<float> dir{1.0f, 2.0f, 3.0f, 0.0f};
    auto unchanged = dir.homogenize();
    CONSTEXPR_REQUIRE(unchanged == dir);

    point.homogenize_in_place();
    CONSTEXPR_REQUIRE(point == homogenized);

    return true;
}
static_assert(test_homogenize());

// ---------------------------------------------------------------------
// lerp / lerp_in_place
// ---------------------------------------------------------------------
constexpr bool test_lerp() {
    VectorH<float> a{0.0f, 0.0f, 0.0f, 0.0f};
    VectorH<float> b{10.0f, 20.0f, 30.0f, 0.0f};

    auto mid = a.lerp(b, 0.5f);
    CONSTEXPR_REQUIRE(mid == VectorH<float>{5.0f, 10.0f, 15.0f, 0.0f});

    auto start = a.lerp(b, 0.0f);
    CONSTEXPR_REQUIRE(start == a);

    auto end = a.lerp(b, 1.0f);
    CONSTEXPR_REQUIRE(end == b);

    a.lerp_in_place(b, 0.5f);
    CONSTEXPR_REQUIRE(a == mid);

    return true;
}
static_assert(test_lerp());

// ---------------------------------------------------------------------
// clamp / clamp_in_place
// ---------------------------------------------------------------------
constexpr bool test_clamp() {
    VectorH<float> v{1.0f, -1.0f, 5.0f, 0.0f};

    auto clamped = v.clamp(-0.5f, 2.0f);
    CONSTEXPR_REQUIRE(clamped == VectorH<float>{1.0f, -0.5f, 2.0f, 0.0f});

    v.clamp_in_place(-1.0f, 1.0f);
    CONSTEXPR_REQUIRE(v == VectorH<float>{1.0f, -1.0f, 1.0f, 0.0f});

    return true;
}
static_assert(test_clamp());

// ---------------------------------------------------------------------
// refract / refract_in_place / refract_safe / refract_in_place_safe
// ---------------------------------------------------------------------
/* TODO
constexpr bool test_refract() {
    VectorH<float> incoming{0.0f, -1.0f, 0.0f, 0.0f};
    VectorH<float> normal{0.0f, 1.0f, 0.0f, 0.0f};

    auto refracted = incoming.refract(normal, 1.0f, 1.5f);
    CONSTEXPR_REQUIRE(refracted.magnitudeSquared() >= 0.0f);

    incoming.refract_in_place(normal, 1.0f, 1.5f);
    CONSTEXPR_REQUIRE(incoming.magnitudeSquared() >= 0.0f);

    VectorH<float> incoming2{0.0f, -5.0f, 0.0f, 0.0f};
    VectorH<float> normal2{0.0f, 3.0f, 0.0f, 0.0f};
    auto safeRefracted = incoming2.refract_safe(normal2, 1.0f, 1.5f);
    CONSTEXPR_REQUIRE(safeRefracted.magnitudeSquared() >= 0.0f);

    incoming2.refract_in_place_safe(normal2, 1.0f, 1.5f);
    CONSTEXPR_REQUIRE(incoming2.magnitudeSquared() >= 0.0f);

    return true;
}
    Not constexpr yet
    static_assert(test_refract());
*/