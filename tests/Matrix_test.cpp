#define NDEBUG
#include <catch2/catch_test_macros.hpp>
#include "../ES_math.hpp"
#include "../Matrix.hpp"
#include "../VectorN.hpp"

using namespace ES;

TEST_CASE("Matrix 2x2 constructor and accessors", "[Matrix]"){
    Matrix<float, 2, 2> m;
    m(0,0) = 1.0f; m(0,1) = 2.0f;
    m(1,0) = 3.0f; m(1,1) = 4.0f;
    
    REQUIRE(m(0,0) == 1.0f);
    REQUIRE(m(0,1) == 2.0f);
    REQUIRE(m(1,0) == 3.0f);
    REQUIRE(m(1,1) == 4.0f);
}

TEST_CASE("Matrix 3x3 constructor and accessors", "[Matrix]"){
    Matrix<float, 3, 3> m;
    m(0,0) = 1.0f; m(0,1) = 2.0f; m(0,2) = 3.0f;
    m(1,0) = 4.0f; m(1,1) = 5.0f; m(1,2) = 6.0f;
    m(2,0) = 7.0f; m(2,1) = 8.0f; m(2,2) = 9.0f;
    
    REQUIRE(m(0,0) == 1.0f);
    REQUIRE(m(1,1) == 5.0f);
    REQUIRE(m(2,2) == 9.0f);
}

TEST_CASE("Matrix 4x4 constructor and accessors", "[Matrix]"){
    Matrix<float, 4, 4> m;
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            m(i,j) = i * 4 + j;
        }
    }
    
    REQUIRE(m(0,0) == 0.0f);
    REQUIRE(m(1,1) == 5.0f);
    REQUIRE(m(2,2) == 10.0f);
    REQUIRE(m(3,3) == 15.0f);
}

TEST_CASE("Matrix column constructor", "[Matrix]"){
    Vector3<float> col1(1.0f, 2.0f, 3.0f);
    Vector3<float> col2(4.0f, 5.0f, 6.0f);
    Vector3<float> col3(7.0f, 8.0f, 9.0f);
    
    Matrix<float, 3, 3> m(col1, col2, col3);
    
    REQUIRE(m(0,0) == 1.0f);
    REQUIRE(m(1,0) == 2.0f);
    REQUIRE(m(2,0) == 3.0f);
    REQUIRE(m(0,1) == 4.0f);
    REQUIRE(m(0,2) == 7.0f);
}

TEST_CASE("Matrix arithmetic operator mixin check", "[Matrix]"){
    Matrix<float, 2, 2> m1;
    m1(0,0) = 1.0f; m1(0,1) = 2.0f;
    m1(1,0) = 3.0f; m1(1,1) = 4.0f;
    
    Matrix<float, 2, 2> m2;
    m2(0,0) = 5.0f; m2(0,1) = 6.0f;
    m2(1,0) = 7.0f; m2(1,1) = 8.0f;
    
    Matrix<float, 2, 2> zero;
    zero(0,0) = 0.0f; zero(0,1) = 0.0f;
    zero(1,0) = 0.0f; zero(1,1) = 0.0f;
    REQUIRE((m1 + zero) == m1);
    REQUIRE((m1 - zero) == m1);
    REQUIRE((m1 * 1.0f) == m1);
    REQUIRE((m1 / 1.0f) == m1);
}

TEST_CASE("Matrix swap_rows", "[Matrix]"){
    Matrix<float, 3, 3> m;
    m(0,0) = 1.0f; m(0,1) = 2.0f; m(0,2) = 3.0f;
    m(1,0) = 4.0f; m(1,1) = 5.0f; m(1,2) = 6.0f;
    m(2,0) = 7.0f; m(2,1) = 8.0f; m(2,2) = 9.0f;
    
    auto result = m.swap_rows(0, 2);
    
    REQUIRE(result(0,0) == 7.0f);
    REQUIRE(result(2,0) == 1.0f);
    REQUIRE(m(0,0) == 1.0f); // Original unchanged
}

