#include "constexpr_require.hpp"
#include "../ES_math.hpp"
#include "../Matrix.hpp"
#include "../VectorN.hpp"
#include "../PointN.hpp"

using namespace ES;

// ---------------------------------------------------------------------
// constructor (from column vectors) + operator() column-major layout
// ---------------------------------------------------------------------
constexpr bool test_constructor_and_layout() {
    Matrix<float, 2, 2> m(Vector2<float>{1.0f, 3.0f}, Vector2<float>{2.0f, 4.0f});

    CONSTEXPR_REQUIRE(ES::math::approx_equal(m(0, 0), 1.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(m(0, 1), 2.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(m(1, 0), 3.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(m(1, 1), 4.0f));

    m(0, 0) = 99.0f;
    CONSTEXPR_REQUIRE(ES::math::approx_equal(m(0, 0), 99.0f));

    Matrix<float, 3, 3> m3(Vector3<float>{1.0f, 0.0f, 0.0f},
                           Vector3<float>{0.0f, 1.0f, 0.0f},
                           Vector3<float>{0.0f, 0.0f, 1.0f});
    CONSTEXPR_REQUIRE(ES::math::approx_equal(m3(1, 1), 1.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(m3(0, 1), 0.0f));

    return true;
}
static_assert(test_constructor_and_layout());

// ---------------------------------------------------------------------
// swap_rows / swap_rows_in_place
// ---------------------------------------------------------------------
constexpr bool test_swap_rows() {
    Matrix<float, 2, 2> m(Vector2<float>{1.0f, 3.0f}, Vector2<float>{2.0f, 4.0f});

    auto swapped = m.swap_rows(0, 1);
    CONSTEXPR_REQUIRE(ES::math::approx_equal(swapped(0, 0), 3.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(swapped(1, 0), 1.0f));

    m.swap_rows_in_place(0, 1);
    CONSTEXPR_REQUIRE(ES::math::approx_equal(m(0, 0), 3.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(m(1, 0), 1.0f));

    return true;
}
static_assert(test_swap_rows());

// ---------------------------------------------------------------------
// scale_row / scale_row_in_place
// ---------------------------------------------------------------------
constexpr bool test_scale_row() {
    Matrix<float, 2, 2> m(Vector2<float>{1.0f, 3.0f}, Vector2<float>{2.0f, 4.0f});

    auto scaled = m.scale_row(0, 2.0f);
    CONSTEXPR_REQUIRE(ES::math::approx_equal(scaled(0, 0), 2.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(scaled(0, 1), 4.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(scaled(1, 0), 3.0f)); // untouched row

    m.scale_row_in_place(0, 2.0f);
    CONSTEXPR_REQUIRE(ES::math::approx_equal(m(0, 0), 2.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(m(0, 1), 4.0f));

    return true;
}
static_assert(test_scale_row());

// ---------------------------------------------------------------------
// add_scaled_row / add_scaled_row_in_place
// ---------------------------------------------------------------------
constexpr bool test_add_scaled_row() {
    Matrix<float, 2, 2> m(Vector2<float>{1.0f, 3.0f}, Vector2<float>{2.0f, 4.0f});
    // row0=(1,2), row1=(3,4). destination row1 += 2 * row0 -> (3+2, 4+4) = (5,8)

    auto result = m.add_scaled_row(0, 2.0f, 1);
    CONSTEXPR_REQUIRE(ES::math::approx_equal(result(1, 0), 5.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(result(1, 1), 8.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(result(0, 0), 1.0f)); // source untouched

    m.add_scaled_row_in_place(0, 2.0f, 1);
    CONSTEXPR_REQUIRE(ES::math::approx_equal(m(1, 0), 5.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(m(1, 1), 8.0f));

    return true;
}
static_assert(test_add_scaled_row());

// ---------------------------------------------------------------------
// determinant: 1x1, 2x2, 3x3, 4x4 direct-formula specializations
// ---------------------------------------------------------------------
constexpr bool test_determinant_small() {
    Matrix<float, 1, 1> m1(7.0f);
    CONSTEXPR_REQUIRE(ES::math::approx_equal(m1.determinant(), 7.0f));

    // [[1,2],[3,4]] -> det = 1*4 - 2*3 = -2
    Matrix<float, 2, 2> m2(Vector2<float>{1.0f, 3.0f}, Vector2<float>{2.0f, 4.0f});
    CONSTEXPR_REQUIRE(ES::math::approx_equal(m2.determinant(), -2.0f));

    // diag(2,4,8) -> det = 64
    Matrix<float, 3, 3> m3(Vector3<float>{2.0f, 0.0f, 0.0f},
                           Vector3<float>{0.0f, 4.0f, 0.0f},
                           Vector3<float>{0.0f, 0.0f, 8.0f});
    CONSTEXPR_REQUIRE(ES::math::approx_equal(m3.determinant(), 64.0f));

    // diag(2,4,8,16) -> det = 1024
    Matrix<float, 4, 4> m4(Vector4<float>{2.0f, 0.0f, 0.0f, 0.0f},
                           Vector4<float>{0.0f, 4.0f, 0.0f, 0.0f},
                           Vector4<float>{0.0f, 0.0f, 8.0f, 0.0f},
                           Vector4<float>{0.0f, 0.0f, 0.0f, 16.0f});
    CONSTEXPR_REQUIRE(ES::math::approx_equal(m4.determinant(), 1024.0f));

    return true;
}
static_assert(test_determinant_small());

// ---------------------------------------------------------------------
// determinant: general N>4 case (Gaussian elimination w/ partial pivoting)
// ---------------------------------------------------------------------
constexpr bool test_determinant_general() {
    Matrix<float, 5, 5> m(
        VectorN<float,5>{2.0f, 0.0f, 0.0f, 0.0f, 0.0f},
        VectorN<float,5>{0.0f, 4.0f, 0.0f, 0.0f, 0.0f},
        VectorN<float,5>{0.0f, 0.0f, 8.0f, 0.0f, 0.0f},
        VectorN<float,5>{0.0f, 0.0f, 0.0f, 16.0f, 0.0f},
        VectorN<float,5>{0.0f, 0.0f, 0.0f, 0.0f, 32.0f});

    CONSTEXPR_REQUIRE(ES::math::approx_equal(m.determinant(), 32768.0f));

    return true;
}
static_assert(test_determinant_general());

// ---------------------------------------------------------------------
// product_of_diagonals
// ---------------------------------------------------------------------
constexpr bool test_product_of_diagonals() {
    Matrix<float, 3, 3> m(Vector3<float>{2.0f, 0.0f, 0.0f},
                          Vector3<float>{0.0f, 4.0f, 0.0f},
                          Vector3<float>{0.0f, 0.0f, 8.0f});
    CONSTEXPR_REQUIRE(ES::math::approx_equal(m.product_of_diagonals(), 64.0f));
    return true;
}
static_assert(test_product_of_diagonals());

// ---------------------------------------------------------------------
// column
// ---------------------------------------------------------------------
constexpr bool test_column() {
    Matrix<float, 2, 2> m(Vector2<float>{1.0f, 3.0f}, Vector2<float>{2.0f, 4.0f});
    auto col0 = m.column(0);
    CONSTEXPR_REQUIRE(ES::math::approx_equal(col0[0], 1.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(col0[1], 3.0f));
    return true;
}
static_assert(test_column());

// ---------------------------------------------------------------------
// transpose / transpose_in_place
// ---------------------------------------------------------------------
constexpr bool test_transpose() {
    Matrix<float, 2, 2> m(Vector2<float>{1.0f, 3.0f}, Vector2<float>{2.0f, 4.0f});
    auto t = m.transpose();
    CONSTEXPR_REQUIRE(ES::math::approx_equal(t(0, 1), 3.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(t(1, 0), 2.0f));

    m.transpose_in_place();
    CONSTEXPR_REQUIRE(ES::math::approx_equal(m(0, 1), 3.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(m(1, 0), 2.0f));

    return true;
}
static_assert(test_transpose());

// ---------------------------------------------------------------------
// inverse / inverse_in_place: 2x2
// ---------------------------------------------------------------------
constexpr bool test_inverse_2x2() {
    Matrix<float, 2, 2> m(Vector2<float>{2.0f, 0.0f}, Vector2<float>{0.0f, 4.0f});
    auto inv = m.inverse();
    CONSTEXPR_REQUIRE(ES::math::approx_equal(inv(0, 0), 0.5f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(inv(1, 1), 0.25f));

    // round trip: m * inverse(m) == identity
    auto shouldBeIdentity = m * inv;
    CONSTEXPR_REQUIRE(ES::math::approx_equal(shouldBeIdentity(0, 0), 1.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(shouldBeIdentity(1, 1), 1.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(shouldBeIdentity(0, 1), 0.0f));

    return true;
}
static_assert(test_inverse_2x2());

// ---------------------------------------------------------------------
// inverse / inverse_in_place: 3x3 and 4x4 (cofactor/adjugate path)
// ---------------------------------------------------------------------
constexpr bool test_inverse_3x3_and_4x4() {
    Matrix<float, 3, 3> m3(Vector3<float>{2.0f, 0.0f, 0.0f},
                           Vector3<float>{0.0f, 4.0f, 0.0f},
                           Vector3<float>{0.0f, 0.0f, 8.0f});
    auto inv3 = m3.inverse();
    CONSTEXPR_REQUIRE(ES::math::approx_equal(inv3(0, 0), 0.5f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(inv3(1, 1), 0.25f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(inv3(2, 2), 0.125f));

    Matrix<float, 4, 4> m4(Vector4<float>{2.0f, 0.0f, 0.0f, 0.0f},
                           Vector4<float>{0.0f, 4.0f, 0.0f, 0.0f},
                           Vector4<float>{0.0f, 0.0f, 8.0f, 0.0f},
                           Vector4<float>{0.0f, 0.0f, 0.0f, 16.0f});
    auto inv4 = m4.inverse();
    CONSTEXPR_REQUIRE(ES::math::approx_equal(inv4(0, 0), 0.5f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(inv4(3, 3), 0.0625f));

    return true;
}
static_assert(test_inverse_3x3_and_4x4());

// ---------------------------------------------------------------------
// inverse_in_place: general N>4 case (Gauss-Jordan)
// ---------------------------------------------------------------------
constexpr bool test_inverse_general() {
    Matrix<float, 5, 5> m(
        VectorN<float,5>{2.0f, 0.0f, 0.0f, 0.0f, 0.0f},
        VectorN<float,5>{0.0f, 4.0f, 0.0f, 0.0f, 0.0f},
        VectorN<float,5>{0.0f, 0.0f, 8.0f, 0.0f, 0.0f},
        VectorN<float,5>{0.0f, 0.0f, 0.0f, 16.0f, 0.0f},
        VectorN<float,5>{0.0f, 0.0f, 0.0f, 0.0f, 32.0f});

    auto inv = m.inverse();
    CONSTEXPR_REQUIRE(ES::math::approx_equal(inv(0, 0), 0.5f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(inv(1, 1), 0.25f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(inv(2, 2), 0.125f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(inv(3, 3), 0.0625f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(inv(4, 4), 0.03125f));

    return true;
}
static_assert(test_inverse_general());

// ---------------------------------------------------------------------
// pseudo_inverse: square case (N>=M branch, degenerate to N==M)
// ---------------------------------------------------------------------
constexpr bool test_pseudo_inverse_square() {
    auto identity = Matrix<float, 2, 2>::identity();
    auto pinv = identity.pseudo_inverse();
    CONSTEXPR_REQUIRE(ES::math::approx_equal(pinv(0, 0), 1.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(pinv(1, 1), 1.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(pinv(0, 1), 0.0f));
    return true;
}
static_assert(test_pseudo_inverse_square());

// ---------------------------------------------------------------------
// pseudo_inverse: wide case (M>N branch)
// ---------------------------------------------------------------------
constexpr bool test_pseudo_inverse_wide() {
    Matrix<float, 2, 3> A(Vector2<float>{1.0f, 0.0f},
                          Vector2<float>{0.0f, 1.0f},
                          Vector2<float>{0.0f, 0.0f});

    auto Aplus = A.pseudo_inverse();
    auto At = A.transpose();

    CONSTEXPR_REQUIRE(ES::math::approx_equal(Aplus(0, 0), At(0, 0)));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(Aplus(1, 1), At(1, 1)));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(Aplus(2, 0), At(2, 0)));

    return true;
}
static_assert(test_pseudo_inverse_wide());

// ---------------------------------------------------------------------
// adjugate / cofactor
// ---------------------------------------------------------------------
constexpr bool test_adjugate_and_cofactor() {
    Matrix<float, 2, 2> m(Vector2<float>{2.0f, 0.0f}, Vector2<float>{0.0f, 4.0f});

    auto adj = m.adjugate();
    CONSTEXPR_REQUIRE(ES::math::approx_equal(adj(0, 0), 4.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(adj(1, 1), 2.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(adj(0, 1), 0.0f));

    auto cof = m.cofactor();
    CONSTEXPR_REQUIRE(ES::math::approx_equal(cof(0, 0), 4.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(cof(1, 1), 2.0f));

    return true;
}
static_assert(test_adjugate_and_cofactor());

// ---------------------------------------------------------------------
// operator*(Matrix)
// ---------------------------------------------------------------------
constexpr bool test_matrix_multiply() {
    Matrix<float, 2, 2> A(Vector2<float>{1.0f, 3.0f}, Vector2<float>{2.0f, 4.0f});
    Matrix<float, 2, 2> B(Vector2<float>{5.0f, 7.0f}, Vector2<float>{6.0f, 8.0f});

    auto C = A * B;
    CONSTEXPR_REQUIRE(ES::math::approx_equal(C(0, 0), 19.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(C(0, 1), 22.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(C(1, 0), 43.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(C(1, 1), 50.0f));

    return true;
}
static_assert(test_matrix_multiply());

// ---------------------------------------------------------------------
// operator*(VectorN) - generic template
// ---------------------------------------------------------------------
constexpr bool test_matrix_times_vector() {
    Matrix<float, 2, 2> m(Vector2<float>{2.0f, 0.0f}, Vector2<float>{0.0f, 4.0f});
    auto result = m * Vector2<float>{1.0f, 1.0f};
    CONSTEXPR_REQUIRE(ES::math::approx_equal(result[0], 2.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(result[1], 4.0f));
    return true;
}
static_assert(test_matrix_times_vector());

// ---------------------------------------------------------------------
// operator*(PointN) - generic template
// ---------------------------------------------------------------------
constexpr bool test_matrix_times_point() {
    Matrix<float, 2, 2> m(Vector2<float>{2.0f, 0.0f}, Vector2<float>{0.0f, 4.0f});
    auto result = m * PointN<float, 2>{1.0f, 1.0f};
    CONSTEXPR_REQUIRE(ES::math::approx_equal(result[0], 2.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(result[1], 4.0f));
    return true;
}
static_assert(test_matrix_times_point());

// ---------------------------------------------------------------------
// operator*(VectorN<T,3>) / operator*(PointN<T,3>) - the "sneaky" 4x4
// ---------------------------------------------------------------------
constexpr bool test_homogeneous_specializations() {
    Matrix<float, 4, 4> T(Vector4<float>{1.0f, 0.0f, 0.0f, 0.0f},
                          Vector4<float>{0.0f, 1.0f, 0.0f, 0.0f},
                          Vector4<float>{0.0f, 0.0f, 1.0f, 0.0f},
                          Vector4<float>{5.0f, 6.0f, 7.0f, 1.0f});

    // direction vectors must be unaffected by translation
    auto rotatedOrScaled = T * Vector3<float>{1.0f, 2.0f, 3.0f};
    CONSTEXPR_REQUIRE(ES::math::approx_equal(rotatedOrScaled[0], 1.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(rotatedOrScaled[1], 2.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(rotatedOrScaled[2], 3.0f));

    // points must pick up the translation
    auto movedPoint = T * PointN<float, 3>{1.0f, 2.0f, 3.0f};
    CONSTEXPR_REQUIRE(ES::math::approx_equal(movedPoint[0], 6.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(movedPoint[1], 8.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(movedPoint[2], 10.0f));

    return true;
}
static_assert(test_homogeneous_specializations());

// ---------------------------------------------------------------------
// rref / rref_in_place
// ---------------------------------------------------------------------
constexpr bool test_rref() {
    Matrix<float, 2, 3> m(Vector2<float>{1.0f, 1.0f},
                          Vector2<float>{1.0f, -1.0f},
                          Vector2<float>{3.0f, 1.0f});

    auto reduced = m.rref();
    CONSTEXPR_REQUIRE(ES::math::approx_equal(reduced(0, 0), 1.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(reduced(0, 1), 0.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(reduced(0, 2), 2.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(reduced(1, 0), 0.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(reduced(1, 1), 1.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(reduced(1, 2), 1.0f));

    m.rref_in_place();
    CONSTEXPR_REQUIRE(ES::math::approx_equal(m(0, 2), 2.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(m(1, 2), 1.0f));

    return true;
}
static_assert(test_rref());

// ---------------------------------------------------------------------
// reduce / reduce_in_place
// ---------------------------------------------------------------------
constexpr bool test_reduce() {
    Matrix<float, 2, 3> m(Vector2<float>{1.0f, 1.0f},
                          Vector2<float>{1.0f, -1.0f},
                          Vector2<float>{3.0f, 1.0f});

    auto reduced = m.reduce();

    CONSTEXPR_REQUIRE(ES::math::approx_equal(reduced(0, 0), 1.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(reduced(0, 1), 1.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(reduced(0, 2), 3.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(reduced(1, 0), -1.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(reduced(1, 1), 1.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(reduced(1, 2), -1.0f));

    m.reduce_in_place();
    CONSTEXPR_REQUIRE(ES::math::approx_equal(m(1, 1), 1.0f));

    return true;
}
static_assert(test_reduce());

// ---------------------------------------------------------------------
// trace
// ---------------------------------------------------------------------
constexpr bool test_trace() {
    Matrix<float, 2, 2> m(Vector2<float>{2.0f, 0.0f}, Vector2<float>{0.0f, 4.0f});
    CONSTEXPR_REQUIRE(ES::math::approx_equal(m.trace(), 6.0f));
    return true;
}
static_assert(test_trace());

// ---------------------------------------------------------------------
// minor
// ---------------------------------------------------------------------
constexpr bool test_minor() {
    Matrix<float, 2, 2> m(Vector2<float>{1.0f, 3.0f}, Vector2<float>{2.0f, 4.0f});

    auto minor00 = m.minor(0, 0);
    CONSTEXPR_REQUIRE(ES::math::approx_equal(minor00(0, 0), 4.0f));

    auto minor11 = m.minor(1, 1);
    CONSTEXPR_REQUIRE(ES::math::approx_equal(minor11(0, 0), 1.0f));

    return true;
}
static_assert(test_minor());

// ---------------------------------------------------------------------
// map / map_in_place
// ---------------------------------------------------------------------
constexpr bool test_map() {
    Matrix<float, 2, 2> m(Vector2<float>{2.0f, 0.0f}, Vector2<float>{0.0f, 4.0f});

    auto doubled = m.map([](float v) { return v * 2.0f; });
    CONSTEXPR_REQUIRE(ES::math::approx_equal(doubled(0, 0), 4.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(doubled(1, 1), 8.0f));

    m.map_in_place([](float v) { return v * 2.0f; });
    CONSTEXPR_REQUIRE(ES::math::approx_equal(m(0, 0), 4.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(m(1, 1), 8.0f));

    return true;
}
static_assert(test_map());

// ---------------------------------------------------------------------
// identity
// ---------------------------------------------------------------------
constexpr bool test_identity() {
    auto id = Matrix<float, 3, 3>::identity();
    CONSTEXPR_REQUIRE(ES::math::approx_equal(id(0, 0), 1.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(id(1, 1), 1.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(id(2, 2), 1.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(id(0, 1), 0.0f));
    return true;
}
static_assert(test_identity());

// ---------------------------------------------------------------------
// orthonormalize
// ---------------------------------------------------------------------
constexpr bool test_orthonormalize() {
    Matrix<float, 2, 2> m(Vector2<float>{2.0f, 0.0f}, Vector2<float>{0.0f, 4.0f});
    auto ortho = m.orthonormalize();

    CONSTEXPR_REQUIRE(ES::math::approx_equal(ortho(0, 0), 1.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(ortho(1, 1), 1.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(ortho(0, 1), 0.0f));

    return true;
}
/* TODO 
    make constexpr
    static_assert(test_orthonormalize());
*/

// ---------------------------------------------------------------------
// is_symmetric
// ---------------------------------------------------------------------
constexpr bool test_is_symmetric() {
    Matrix<float, 2, 2> symmetric(Vector2<float>{2.0f, 0.0f}, Vector2<float>{0.0f, 4.0f});
    CONSTEXPR_REQUIRE(symmetric.is_symmetric());

    Matrix<float, 2, 2> notSymmetric(Vector2<float>{1.0f, 3.0f}, Vector2<float>{2.0f, 4.0f});
    CONSTEXPR_REQUIRE(!notSymmetric.is_symmetric());

    return true;
}
static_assert(test_is_symmetric());

// ---------------------------------------------------------------------
// is_orthogonal
// ---------------------------------------------------------------------
constexpr bool test_is_orthogonal() {
    auto id = Matrix<float, 2, 2>::identity();
    CONSTEXPR_REQUIRE(id.is_orthogonal());

    Matrix<float, 2, 2> notOrthogonal(Vector2<float>{2.0f, 0.0f}, Vector2<float>{0.0f, 4.0f});
    CONSTEXPR_REQUIRE(!notOrthogonal.is_orthogonal()); // columns aren't unit length

    return true;
}
static_assert(test_is_orthogonal());

// ---------------------------------------------------------------------
// is_invertible
// ---------------------------------------------------------------------
constexpr bool test_is_invertible() {
    Matrix<float, 2, 2> invertible(Vector2<float>{2.0f, 0.0f}, Vector2<float>{0.0f, 4.0f});
    CONSTEXPR_REQUIRE(invertible.is_invertible());

    // col1 = 2 * col0 -> linearly dependent -> singular
    Matrix<float, 2, 2> singular(Vector2<float>{1.0f, 2.0f}, Vector2<float>{2.0f, 4.0f});
    CONSTEXPR_REQUIRE(!singular.is_invertible());

    return true;
}
static_assert(test_is_invertible());

// ---------------------------------------------------------------------
// normalize
// ---------------------------------------------------------------------
constexpr bool test_normalize() {
    Matrix<float, 2, 2> m(Vector2<float>{2.0f, 0.0f}, Vector2<float>{0.0f, 4.0f});
    auto normalized = m.normalize();
    CONSTEXPR_REQUIRE(ES::math::approx_equal(normalized(0, 0), 1.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(normalized(1, 1), 1.0f));
    return true;
}
/* TODO 
    make constexpr
    static_assert(test_normalize());
*/

// ---------------------------------------------------------------------
// set_row / set_row_in_place
// ---------------------------------------------------------------------
constexpr bool test_set_row() {
    auto id = Matrix<float, 2, 2>::identity();
    auto modified = id.set_row(0, Vector2<float>{9.0f, 8.0f});
    CONSTEXPR_REQUIRE(ES::math::approx_equal(modified(0, 0), 9.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(modified(0, 1), 8.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(modified(1, 1), 1.0f)); // untouched row

    id.set_row_in_place(0, Vector2<float>{9.0f, 8.0f});
    CONSTEXPR_REQUIRE(ES::math::approx_equal(id(0, 0), 9.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(id(0, 1), 8.0f));

    return true;
}
static_assert(test_set_row());

// ---------------------------------------------------------------------
// set_column / set_column_in_place
// ---------------------------------------------------------------------
constexpr bool test_set_column() {
    auto id = Matrix<float, 2, 2>::identity();
    auto modified = id.set_column(0, Vector2<float>{9.0f, 8.0f});
    CONSTEXPR_REQUIRE(ES::math::approx_equal(modified(0, 0), 9.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(modified(1, 0), 8.0f));

    id.set_column_in_place(0, Vector2<float>{9.0f, 8.0f});
    CONSTEXPR_REQUIRE(ES::math::approx_equal(id(0, 0), 9.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(id(1, 0), 8.0f));

    return true;
}
static_assert(test_set_column());

// ---------------------------------------------------------------------
// can_scalar_multiply / can_scalar_divide / can_component_add /
// can_component_subtract / can_lerp / can_negate / can_clamp
// ---------------------------------------------------------------------
constexpr bool test_capability_tags() {
    Matrix<float, 2, 2>::can_scalar_multiply();
    Matrix<float, 2, 2>::can_scalar_divide();
    Matrix<float, 2, 2>::can_component_add();
    Matrix<float, 2, 2>::can_component_subtract();
    Matrix<float, 2, 2>::can_lerp();
    Matrix<float, 2, 2>::can_negate();
    Matrix<float, 2, 2>::can_clamp();
    return true;
}
static_assert(test_capability_tags());

// ---------------------------------------------------------------------
// inherited arithmetic: component add/subtract, negate, scalar
// multiply/divide, lerp, clamp - brought in from ArithmeticOpsMixin
// ---------------------------------------------------------------------
constexpr bool test_component_addition() {
    Matrix<float, 2, 2> a(Vector2<float>{1.0f, 3.0f}, Vector2<float>{2.0f, 4.0f});
    Matrix<float, 2, 2> b(Vector2<float>{5.0f, 7.0f}, Vector2<float>{6.0f, 8.0f});

    auto result = a + b;
    CONSTEXPR_REQUIRE(ES::math::approx_equal(result(0, 0), 6.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(result(1, 1), 12.0f));

    a += b;
    CONSTEXPR_REQUIRE(a.almost_equal(result));

    return true;
}
static_assert(test_component_addition());

constexpr bool test_component_subtraction() {
    Matrix<float, 2, 2> a(Vector2<float>{5.0f, 7.0f}, Vector2<float>{6.0f, 8.0f});
    Matrix<float, 2, 2> b(Vector2<float>{1.0f, 3.0f}, Vector2<float>{2.0f, 4.0f});

    auto result = a - b;
    CONSTEXPR_REQUIRE(ES::math::approx_equal(result(0, 0), 4.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(result(1, 1), 4.0f));

    a -= b;
    CONSTEXPR_REQUIRE(a.almost_equal(result));

    return true;
}
static_assert(test_component_subtraction());

constexpr bool test_negate() {
    Matrix<float, 2, 2> m(Vector2<float>{1.0f, 3.0f}, Vector2<float>{2.0f, 4.0f});
    auto negated = -m;
    CONSTEXPR_REQUIRE(ES::math::approx_equal(negated(0, 0), -1.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(negated(1, 1), -4.0f));
    return true;
}
static_assert(test_negate());

constexpr bool test_scalar_multiplication() {
    Matrix<float, 2, 2> m(Vector2<float>{1.0f, 3.0f}, Vector2<float>{2.0f, 4.0f});
    auto result = m * 2.0f;
    CONSTEXPR_REQUIRE(ES::math::approx_equal(result(0, 0), 2.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(result(1, 1), 8.0f));

    m *= 2.0f;
    CONSTEXPR_REQUIRE(m.almost_equal(result));

    return true;
}
static_assert(test_scalar_multiplication());

constexpr bool test_scalar_division() {
    Matrix<float, 2, 2> m(Vector2<float>{2.0f, 4.0f}, Vector2<float>{6.0f, 8.0f});
    auto result = m / 2.0f;
    CONSTEXPR_REQUIRE(ES::math::approx_equal(result(0, 0), 1.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(result(1, 1), 4.0f));

    m /= 2.0f;
    CONSTEXPR_REQUIRE(m.almost_equal(result));

    return true;
}
static_assert(test_scalar_division());

constexpr bool test_lerp() {
    Matrix<float, 2, 2> a(Vector2<float>{0.0f, 0.0f}, Vector2<float>{0.0f, 0.0f});
    Matrix<float, 2, 2> b(Vector2<float>{2.0f, 4.0f}, Vector2<float>{6.0f, 8.0f});

    auto result = a.lerp(b, 0.5f);
    CONSTEXPR_REQUIRE(ES::math::approx_equal(result(0, 0), 1.0f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(result(1, 1), 4.0f));

    a.lerp_in_place(b, 0.5f);
    CONSTEXPR_REQUIRE(a.almost_equal(result));

    return true;
}
static_assert(test_lerp());

constexpr bool test_clamp() {
    Matrix<float, 2, 2> m(Vector2<float>{1.0f, 3.0f}, Vector2<float>{2.0f, 4.0f});
    auto clamped = m.clamp(1.5f, 3.5f);
    CONSTEXPR_REQUIRE(ES::math::approx_equal(clamped(0, 0), 1.5f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(clamped(1, 1), 3.5f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(clamped(0, 1), 2.0f));

    m.clamp_in_place(1.5f, 3.5f);
    CONSTEXPR_REQUIRE(ES::math::approx_equal(m(0, 0), 1.5f));
    CONSTEXPR_REQUIRE(ES::math::approx_equal(m(1, 1), 3.5f));

    return true;
}
static_assert(test_clamp());
