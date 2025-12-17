#include <catch2/catch_test_macros.hpp>
#include "../ES_math.hpp"
#include "../PointN.hpp"
#include <iostream>

using namespace ES;

TEST_CASE("PointN accessors") {
    PointN<double, 3> pos{1.0, 2.0, 3.0};

    REQUIRE(math::approx_equal(pos.x(), 1.0));
    REQUIRE(math::approx_equal(pos.y(), 2.0));
    REQUIRE(math::approx_equal(pos.z(), 3.0));
}

TEST_CASE("PointN accessor w() for 4D points") {
    PointN<double, 4> pos{1.0, 2.0, 3.0, 4.0};

    REQUIRE(math::approx_equal(pos.x(), 1.0));
    REQUIRE(math::approx_equal(pos.y(), 2.0));
    REQUIRE(math::approx_equal(pos.z(), 3.0));
    REQUIRE(math::approx_equal(pos.w(), 4.0));
    
    pos.w() = 10.0;
    REQUIRE(math::approx_equal(pos.w(), 10.0));
}

TEST_CASE("PointN arithmetic with VectorN") {
    PointN<double, 3> pos{1.0, 2.0, 3.0};
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

TEST_CASE("PointN distance functions") {
    PointN<double, 2> a{0.0, 0.0};
    PointN<double, 2> b{3.0, 4.0};

    REQUIRE(math::approx_equal(a.distance(b), 5.0));
    REQUIRE(math::approx_equal(a.distance_squared(b), 25.0));
}

TEST_CASE("PointN interpolation") {
    PointN<double, 2> a{0.0, 0.0};
    PointN<double, 2> b{2.0, 2.0};

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

TEST_CASE("PointN barycentric coordinates") {
    PointN<double, 2> A{0.0, 0.0};
    PointN<double, 2> B{2.0, 0.0};
    PointN<double, 2> C{0.0, 2.0};

    auto P = barycentric(A, B, C, 0.25, 0.25, 0.5);
    REQUIRE(math::approx_equal(P.x(), 0.5)); 
    REQUIRE(math::approx_equal(P.y(), 1.0)); 
}

TEST_CASE("PointN to_vector conversion") {
    PointN<int, 3> pos{1, 2, 3};
    auto vec = pos.to_vector();
    REQUIRE(vec[0] == 1);
    REQUIRE(vec[1] == 2);
    REQUIRE(vec[2] == 3);
}

TEST_CASE("PointN scalar multiplication") {
    PointN<double, 3> pos{2.0, 4.0, 6.0};

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

TEST_CASE("PointN scalar division") {
    PointN<double, 3> pos{6.0, 12.0, 18.0};

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


TEST_CASE("PointN operator+ with VectorN on left side", "[PointN]") {
    VectorN<double, 3> vec{1.0, 2.0, 3.0};
    PointN<double, 3> pos{4.0, 5.0, 6.0};
    
    auto result = vec + pos;
    
    REQUIRE(math::approx_equal(result.x(), 5.0));
    REQUIRE(math::approx_equal(result.y(), 7.0));
    REQUIRE(math::approx_equal(result.z(), 9.0));
}

TEST_CASE("PointN operator- between two PointNs returns VectorN", "[PointN]") {
    PointN<double, 3> a{5.0, 7.0, 9.0};
    PointN<double, 3> b{2.0, 3.0, 4.0};
    
    auto vec = a - b;
    
    REQUIRE(math::approx_equal(vec[0], 3.0));
    REQUIRE(math::approx_equal(vec[1], 4.0));
    REQUIRE(math::approx_equal(vec[2], 5.0));
    
    auto magnitude = vec.magnitude();
    REQUIRE(magnitude > 0.0);
}

TEST_CASE("PointN lerp with different interpolation values", "[PointN]") {
    PointN<double, 3> start{0.0, 0.0, 0.0};
    PointN<double, 3> end{10.0, 20.0, 30.0};
    
    auto at_start = start.lerp(end, 0.0);
    REQUIRE(math::approx_equal(at_start.x(), 0.0));
    REQUIRE(math::approx_equal(at_start.y(), 0.0));
    REQUIRE(math::approx_equal(at_start.z(), 0.0));
    
    auto at_end = start.lerp(end, 1.0);
    REQUIRE(math::approx_equal(at_end.x(), 10.0));
    REQUIRE(math::approx_equal(at_end.y(), 20.0));
    REQUIRE(math::approx_equal(at_end.z(), 30.0));
    
    auto three_quarters = start.lerp(end, 0.75);
    REQUIRE(math::approx_equal(three_quarters.x(), 7.5));
    REQUIRE(math::approx_equal(three_quarters.y(), 15.0));
    REQUIRE(math::approx_equal(three_quarters.z(), 22.5));
}

TEST_CASE("PointN lerp_in_place modifies original point", "[PointN]") {
    PointN<float, 3> pos{0.0, 0.0, 0.0};
    PointN<float, 3> target{10.0, 20.0, 30.0};
    
    pos.lerp_in_place(target, 0.3);
    
    REQUIRE(math::approx_equal(pos.x(), 3.0));
    REQUIRE(math::approx_equal(pos.y(), 6.0));
    REQUIRE(math::approx_equal(pos.z(), 9.0));
    
    pos.lerp_in_place(target, 0.5);
    REQUIRE(math::approx_equal(pos.x(), 6.5));
    REQUIRE(math::approx_equal(pos.y(), 13.0));
    REQUIRE(math::approx_equal(pos.z(), 19.5));
}

TEST_CASE("PointN mid_point between two points", "[PointN]") {
    PointN<double, 3> a{-10.0, 0.0, 10.0};
    PointN<double, 3> b{10.0, 20.0, 30.0};
    
    auto mid = a.mid_point(b);
    
    REQUIRE(math::approx_equal(mid.x(), 0.0));
    REQUIRE(math::approx_equal(mid.y(), 10.0));
    REQUIRE(math::approx_equal(mid.z(), 20.0));
}

TEST_CASE("PointN barycentric with different weight combinations", "[PointN]") {
    PointN<double, 2> A{0.0, 0.0};
    PointN<double, 2> B{10.0, 0.0};
    PointN<double, 2> C{0.0, 10.0};
    
    auto at_A = barycentric(A, B, C, 1.0, 0.0, 0.0);
    REQUIRE(math::approx_equal(at_A.x(), 0.0));
    REQUIRE(math::approx_equal(at_A.y(), 0.0));
    
    auto at_B = barycentric(A, B, C, 0.0, 1.0, 0.0);
    REQUIRE(math::approx_equal(at_B.x(), 10.0));
    REQUIRE(math::approx_equal(at_B.y(), 0.0));
    
    auto at_C = barycentric(A, B, C, 0.0, 0.0, 1.0);
    REQUIRE(math::approx_equal(at_C.x(), 0.0));
    REQUIRE(math::approx_equal(at_C.y(), 10.0));
    
    auto centroid = barycentric(A, B, C, 1.0/3.0, 1.0/3.0, 1.0/3.0);
    REQUIRE(math::approx_equal(centroid.x(), 10.0/3.0));
    REQUIRE(math::approx_equal(centroid.y(), 10.0/3.0));
}

TEST_CASE("PointN to_vector creates proper vector representation", "[PointN]") {
    PointN<float, 4> pos{1.5f, 2.5f, 3.5f, 4.5f};
    
    auto vec = pos.to_vector();
    
    REQUIRE(math::approx_equal(vec[0], 1.5f));
    REQUIRE(math::approx_equal(vec[1], 2.5f));
    REQUIRE(math::approx_equal(vec[2], 3.5f));
    REQUIRE(math::approx_equal(vec[3], 4.5f));
    
    auto magnitude = vec.magnitude();
    REQUIRE(magnitude > 0.0f);
    
    auto normalized = vec.normalize();
    REQUIRE(math::approx_equal(normalized.magnitude(), 1.0f));
}

TEST_CASE("PointN distance is symmetric", "[PointN]") {
    PointN<double, 3> a{1.0, 2.0, 3.0};
    PointN<double, 3> b{4.0, 6.0, 8.0};
    
    double dist_ab = a.distance(b);
    double dist_ba = b.distance(a);
    
    REQUIRE(math::approx_equal(dist_ab, dist_ba));
}

TEST_CASE("PointN distance_squared avoids sqrt", "[PointN]") {
    PointN<double, 2> origin{0.0, 0.0};
    PointN<double, 2> point{3.0, 4.0};
    
    double dist_sq = origin.distance_squared(point);
    double dist = origin.distance(point);
    
    REQUIRE(math::approx_equal(dist_sq, 25.0));
    REQUIRE(math::approx_equal(dist, 5.0));
    REQUIRE(math::approx_equal(dist * dist, dist_sq));
}

TEST_CASE("PointN constructors with various types", "[PointN]") {
    PointN<int, 3> int_pos{1, 2, 3};
    REQUIRE(int_pos.x() == 1);
    REQUIRE(int_pos.y() == 2);
    REQUIRE(int_pos.z() == 3);
    
    PointN<float, 2> float_pos{1.5f, 2.5f};
    REQUIRE(math::approx_equal(float_pos.x(), 1.5f));
    REQUIRE(math::approx_equal(float_pos.y(), 2.5f));

    PointN<float, 3> mixed_pos{1, 2, 3};
    REQUIRE(math::approx_equal(mixed_pos.x(), 1.0f));
    REQUIRE(math::approx_equal(mixed_pos.y(), 2.0f));
    REQUIRE(math::approx_equal(mixed_pos.z(), 3.0f));
}

TEST_CASE("PointN arithmetic preserves point semantics", "[PointN]") {
    PointN<double, 3> origin{0.0, 0.0, 0.0};
    PointN<double, 3> point{3.0, 4.0, 5.0};
    VectorN<double, 3> displacement{1.0, 1.0, 1.0};
    
    auto new_point = point + displacement;
    REQUIRE(math::approx_equal(new_point.x(), 4.0));
    REQUIRE(math::approx_equal(new_point.y(), 5.0));
    REQUIRE(math::approx_equal(new_point.z(), 6.0));
    
    auto moved_back = new_point - displacement;
    REQUIRE(math::approx_equal(moved_back.x(), 3.0));
    REQUIRE(math::approx_equal(moved_back.y(), 4.0));
    REQUIRE(math::approx_equal(moved_back.z(), 5.0));
    
    auto difference = point - origin;
    REQUIRE(math::approx_equal(difference.magnitude(), 
            std::sqrt(3.0*3.0 + 4.0*4.0 + 5.0*5.0)));
}

TEST_CASE("PointN scalar operations maintain proper semantics", "[PointN]") {
    PointN<double, 3> pos{2.0, 4.0, 6.0};
    
    auto scaled = pos * 2.0;
    REQUIRE(math::approx_equal(scaled.x(), 4.0));
    REQUIRE(math::approx_equal(scaled.y(), 8.0));
    REQUIRE(math::approx_equal(scaled.z(), 12.0));
    
    auto divided = scaled / 4.0;
    REQUIRE(math::approx_equal(divided.x(), 1.0));
    REQUIRE(math::approx_equal(divided.y(), 2.0));
    REQUIRE(math::approx_equal(divided.z(), 3.0));
}

TEST_CASE("PointN edge cases", "[PointN]") {
    PointN<double, 3> zero{0.0, 0.0, 0.0};
    REQUIRE(math::approx_equal(zero.distance(zero), 0.0));
    
    auto self_mid = zero.mid_point(zero);
    REQUIRE(math::approx_equal(self_mid.x(), 0.0));
    REQUIRE(math::approx_equal(self_mid.y(), 0.0));
    REQUIRE(math::approx_equal(self_mid.z(), 0.0));
    
    PointN<double, 3> point{5.0, 5.0, 5.0};
    auto at_zero = zero.lerp(point, 0.0);
    auto at_one = zero.lerp(point, 1.0);
    
    REQUIRE(math::approx_equal(at_zero.x(), 0.0));
    REQUIRE(math::approx_equal(at_one.x(), 5.0));
}

TEST_CASE("PointN 2D specific operations", "[PointN]") {
    PointN<double, 2> a{0.0, 0.0};
    PointN<double, 2> b{1.0, 0.0};
    PointN<double, 2> c{0.0, 1.0};
    
    REQUIRE(math::approx_equal(a.distance(b), 1.0));
    REQUIRE(math::approx_equal(a.distance(c), 1.0));
    REQUIRE(math::approx_equal(b.distance(c), std::sqrt(2.0)));
}

TEST_CASE("PointN 4D operations", "[PointN]") {
    PointN<double, 4> origin{0.0, 0.0, 0.0, 0.0};
    PointN<double, 4> point{1.0, 1.0, 1.0, 1.0};
    
    double dist = origin.distance(point);
    REQUIRE(math::approx_equal(dist, 2.0));
    
    auto mid = origin.mid_point(point);
    REQUIRE(math::approx_equal(mid.x(), 0.5));
    REQUIRE(math::approx_equal(mid.y(), 0.5));
    REQUIRE(math::approx_equal(mid.z(), 0.5));
    REQUIRE(math::approx_equal(mid.w(), 0.5));
}

TEST_CASE("PointN operator*= returns proper reference", "[PointN]") {
    PointN<double, 3> pos{1.0, 2.0, 3.0};
    
    auto ref = (pos *= 2.0);
    
    REQUIRE(ref.almost_equal(pos));
    REQUIRE(math::approx_equal(pos.x(), 2.0));
    REQUIRE(math::approx_equal(pos.y(), 4.0));
    REQUIRE(math::approx_equal(pos.z(), 6.0));
}

TEST_CASE("PointN chained operations", "[PointN]") {
    PointN<double, 3> pos{1.0, 2.0, 3.0};
    VectorN<double, 3> vec{1.0, 1.0, 1.0};
    
    pos += vec;
    pos *= 2.0;
    pos -= vec;
    pos /= 2.0;
    
    REQUIRE(math::approx_equal(pos.x(), 1.5));
    REQUIRE(math::approx_equal(pos.y(), 2.5));
    REQUIRE(math::approx_equal(pos.z(), 3.5));
}