TEST_CASE("Matrix swap_rows_in_place", "[Matrix]"){
    Matrix<float, 3, 3> m;
    m(0,0) = 1.0f; m(0,1) = 2.0f; m(0,2) = 3.0f;
    m(1,0) = 4.0f; m(1,1) = 5.0f; m(1,2) = 6.0f;
    m(2,0) = 7.0f; m(2,1) = 8.0f; m(2,2) = 9.0f;
    
    m.swap_rows_in_place(0, 2);
    
    REQUIRE(m(0,0) == 7.0f);
    REQUIRE(m(2,0) == 1.0f);
}

TEST_CASE("Matrix scale_row", "[Matrix]"){
    Matrix<float, 2, 2> m;
    m(0,0) = 1.0f; m(0,1) = 2.0f;
    m(1,0) = 3.0f; m(1,1) = 4.0f;
    
    auto result = m.scale_row(0, 2.0f);
    
    REQUIRE(result(0,0) == 2.0f);
    REQUIRE(result(0,1) == 4.0f);
    REQUIRE(result(1,0) == 3.0f);
    REQUIRE(m(0,0) == 1.0f); // Original unchanged
}

TEST_CASE("Matrix scale_row_in_place", "[Matrix]"){
    Matrix<float, 2, 2> m;
    m(0,0) = 1.0f; m(0,1) = 2.0f;
    m(1,0) = 3.0f; m(1,1) = 4.0f;
    
    m.scale_row_in_place(0, 2.0f);
    
    REQUIRE(m(0,0) == 2.0f);
    REQUIRE(m(0,1) == 4.0f);
}

TEST_CASE("Matrix add_scaled_row", "[Matrix]"){
    Matrix<float, 2, 2> m;
    m(0,0) = 1.0f; m(0,1) = 2.0f;
    m(1,0) = 3.0f; m(1,1) = 4.0f;
    
    auto result = m.add_scaled_row(0, 2.0f, 1);
    
    REQUIRE(result(1,0) == 5.0f); // 3 + 1*2
    REQUIRE(result(1,1) == 8.0f); // 4 + 2*2
    REQUIRE(m(1,0) == 3.0f); // Original unchanged
}

TEST_CASE("Matrix add_scaled_row_in_place", "[Matrix]"){
    Matrix<float, 2, 2> m;
    m(0,0) = 1.0f; m(0,1) = 2.0f;
    m(1,0) = 3.0f; m(1,1) = 4.0f;
    
    m.add_scaled_row_in_place(0, 2.0f, 1);
    
    REQUIRE(m(1,0) == 5.0f);
    REQUIRE(m(1,1) == 8.0f);
}

TEST_CASE("Matrix 2x2 determinant", "[Matrix]"){
    Matrix<float, 2, 2> m;
    m(0,0) = 1.0f; m(0,1) = 2.0f;
    m(1,0) = 3.0f; m(1,1) = 4.0f;
    
    float det = m.determinant();
    
    REQUIRE(det == -2.0f); // 1*4 - 2*3 = -2
}

TEST_CASE("Matrix 3x3 determinant", "[Matrix]"){
    Matrix<float, 3, 3> m;
    m(0,0) = 1.0f; m(0,1) = 2.0f; m(0,2) = 3.0f;
    m(1,0) = 0.0f; m(1,1) = 1.0f; m(1,2) = 4.0f;
    m(2,0) = 5.0f; m(2,1) = 6.0f; m(2,2) = 0.0f;
    
    float det = m.determinant();
    
    REQUIRE(det == 1.0f);
}

TEST_CASE("Matrix 4x4 determinant", "[Matrix]"){
    Matrix<float, 4, 4> m = Matrix<float, 4, 4>::identity();
    
    float det = m.determinant();
    
    REQUIRE(det == 1.0f);
}

