#define NDEBUG
#include <catch2/catch_test_macros.hpp>
#include "../ES_math.hpp"
#include "../Matrix.hpp"
#include "../VectorN.hpp"
#include "../Transform.hpp"

using namespace ES;

TEST_CASE("Transform default constructor", "[Transform]"){
    Transform<float, 3> transform;
    
    auto mat = transform.matrix();
    auto inv = transform.inverse();
    
    REQUIRE(mat(0,0) == 1.0f);
    REQUIRE(mat(1,1) == 1.0f);
    REQUIRE(mat(2,2) == 1.0f);
    REQUIRE(inv(0,0) == 1.0f);
    REQUIRE(inv(1,1) == 1.0f);
    REQUIRE(inv(2,2) == 1.0f);
}

TEST_CASE("Transform constructor from matrix", "[Transform]"){
    Matrix<float, 2, 2> m;
    m(0,0) = 2.0f; m(0,1) = 0.0f;
    m(1,0) = 0.0f; m(1,1) = 2.0f;
    
    Transform<float, 2> transform(m);
    
    auto mat = transform.matrix();
    auto inv = transform.inverse();
    
    REQUIRE(mat(0,0) == 2.0f);
    REQUIRE(mat(1,1) == 2.0f);
    REQUIRE(math::approx_equal(inv(0,0), 0.5f));
    REQUIRE(math::approx_equal(inv(1,1), 0.5f));
}

TEST_CASE("Transform constructor from matrix and inverse", "[Transform]"){
    Matrix<float, 2, 2> m;
    m(0,0) = 2.0f; m(0,1) = 0.0f;
    m(1,0) = 0.0f; m(1,1) = 2.0f;
    
    Matrix<float, 2, 2> i;
    i(0,0) = 0.5f; i(0,1) = 0.0f;
    i(1,0) = 0.0f; i(1,1) = 0.5f;
    
    Transform<float, 2> transform(m, i);
    
    auto mat = transform.matrix();
    auto inv = transform.inverse();
    
    REQUIRE(mat(0,0) == 2.0f);
    REQUIRE(inv(0,0) == 0.5f);
}

TEST_CASE("Transform matrix accessor", "[Transform]"){
    Matrix<float, 3, 3> m = Matrix<float, 3, 3>::identity();
    m(0,0) = 3.0f;
    
    Transform<float, 3> transform(m);
    
    auto mat = transform.matrix();
    
    REQUIRE(mat(0,0) == 3.0f);
    REQUIRE(mat(1,1) == 1.0f);
    REQUIRE(mat(2,2) == 1.0f);
}

TEST_CASE("Transform inverse accessor", "[Transform]"){
    Matrix<float, 2, 2> m;
    m(0,0) = 4.0f; m(0,1) = 0.0f;
    m(1,0) = 0.0f; m(1,1) = 4.0f;
    
    Transform<float, 2> transform(m);
    
    auto inv = transform.inverse();
    
    REQUIRE(math::approx_equal(inv(0,0), 0.25f));
    REQUIRE(math::approx_equal(inv(1,1), 0.25f));
}

TEST_CASE("Transform apply", "[Transform]"){
    Matrix<float, 2, 2> m;
    m(0,0) = 2.0f; m(0,1) = 0.0f;
    m(1,0) = 0.0f; m(1,1) = 3.0f;
    
    Transform<float, 2> transform(m);
    
    Vector2<float> vec(1.0f, 1.0f);
    auto result = transform.apply(vec);
    
    REQUIRE(result[0] == 2.0f);
    REQUIRE(result[1] == 3.0f);
}

TEST_CASE("Transform apply_inverse", "[Transform]"){
    Matrix<float, 2, 2> m;
    m(0,0) = 2.0f; m(0,1) = 0.0f;
    m(1,0) = 0.0f; m(1,1) = 4.0f;
    
    Transform<float, 2> transform(m);
    
    Vector2<float> vec(4.0f, 8.0f);
    auto result = transform.apply_inverse(vec);
    
    REQUIRE(math::approx_equal(result[0], 2.0f));
    REQUIRE(math::approx_equal(result[1], 2.0f));
}

TEST_CASE("Transform operator*", "[Transform]"){
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
    
    REQUIRE(math::approx_equal(mat(0,0), 6.0f));
    REQUIRE(math::approx_equal(mat(1,1), 6.0f));
}

TEST_CASE("Transform operator* inverse correctness", "[Transform]"){
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
    
    REQUIRE(math::approx_equal(inv(0,0), 1.0f/6.0f));
    REQUIRE(math::approx_equal(inv(1,1), 1.0f/6.0f));
}

TEST_CASE("Transform operator*=", "[Transform]"){
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
    
    REQUIRE(math::approx_equal(mat(0,0), 6.0f));
    REQUIRE(math::approx_equal(mat(1,1), 6.0f));
}

TEST_CASE("Transform invert", "[Transform]"){
    Matrix<float, 2, 2> m;
    m(0,0) = 2.0f; m(0,1) = 0.0f;
    m(1,0) = 0.0f; m(1,1) = 2.0f;
    
    Transform<float, 2> transform(m);
    auto inverted = transform.invert();
    
    auto mat = inverted.matrix();
    auto inv = inverted.inverse();
    
    REQUIRE(math::approx_equal(mat(0,0), 0.5f));
    REQUIRE(math::approx_equal(mat(1,1), 0.5f));
    REQUIRE(inv(0,0) == 2.0f);
    REQUIRE(inv(1,1) == 2.0f);
}

