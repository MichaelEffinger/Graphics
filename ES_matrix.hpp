#ifndef COMPUTERGRAPHICS_ES_MATRIX_HPP
#define COMPUTERGRAPHICS_ES_MATRIX_HPP
#include <array>
#include <mdspan>

// #define ES_MATRIX_MDSPAN_NAME mdspan_
//
// #ifndef ES_MATRIX_MDSPAN_FUNCTION_ONLY
// #define GET
//
// #else
// #define ES_MATRIX_INTERNAL_MDSPAN(x)
// #endif

namespace ES {
    /**
     * The (row major) heavyweight champion of computer graphics.
     * @tparam T that which to represent the data inside!
     * @tparam N the value to be squared to make this square matrix.
     */
    template<typename T, std::size_t N>
    class matrix;
}

template<typename T, std::size_t N>
class ES::matrix {
public:
    static constexpr T N2 = N*N;
    static constexpr matrix identity_matrix() noexcept;

    using internal_v = std::array<T, N2>;
    using row_major_v = std::mdspan<float, std::extents<size_t, N, N>, std::layout_right>;
    using col_major_v = std::mdspan<float, std::extents<size_t, N, N>, std::layout_left>;

private:
    internal_v data_; ///< stored in row major
public:

    internal_v get() noexcept {
        return data_;
    }

    row_major_v get_row_major() {
        return data_;
    }

    col_major_v get_col_major() {
        return data_;
    }

};




#endif //COMPUTERGRAPHICS_ES_MATRIX_HPP