TEST_CASE("Matrix product_of_diagonals", "[Matrix]"){
    Matrix<float, 3, 3> m;
    m(0,0) = 2.0f; m(0,1) = 0.0f; m(0,2) = 0.0f;
    m(1,0) = 0.0f; m(1,1) = 3.0f; m(1,2) = 0.0f;
    m(2,0) = 0.0f; m(2,1) = 0.0f; m(2,2) = 4.0f;
    
    float prod = m.product_of_diagonals();
    
    REQUIRE(prod == 24.0f);
}

TEST_CASE("Matrix column", "[Matrix]"){
    Matrix<float, 3, 3> m;
    m(0,0) = 1.0f; m(0,1) = 2.0f; m(0,2) = 3.0f;
    m(1,0) = 4.0f; m(1,1) = 5.0f; m(1,2) = 6.0f;
    m(2,0) = 7.0f; m(2,1) = 8.0f; m(2,2) = 9.0f;
    
    auto col = m.column(1);
    
    REQUIRE(col[0] == 2.0f);
    REQUIRE(col[1] == 5.0f);
    REQUIRE(col[2] == 8.0f);
}

TEST_CASE("Matrix transpose", "[Matrix]"){
    Matrix<float, 2, 3> m;
    m(0,0) = 1.0f; m(0,1) = 2.0f; m(0,2) = 3.0f;
    m(1,0) = 4.0f; m(1,1) = 5.0f; m(1,2) = 6.0f;
    
    auto result = m.transpose();
    
    REQUIRE(result(0,0) == 1.0f);
    REQUIRE(result(1,0) == 2.0f);
    REQUIRE(result(2,0) == 3.0f);
    REQUIRE(result(0,1) == 4.0f);
}

TEST_CASE("Matrix transpose_in_place", "[Matrix]"){
    Matrix<float, 3, 3> m;
    m(0,0) = 1.0f; m(0,1) = 2.0f; m(0,2) = 3.0f;
    m(1,0) = 4.0f; m(1,1) = 5.0f; m(1,2) = 6.0f;
    m(2,0) = 7.0f; m(2,1) = 8.0f; m(2,2) = 9.0f;
    
    m.transpose_in_place();
    
    REQUIRE(m(0,1) == 4.0f);
    REQUIRE(m(1,0) == 2.0f);
    REQUIRE(m(0,2) == 7.0f);
}

TEST_CASE("Matrix 2x2 inverse", "[Matrix]"){
    Matrix<float, 2, 2> m;
    m(0,0) = 4.0f; m(0,1) = 7.0f;
    m(1,0) = 2.0f; m(1,1) = 6.0f;
    
    auto inv = m.inverse();
    auto identity = m * inv;
    
    REQUIRE(math::approx_equal(identity(0,0), 1.0f));
    REQUIRE(math::approx_equal(identity(1,1), 1.0f));
    REQUIRE(math::approx_equal(identity(0,1), 0.0f));
    REQUIRE(math::approx_equal(identity(1,0), 0.0f));
}

TEST_CASE("Matrix 2x2 inverse_in_place", "[Matrix]"){
    Matrix<float, 2, 2> m;
    m(0,0) = 4.0f; m(0,1) = 7.0f;
    m(1,0) = 2.0f; m(1,1) = 6.0f;
    
    Matrix<float, 2, 2> original = m;
    m.inverse_in_place();
    
    auto identity = original * m;
    
    REQUIRE(math::approx_equal(identity(0,0), 1.0f));
    REQUIRE(math::approx_equal(identity(1,1), 1.0f));
}

TEST_CASE("Matrix 3x3 inverse", "[Matrix]"){
    Matrix<float, 3, 3> m;
    m(0,0) = 1.0f; m(0,1) = 2.0f; m(0,2) = 3.0f;
    m(1,0) = 0.0f; m(1,1) = 1.0f; m(1,2) = 4.0f;
    m(2,0) = 5.0f; m(2,1) = 6.0f; m(2,2) = 0.0f;
    
    auto inv = m.inverse();
    auto identity = m * inv;
    
    REQUIRE(math::approx_equal(identity(0,0), 1.0f));
    REQUIRE(math::approx_equal(identity(1,1), 1.0f));
    REQUIRE(math::approx_equal(identity(2,2), 1.0f));
}

