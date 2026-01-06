#pragma once
#include "Matrix.hpp"
#include "VectorN.hpp"
namespace ES{

    template <typename T, std::size_t N>
    class Transform{

        Matrix<T,N> mat;
        Matrix<T,N> inv;

        public:

        constexpr Transform() noexcept{
            mat = Matrix<T,N>::identity();
            inv = mat;
        }
        explicit constexpr Transform(const Matrix<T,N>& m){
            mat = m;
            inv = m.inverse();
        }
        constexpr Transform(const Matrix<T,N>& m, const Matrix<T,N>& i) noexcept{
            mat = m;
            inv = i;
        }

        [[nodiscard]] constexpr const Matrix<T,N>& matrix() const noexcept{
            return mat;
        }
        [[nodiscard]] constexpr const Matrix<T,N>& inverse() const noexcept{
            return inv;
        }

        [[nodiscard]] constexpr VectorN<T,N> apply(const VectorN<T,N>& vec) const noexcept{
            return mat * vec;
        }
        [[nodiscard]] constexpr VectorN<T,N> apply_inverse(const VectorN<T,N>& vec) const noexcept{
            return inv * vec;
        }


        [[nodiscard]] constexpr Transform operator*(const Transform& rhs) const noexcept{
            Transform temp;
            temp.mat = mat*rhs.mat;
            temp.inv = rhs.inv * inv;
            return temp;
        }

        constexpr Transform& operator*=(const Transform& rhs) noexcept {
            mat *= rhs.mat;
            inv = rhs.inv * inv;
            return *this;
        }


        [[nodiscard]] constexpr Transform invert() const noexcept{
            Transform temp;
            temp.mat = inv;
            temp.inv = mat;
            return temp; 
        }

        constexpr Transform& invert_in_place() noexcept{
            std::swap(mat,inv);
            return *this;
        }

        [[nodiscard]] static constexpr Transform identity() noexcept{
            Transform temp;
            temp.mat = Matrix<T,N>::identity();
            temp.inv = temp.mat;
            return temp;
        }

        [[nodiscard]] constexpr bool is_identity() const noexcept{
            return mat.almost_equal(Matrix<T,N>::identity());
        }

        [[nodiscard]] constexpr Matrix<T,N> normal_matrix() const noexcept{   
            return mat.normalize();
        }

        [[nodiscard]] static constexpr Transform translation(const VectorN<T,N-1> t) noexcept{
            Transform temp;
            std::memcpy(&temp.mat(0,N-1),t.data(),sizeof(T) * N-1);
            temp.inv = temp.mat.inverse();
            return temp;
        }
        [[nodiscard]] static constexpr Transform scale(const VectorN<T,N>& s) noexcept{
            Transform temp;
            for(std::size_t i =0; i<N; i++){
                temp.mat(i,i) = s[i];
            }
            temp.inv = temp.mat.inverse();
            return temp;
        }
        [[nodiscard]] static constexpr Transform uniform_scale(T s) noexcept{
            Transform temp;
            for(std::size_t i =0; i<N; i++){
                temp.mat(i,i) = s;
            }
            temp.inv = temp.mat.inverse();
            return temp;
        }
            
    };
}
