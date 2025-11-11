#include <catch2/catch_test_macros.hpp>
#include "../ES_math.hpp"
#include "../PositionN.hpp"

using namespace ES;

TEST_CASE("PositionN accessors") {
    PositionN<double, 3> pos{1.0, 2.0, 3.0};

    REQUIRE(math::approx_equal(pos.x(), 1.0));
    REQUIRE(math::approx_equal(pos.y(), 2.0));
    REQUIRE(math::approx_equal(pos.z(), 3.0));

}

TEST_CASE("PositionN arithmetic with VectorN") {
    PositionN<double, 3> pos{1.0, 2.0, 3.0};
    VectorN<double, 3> vec{0.5, 1.0, 1.5};

    auto sum = pos + vec;
    REQUIRE(math::approx_equal(sum.x(), 1.5));
    REQUIRE(math::approx_equal(sum.y(), 3.0));
    REQUIRE(math::approx_equal(sum.z(), 4.5));

    pos += vec;
    REQUIRE(math::approx_equal(pos.x(), 1.5));
    REQUIRE(math::approx_equal(pos.y(), 3.0));
    REQUIRE(math::approx_equal(pos.z(), 4.5));

    auto diff = pos - vec;
    REQUIRE(math::approx_equal(diff[0], 1.0));
    REQUIRE(math::approx_equal(diff[1], 2.0));
    REQUIRE(math::approx_equal(diff[2], 3.0));

    pos -= vec;
    REQUIRE(math::approx_equal(pos.x(), 1.0));
    REQUIRE(math::approx_equal(pos.y(), 2.0));
    REQUIRE(math::approx_equal(pos.z(), 3.0));
}

TEST_CASE("PositionN distance functions") {
    PositionN<double, 2> a{0.0, 0.0};
    PositionN<double, 2> b{3.0, 4.0};

    REQUIRE(math::approx_equal(a.distance(b), 5.0));
    REQUIRE(math::approx_equal(a.distance_squared(b), 25.0));
}

TEST_CASE("PositionN interpolation") {
    PositionN<double, 2> a{0.0, 0.0};
    PositionN<double, 2> b{2.0, 2.0};

    auto mid = a.mid_point(b);
    REQUIRE(math::approx_equal(mid.x(), 1.0));
    REQUIRE(math::approx_equal(mid.y(), 1.0));

    auto lerped = a.lerp(b, 0.25);
    REQUIRE(math::approx_equal(lerped.x(), 0.5));
    REQUIRE(math::approx_equal(lerped.y(), 0.5));

    a.lerp_in_place(b, 0.5);
    REQUIRE(math::approx_equal(a.x(), 1.0));
    REQUIRE(math::approx_equal(a.y(), 1.0));
}

TEST_CASE("PositionN barycentric coordinates") {
    PositionN<double, 2> A{0.0, 0.0};
    PositionN<double, 2> B{2.0, 0.0};
    PositionN<double, 2> C{0.0, 2.0};

    auto P = barycentric(A, B, C, 0.25, 0.25, 0.5);
    REQUIRE(math::approx_equal(P.x(), 0.5)); 
    REQUIRE(math::approx_equal(P.y(), 1.0)); 
}

TEST_CASE("PositionN to_vector conversion") {
    PositionN<int, 3> pos{1, 2, 3};
    auto vec = pos.to_vector();
    REQUIRE(vec[0] == 1);
    REQUIRE(vec[1] == 2);
    REQUIRE(vec[2] == 3);
}

TEST_CASE("PositionN scalar multiplication") {
    PositionN<double, 3> pos{2.0, 4.0, 6.0};

    auto result1 = pos * 2.0;
    REQUIRE(math::approx_equal(result1[0], 4.0));
    REQUIRE(math::approx_equal(result1[1], 8.0));
    REQUIRE(math::approx_equal(result1[2], 12.0));

    auto result2 = 3.0 * pos;
    REQUIRE(math::approx_equal(result2[0], 6.0));
    REQUIRE(math::approx_equal(result2[1], 12.0));
    REQUIRE(math::approx_equal(result2[2], 18.0));

    pos *= 1.5;
    REQUIRE(math::approx_equal(pos[0], 3.0));
    REQUIRE(math::approx_equal(pos[1], 6.0));
    REQUIRE(math::approx_equal(pos[2], 9.0));
}

TEST_CASE("PositionN scalar division") {
    PositionN<double, 3> pos{6.0, 12.0, 18.0};

    auto result1 = pos / 3.0;
    REQUIRE(math::approx_equal(result1[0], 2.0));
    REQUIRE(math::approx_equal(result1[1], 4.0));
    REQUIRE(math::approx_equal(result1[2], 6.0));

    pos /= 6.0;
    REQUIRE(math::approx_equal(pos[0], 1.0));
    REQUIRE(math::approx_equal(pos[1], 2.0));
    REQUIRE(math::approx_equal(pos[2], 3.0));

#ifndef NDEBUG
    // In debug mode, division by zero asserts; skip test
#else
    auto result2 = pos / 0.0;
    REQUIRE(math::approx_equal(result2[0], 0.0));
    REQUIRE(math::approx_equal(result2[1], 0.0));
    REQUIRE(math::approx_equal(result2[2], 0.0));

    pos /= 0.0;
    REQUIRE(math::approx_equal(pos[0], 0.0));
    REQUIRE(math::approx_equal(pos[1], 0.0));
    REQUIRE(math::approx_equal(pos[2], 0.0));
#endif
}