TEST_CASE("Matrix 4x4 inverse", "[Matrix]"){
    Matrix<float, 4, 4> m = Matrix<float, 4, 4>::identity();
    m(0,1) = 2.0f;
    m(1,2) = 3.0f;
    m(2,3) = 4.0f;
    
    auto inv = m.inverse();
    auto identity = m * inv;
    
    REQUIRE(math::approx_equal(identity(0,0), 1.0f));
    REQUIRE(math::approx_equal(identity(1,1), 1.0f));
    REQUIRE(math::approx_equal(identity(2,2), 1.0f));
    REQUIRE(math::approx_equal(identity(3,3), 1.0f));
}

TEST_CASE("Matrix cofactor", "[Matrix]"){
    Matrix<float, 2, 2> m;
    m(0,0) = 1.0f; m(0,1) = 2.0f;
    m(1,0) = 3.0f; m(1,1) = 4.0f;
    
    auto cof = m.cofactor();
    
    REQUIRE(cof(0,0) == 4.0f);
    REQUIRE(cof(0,1) == -3.0f);
    REQUIRE(cof(1,0) == -2.0f);
    REQUIRE(cof(1,1) == 1.0f);
}

TEST_CASE("Matrix multiplication 2x2", "[Matrix]"){
    Matrix<float, 2, 2> m1;
    m1(0,0) = 1.0f; m1(0,1) = 2.0f;
    m1(1,0) = 3.0f; m1(1,1) = 4.0f;
    
    Matrix<float, 2, 2> m2;
    m2(0,0) = 5.0f; m2(0,1) = 6.0f;
    m2(1,0) = 7.0f; m2(1,1) = 8.0f;
    
    auto result = m1 * m2;
    
    REQUIRE(result(0,0) == 19.0f);
    REQUIRE(result(0,1) == 22.0f); 
    REQUIRE(result(1,0) == 43.0f);
    REQUIRE(result(1,1) == 50.0f);
}

TEST_CASE("Matrix multiplication 3x3", "[Matrix]"){
    Matrix<float, 3, 3> m1 = Matrix<float, 3, 3>::identity();
    Matrix<float, 3, 3> m2;
    m2(0,0) = 2.0f; m2(0,1) = 3.0f; m2(0,2) = 4.0f;
    m2(1,0) = 5.0f; m2(1,1) = 6.0f; m2(1,2) = 7.0f;
    m2(2,0) = 8.0f; m2(2,1) = 9.0f; m2(2,2) = 10.0f;
    
    auto result = m1 * m2;
    
    REQUIRE(result == m2);
}

TEST_CASE("Matrix multiplication non-square", "[Matrix]"){
    Matrix<float, 2, 3> m1;
    m1(0,0) = 1.0f; m1(0,1) = 2.0f; m1(0,2) = 3.0f;
    m1(1,0) = 4.0f; m1(1,1) = 5.0f; m1(1,2) = 6.0f;
    
    Matrix<float, 3, 2> m2;
    m2(0,0) = 7.0f; m2(0,1) = 8.0f;
    m2(1,0) = 9.0f; m2(1,1) = 10.0f;
    m2(2,0) = 11.0f; m2(2,1) = 12.0f;
    
    auto result = m1 * m2;
    
    REQUIRE(result(0,0) == 58.0f);
    REQUIRE(result(0,1) == 64.0f);
    REQUIRE(result(1,0) == 139.0f);
    REQUIRE(result(1,1) == 154.0f);
}

TEST_CASE("Matrix trace", "[Matrix]"){
    Matrix<float, 3, 3> m;
    m(0,0) = 1.0f; m(0,1) = 2.0f; m(0,2) = 3.0f;
    m(1,0) = 4.0f; m(1,1) = 5.0f; m(1,2) = 6.0f;
    m(2,0) = 7.0f; m(2,1) = 8.0f; m(2,2) = 9.0f;
    
    float tr = m.trace();
    
    REQUIRE(tr == 15.0f);
}