TEST_CASE("Transform invert_in_place", "[Transform]"){
    Matrix<float, 2, 2> m;
    m(0,0) = 2.0f; m(0,1) = 0.0f;
    m(1,0) = 0.0f; m(1,1) = 2.0f;
    
    Transform<float, 2> transform(m);
    transform.invert_in_place();
    
    auto mat = transform.matrix();
    auto inv = transform.inverse();
    
    REQUIRE(math::approx_equal(mat(0,0), 0.5f));
    REQUIRE(math::approx_equal(mat(1,1), 0.5f));
    REQUIRE(inv(0,0) == 2.0f);
    REQUIRE(inv(1,1) == 2.0f);
}

TEST_CASE("Transform identity static method", "[Transform]"){
    auto identity = Transform<float, 3>::identity();
    
    auto mat = identity.matrix();
    
    REQUIRE(mat(0,0) == 1.0f);
    REQUIRE(mat(1,1) == 1.0f);
    REQUIRE(mat(2,2) == 1.0f);
    REQUIRE(mat(0,1) == 0.0f);
    REQUIRE(mat(1,2) == 0.0f);
}

TEST_CASE("Transform is_identity true", "[Transform]"){
    Transform<float, 3> transform;
    
    REQUIRE(transform.is_identity() == true);
}

TEST_CASE("Transform is_identity false", "[Transform]"){
    Matrix<float, 3, 3> m = Matrix<float, 3, 3>::identity();
    m(0,0) = 2.0f;
    
    Transform<float, 3> transform(m);
    
    REQUIRE(transform.is_identity() == false);
}

TEST_CASE("Transform translation 2D", "[Transform]"){
    Vector2<float> t(3.0f, 4.0f);
    
    auto transform = Transform<float, 3>::translation(t);
    
    Vector3<float> point(1.0f, 1.0f, 1.0f);
    auto result = transform.apply(point);
    
    REQUIRE(math::approx_equal(result[0], 4.0f));
    REQUIRE(math::approx_equal(result[1], 5.0f));
    REQUIRE(math::approx_equal(result[2], 1.0f));
}

TEST_CASE("Transform translation 3D", "[Transform]"){
    Vector3<float> t(2.0f, 3.0f, 4.0f);
    
    auto transform = Transform<float, 4>::translation(t);
    
    Vector4<float> point(1.0f, 1.0f, 1.0f, 1.0f);
    auto result = transform.apply(point);
    
    REQUIRE(math::approx_equal(result[0], 3.0f));
    REQUIRE(math::approx_equal(result[1], 4.0f));
    REQUIRE(math::approx_equal(result[2], 5.0f));
    REQUIRE(math::approx_equal(result[3], 1.0f));
}

TEST_CASE("Transform scale", "[Transform]"){
    Vector3<float> s(2.0f, 3.0f, 4.0f);
    
    auto transform = Transform<float, 3>::scale(s);
    
    Vector3<float> vec(1.0f, 1.0f, 1.0f);
    auto result = transform.apply(vec);
    
    REQUIRE(result[0] == 2.0f);
    REQUIRE(result[1] == 3.0f);
    REQUIRE(result[2] == 4.0f);
}

TEST_CASE("Transform uniform_scale", "[Transform]"){
    auto transform = Transform<float, 3>::uniform_scale(5.0f);
    
    Vector3<float> vec(1.0f, 2.0f, 3.0f);
    auto result = transform.apply(vec);
    
    REQUIRE(result[0] == 5.0f);
    REQUIRE(result[1] == 10.0f);
    REQUIRE(result[2] == 15.0f);
}

TEST_CASE("Transform uniform_scale inverse", "[Transform]"){
    auto transform = Transform<float, 3>::uniform_scale(4.0f);
    
    Vector3<float> vec(8.0f, 12.0f, 16.0f);
    auto result = transform.apply_inverse(vec);
    
    REQUIRE(math::approx_equal(result[0], 2.0f));
    REQUIRE(math::approx_equal(result[1], 3.0f));
    REQUIRE(math::approx_equal(result[2], 4.0f));
}

TEST_CASE("Transform composition scale then translate", "[Transform]"){
    Vector2<float> t(1.0f, 1.0f);
    Vector3<float> s(2.0f, 2.0f, 1.0f);
    
    auto scale_transform = Transform<float, 3>::scale(s);
    auto translate_transform = Transform<float, 3>::translation(t);
    
    auto combined = translate_transform * scale_transform;
    
    Vector3<float> point(1.0f, 1.0f, 1.0f);
    auto result = combined.apply(point);
    
    REQUIRE(math::approx_equal(result[0], 3.0f));
    REQUIRE(math::approx_equal(result[1], 3.0f));
}

TEST_CASE("Transform apply and apply_inverse round trip", "[Transform]"){
    Vector3<float> s(2.0f, 3.0f, 4.0f);
    auto transform = Transform<float, 3>::scale(s);
    
    Vector3<float> original(5.0f, 6.0f, 7.0f);
    auto transformed = transform.apply(original);
    auto back = transform.apply_inverse(transformed);
    
    REQUIRE(math::approx_equal(back[0], original[0]));
    REQUIRE(math::approx_equal(back[1], original[1]));
    REQUIRE(math::approx_equal(back[2], original[2]));
}

TEST_CASE("Transform composition associativity", "[Transform]"){
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
    
    REQUIRE(math::approx_equal(r1[0], r2[0]));
    REQUIRE(math::approx_equal(r1[1], r2[1]));
}

TEST_CASE("Transform double invert", "[Transform]"){
    Matrix<float, 2, 2> m;
    m(0,0) = 2.0f; m(0,1) = 0.0f;
    m(1,0) = 0.0f; m(1,1) = 3.0f;
    
    Transform<float, 2> transform(m);
    auto double_inverted = transform.invert().invert();
    
    auto mat = double_inverted.matrix();
    
    REQUIRE(math::approx_equal(mat(0,0), 2.0f));
    REQUIRE(math::approx_equal(mat(1,1), 3.0f));
}