TEST_CASE("Matrix minor", "[Matrix]"){
    Matrix<float, 3, 3> m;
    m(0,0) = 1.0f; m(0,1) = 2.0f; m(0,2) = 3.0f;
    m(1,0) = 4.0f; m(1,1) = 5.0f; m(1,2) = 6.0f;
    m(2,0) = 7.0f; m(2,1) = 8.0f; m(2,2) = 9.0f;
    
    auto minor = m.minor(0, 0);
    
    REQUIRE(minor(0,0) == 5.0f);
    REQUIRE(minor(0,1) == 6.0f);
    REQUIRE(minor(1,0) == 8.0f);
    REQUIRE(minor(1,1) == 9.0f);
}

TEST_CASE("Matrix map", "[Matrix]"){
    Matrix<float, 2, 2> m;
    m(0,0) = 1.0f; m(0,1) = 2.0f;
    m(1,0) = 3.0f; m(1,1) = 4.0f;
    
    auto result = m.map([](float in){ return in * 2.0f; });
    
    REQUIRE(result(0,0) == 2.0f);
    REQUIRE(result(0,1) == 4.0f);
    REQUIRE(result(1,0) == 6.0f);
    REQUIRE(result(1,1) == 8.0f);
    REQUIRE(m(0,0) == 1.0f);
}

TEST_CASE("Matrix map_in_place", "[Matrix]"){
    Matrix<float, 2, 2> m;
    m(0,0) = 1.0f; m(0,1) = 2.0f;
    m(1,0) = 3.0f; m(1,1) = 4.0f;
    
    m.map_in_place([](float x){ return x * 2.0f; });
    
    REQUIRE(m(0,0) == 2.0f);
    REQUIRE(m(0,1) == 4.0f);
    REQUIRE(m(1,0) == 6.0f);
    REQUIRE(m(1,1) == 8.0f);
}
TEST_CASE("Matrix identity 2x2", "[Matrix]"){
    auto m = Matrix<float, 2, 2>::identity();
    
    REQUIRE(m(0,0) == 1.0f);
    REQUIRE(m(0,1) == 0.0f);
    REQUIRE(m(1,0) == 0.0f);
    REQUIRE(m(1,1) == 1.0f);
}

TEST_CASE("Matrix identity 3x3", "[Matrix]"){
    auto m = Matrix<float, 3, 3>::identity();
    
    REQUIRE(m(0,0) == 1.0f);
    REQUIRE(m(1,1) == 1.0f);
    REQUIRE(m(2,2) == 1.0f);
    REQUIRE(m(0,1) == 0.0f);
    REQUIRE(m(1,2) == 0.0f);
}

TEST_CASE("Matrix identity 4x4", "[Matrix]"){
    auto m = Matrix<float, 4, 4>::identity();
    
    REQUIRE(m(0,0) == 1.0f);
    REQUIRE(m(1,1) == 1.0f);
    REQUIRE(m(2,2) == 1.0f);
    REQUIRE(m(3,3) == 1.0f);
    REQUIRE(m(0,1) == 0.0f);
}

TEST_CASE("Matrix orthonormalize", "[Matrix]"){
    Matrix<float, 3, 3> m;
    m(0,0) = 1.0f; m(0,1) = 1.0f; m(0,2) = 0.0f;
    m(1,0) = 1.0f; m(1,1) = 0.0f; m(1,2) = 1.0f;
    m(2,0) = 0.0f; m(2,1) = 1.0f; m(2,2) = 1.0f;
    
    auto ortho = m.orthonormalize();
    
    // Check that columns are orthonormal
    auto c0 = ortho.column(0);
    auto c1 = ortho.column(1);
    auto c2 = ortho.column(2);
    
    REQUIRE(math::approx_equal(c0.dot(c0), 1.0f));
    REQUIRE(math::approx_equal(c1.dot(c1), 1.0f));
    REQUIRE(math::approx_equal(c2.dot(c2), 1.0f));
    REQUIRE(math::approx_equal(c0.dot(c1), 0.0f));
    REQUIRE(math::approx_equal(c0.dot(c2), 0.0f));
    REQUIRE(math::approx_equal(c1.dot(c2), 0.0f));
}

TEST_CASE("Matrix is_symmetric", "[Matrix]"){
    Matrix<float, 3, 3> symmetric;
    symmetric(0,0) = 1.0f; symmetric(0,1) = 2.0f; symmetric(0,2) = 3.0f;
    symmetric(1,0) = 2.0f; symmetric(1,1) = 4.0f; symmetric(1,2) = 5.0f;
    symmetric(2,0) = 3.0f; symmetric(2,1) = 5.0f; symmetric(2,2) = 6.0f;
    
    Matrix<float, 3, 3> not_symmetric;
    not_symmetric(0,0) = 1.0f; not_symmetric(0,1) = 2.0f; not_symmetric(0,2) = 3.0f;
    not_symmetric(1,0) = 4.0f; not_symmetric(1,1) = 5.0f; not_symmetric(1,2) = 6.0f;
    not_symmetric(2,0) = 7.0f; not_symmetric(2,1) = 8.0f; not_symmetric(2,2) = 9.0f;
    
    REQUIRE(symmetric.is_symmetric() == true);
    REQUIRE(not_symmetric.is_symmetric() == false);
}

TEST_CASE("Matrix is_orthogonal", "[Matrix]"){
    auto identity = Matrix<float, 3, 3>::identity();
    
    Matrix<float, 3, 3> not_orthogonal;
    not_orthogonal(0,0) = 1.0f; not_orthogonal(0,1) = 2.0f; not_orthogonal(0,2) = 3.0f;
    not_orthogonal(1,0) = 4.0f; not_orthogonal(1,1) = 5.0f; not_orthogonal(1,2) = 6.0f;
    not_orthogonal(2,0) = 7.0f; not_orthogonal(2,1) = 8.0f; not_orthogonal(2,2) = 9.0f;
    
    REQUIRE(identity.is_orthogonal() == true);
    REQUIRE(not_orthogonal.is_orthogonal() == false);
}

TEST_CASE("Matrix is_invertible", "[Matrix]"){
    Matrix<float, 2, 2> invertible;
    invertible(0,0) = 1.0f; invertible(0,1) = 2.0f;
    invertible(1,0) = 3.0f; invertible(1,1) = 4.0f;
    
    Matrix<float, 2, 2> singular;
    singular(0,0) = 1.0f; singular(0,1) = 2.0f;
    singular(1,0) = 2.0f; singular(1,1) = 4.0f;
    
    REQUIRE(invertible.is_invertible() == true);
    REQUIRE(singular.is_invertible() == false);
}

TEST_CASE("Matrix rref", "[Matrix]"){
    Matrix<float, 2, 3> m;
    m(0,0) = 2.0f; m(0,1) = 1.0f; m(0,2) = -1.0f;
    m(1,0) = -3.0f; m(1,1) = -1.0f; m(1,2) = 2.0f;
    
    auto result = m.rref();
    
    REQUIRE(math::approx_equal(result(0,0), 1.0f));
    REQUIRE(math::approx_equal(result(1,1), 1.0f));
}

TEST_CASE("Matrix lerp", "[Matrix]"){
    Matrix<float, 2, 2> m1;
    m1(0,0) = 0.0f; m1(0,1) = 0.0f;
    m1(1,0) = 0.0f; m1(1,1) = 0.0f;
    
    Matrix<float, 2, 2> m2;
    m2(0,0) = 10.0f; m2(0,1) = 10.0f;
    m2(1,0) = 10.0f; m2(1,1) = 10.0f;
    
    auto result = m1.lerp(m2, 0.5f);
    
    REQUIRE(result(0,0) == 5.0f);
    REQUIRE(result(0,1) == 5.0f);
    REQUIRE(result(1,0) == 5.0f);
    REQUIRE(result(1,1) == 5.0f);
}

TEST_CASE("Matrix clamp", "[Matrix]"){
    Matrix<float, 2, 2> m;
    m(0,0) = -5.0f; m(0,1) = 15.0f;
    m(1,0) = 3.0f; m(1,1) = 8.0f;
    
    auto result = m.clamp(0.0f, 10.0f);
    
    REQUIRE(result(0,0) == 0.0f);
    REQUIRE(result(0,1) == 10.0f);
    REQUIRE(result(1,0) == 3.0f);
    REQUIRE(result(1,1) == 8.0f);
}

TEST_CASE("Matrix negate", "[Matrix]"){
    Matrix<float, 2, 2> m;
    m(0,0) = 1.0f; m(0,1) = -2.0f;
    m(1,0) = 3.0f; m(1,1) = -4.0f;
    
    auto result = -m;
    
    REQUIRE(result(0,0) == -1.0f);
    REQUIRE(result(0,1) == 2.0f);
    REQUIRE(result(1,0) == -3.0f);
    REQUIRE(result(1,1) == 4.0f);
}


TEST_CASE("Matrix adjugate 2x2", "[Matrix]"){
    Matrix<float, 2, 2> m;
    m(0,0) = 1.0f; m(0,1) = 2.0f;
    m(1,0) = 3.0f; m(1,1) = 4.0f;
    
    auto adj = m.adjugate();
    
    REQUIRE(adj(0,0) == 4.0f);
    REQUIRE(adj(0,1) == -2.0f);
    REQUIRE(adj(1,0) == -3.0f);
    REQUIRE(adj(1,1) == 1.0f);
}

TEST_CASE("Matrix adjugate 3x3", "[Matrix]"){
    Matrix<float, 3, 3> m;
    m(0,0) = 1.0f; m(0,1) = 2.0f; m(0,2) = 3.0f;
    m(1,0) = 0.0f; m(1,1) = 1.0f; m(1,2) = 4.0f;
    m(2,0) = 5.0f; m(2,1) = 6.0f; m(2,2) = 0.0f;
    
    auto adj = m.adjugate();
    auto det = m.determinant();
    
    auto result = m * adj;
    
    REQUIRE(math::approx_equal(result(0,0), det));
    REQUIRE(math::approx_equal(result(1,1), det));
    REQUIRE(math::approx_equal(result(2,2), det));
    REQUIRE(math::approx_equal(result(0,1), 0.0f));
    REQUIRE(math::approx_equal(result(1,0), 0.0f));
}

TEST_CASE("Matrix pseudo_inverse tall matrix", "[Matrix]"){

    Matrix<float, 3, 2> m;
    m(0,0) = 1.0f; m(0,1) = 2.0f;
    m(1,0) = 3.0f; m(1,1) = 4.0f;
    m(2,0) = 5.0f; m(2,1) = 6.0f;
    
    auto pinv = m.pseudo_inverse();
    
    REQUIRE(pinv.column(0).size() == 2);
    
    auto should_be_identity = pinv * m;
    REQUIRE(math::approx_equal(should_be_identity(0,0), 1.0f));
    REQUIRE(math::approx_equal(should_be_identity(1,1), 1.0f));
    REQUIRE(math::approx_equal(should_be_identity(0,1), 0.0f));
    REQUIRE(math::approx_equal(should_be_identity(1,0), 0.0f));
}

TEST_CASE("Matrix pseudo_inverse wide matrix", "[Matrix]"){
    Matrix<float, 2, 3> m;
    m(0,0) = 1.0f; m(0,1) = 2.0f; m(0,2) = 3.0f;
    m(1,0) = 4.0f; m(1,1) = 5.0f; m(1,2) = 6.0f;
    
    auto pinv = m.pseudo_inverse();
    
    REQUIRE(pinv.column(0).size() == 3);
    
    auto should_be_identity = m * pinv;
    REQUIRE(math::approx_equal(should_be_identity(0,0), 1.0f));
    REQUIRE(math::approx_equal(should_be_identity(1,1), 1.0f));
    REQUIRE(math::approx_equal(should_be_identity(0,1), 0.0f));
    REQUIRE(math::approx_equal(should_be_identity(1,0), 0.0f));
}

TEST_CASE("Matrix reduce", "[Matrix]"){
    Matrix<float, 3, 3> m;
    m(0,0) = 2.0f; m(0,1) = 1.0f; m(0,2) = -1.0f;
    m(1,0) = -3.0f; m(1,1) = -1.0f; m(1,2) = 2.0f;
    m(2,0) = -2.0f; m(2,1) = 1.0f; m(2,2) = 2.0f;
    
    auto result = m.reduce();
    
    REQUIRE(math::approx_equal(result(0,0), 1.0f));
    REQUIRE(m(0,0) == 2.0f);
}

TEST_CASE("Matrix reduce_in_place", "[Matrix]"){
    Matrix<float, 3, 3> m;
    m(0,0) = 2.0f; m(0,1) = 1.0f; m(0,2) = -1.0f;
    m(1,0) = -3.0f; m(1,1) = -1.0f; m(1,2) = 2.0f;
    m(2,0) = -2.0f; m(2,1) = 1.0f; m(2,2) = 2.0f;
    
    m.reduce_in_place();
    
    REQUIRE(math::approx_equal(m(0,0), 1.0f));
}

TEST_CASE("Matrix rref_in_place", "[Matrix]"){
    Matrix<float, 2, 3> m;
    m(0,0) = 2.0f; m(0,1) = 1.0f; m(0,2) = -1.0f;
    m(1,0) = -3.0f; m(1,1) = -1.0f; m(1,2) = 2.0f;
    
    m.rref_in_place();
    
    REQUIRE(math::approx_equal(m(0,0), 1.0f));
    REQUIRE(math::approx_equal(m(1,1), 1.0f));
}

TEST_CASE("Matrix get methods", "[Matrix]"){
    Matrix<float, 2, 2> m;
    m(0,0) = 1.0f; m(0,1) = 2.0f;
    m(1,0) = 3.0f; m(1,1) = 4.0f;
    
    auto data = m.get();
    REQUIRE(data[0] == 1.0f);
    REQUIRE(data[1] == 3.0f);
    REQUIRE(data[2] == 2.0f);
    REQUIRE(data[3] == 4.0f);
    
    auto col_major = m.get_col_major();
    REQUIRE(col_major[0] == 1.0f);
    REQUIRE(col_major[1] == 3.0f);
    
    auto row_major = m.get_row_major();
    REQUIRE(row_major[0] == 1.0f);
    REQUIRE(row_major[1] == 2.0f);
    REQUIRE(row_major[2] == 3.0f);
    REQUIRE(row_major[3] == 4.0f);
}

TEST_CASE("Matrix 5x5 determinant using Gaussian elimination", "[Matrix]"){
    Matrix<float, 5, 5> m;
    for(std::size_t i = 0; i < 5; i++){
        for(std::size_t j = 0; j < 5; j++){
            m(i,j) = (i == j) ? 2.0f : 0.0f;
        }
    }
    
    float det = m.determinant();
    
    REQUIRE(math::approx_equal(det, 32.0f));
}

TEST_CASE("Matrix 5x5 inverse", "[Matrix]"){
    Matrix<float, 5, 5> m = Matrix<float, 5, 5>::identity();
    m(0,1) = 1.0f;
    m(1,2) = 1.0f;
    m(2,3) = 1.0f;
    m(3,4) = 1.0f;
    
    auto inv = m.inverse();
    auto identity = m * inv;
    
    for(std::size_t i = 0; i < 5; i++){
        REQUIRE(math::approx_equal(identity(i,i), 1.0f));
    }
}