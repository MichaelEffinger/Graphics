#pragma once

#include "Matrix.hpp"
#include "VectorN.hpp"
#include "Quaternion.hpp"


namespace ES{
    template <typename T>
    class AffineTransform3{
        
        public:

        Matrix<T,3> linear;
        VectorN<T,3> translation;

        constexpr AffineTransform3() noexcept : linear(Matrix<T,3>::identity()), translation{0,0,0} { }

        constexpr AffineTransform3(const Matrix<T,3>& l, const VectorN<T,3>& t) noexcept{
            linear = l;
            translation = t;
        }

        [[nodiscard]] static constexpr AffineTransform3 from_translation(const VectorN<T,3> t) noexcept{
            AffineTransform3<T> temp_affine;
            temp_affine.linear = Matrix<T,3>::identity();
            temp_affine.translation = t;
            return temp_affine;
        }

    
        [[nodiscard]] static constexpr AffineTransform3 from_scale(const VectorN<T,3> scale) noexcept{
            AffineTransform3 temp_affine;
            temp_affine.translation = {0,0,0};
            std::fill(temp_affine.linear.begin(),temp_affine.linear.end(),0);
            temp_affine.linear(0,0) = scale[0];
            temp_affine.linear(1,1) = scale[1];
            temp_affine.linear(2,2) = scale[2];
            return temp_affine;
        }
 
  
        [[nodiscard]] constexpr VectorN<T,3> transform_point(const VectorN<T,3> point) const noexcept{
            return linear*point + translation;
        }
        [[nodiscard]] constexpr VectorN<T,3> transform_vector(const VectorN<T,3> vec) const noexcept{
            return linear * vec;
        }
        [[nodiscard]] constexpr VectorN<T,3> transform_point_inverse(const VectorN<T,3> point) const noexcept{
            return linear.inverse() * (point-translation);
        }
        [[nodiscard]] constexpr VectorN<T,3> transform_vector_inverse(const VectorN<T,3> vec) const noexcept{
            return linear.inverse() * vec;
        }

        [[nodiscard]] constexpr AffineTransform3 operator*(const AffineTransform3& rhs) const noexcept{
            AffineTransform3 temp_affine;
            temp_affine.linear= linear * rhs.linear;
            temp_affine.translation = linear * rhs.translation + translation;
            return temp_affine;
        }
        constexpr AffineTransform3& operator*=(const AffineTransform3& rhs) noexcept{
            translation = (linear *rhs.translation + translation);
            linear = linear *rhs.linear;
            return *this;
        }


        [[nodiscard]] constexpr AffineTransform3 inverse() const noexcept{
            AffineTransform3 inv;
            inv.linear = linear.inverse();
            inv.translation = -(inv.linear*translation);
            return inv;
        }

        [[nodiscard]] constexpr VectorN<T,3> get_translation() const noexcept{
            return translation;
        }

        [[nodiscard]] constexpr Matrix<T,3> get_linear() const noexcept{

        }

        [[nodiscard]] constexpr VectorN<T,3> get_scale() const noexcept{
            VectorN<T,3> scale;
            scale[0] = linear.column(0).magnitude(); 
            scale[1] = linear.column(1).magnitude(); 
            scale[2] = linear.column(2).magnitude(); 
            return scale;
        }
        [[nodiscard]] constexpr Matrix<T,4> to_matrix4() const noexcept{
            Matrix<T,4> temp;
            std::fill(temp.begin(),temp.end(),0);
            for(std::size_t i =0; i<3;i++){
                memcpy(&temp(0,i),&linear(0,i),sizeof(T)*3);
            }
            temp(0,3) = translation[0];
            temp(1,3) = translation[1];
            temp(2,3) = translation[2];
            temp(3,3) = T{1};

            return temp;
        }

        [[nodiscard]] static constexpr AffineTransform3 from_matrix4(const Matrix<T,4,4>& mat) noexcept{
            AffineTransform3 temp;
            for(std::size_t i=0; i<3;i++){
                memcpy(&temp.linear(0,i), &mat(0,i),sizeof(T)*3);
            }
            memcpy(&temp.translation[0],&mat(0,3),sizeof(T)*3);
            return temp;
        }

        static constexpr AffineTransform3 from_rotation(const Quaternion<T>& quat) noexcept{
            AffineTransform3 temp;

            float xx = quat.x() * quat.x();
            float yy = quat.y() * quat.y();
            float zz = quat.z() * quat.z();
            float xy = quat.x() * quat.y();
            float xz = quat.x() * quat.z();
            float yz = quat.y() * quat.z();
            float wx = quat.w() * quat.x();
            float wy = quat.w() * quat.y();
            float wz = quat.w() * quat.z();

            temp.linear(0,0) = 1 - 2 * (yy + zz);
            temp.linear(0,1) = 2 * (xy - wz);
            temp.linear(0,2) = 2 * (xz + wy);

            temp.linear(1,0) = 2 * (xy + wz);
            temp.linear(1,1) = 1 - 2 * (xx + zz);
            temp.linear(1,2) = 2 * (yz - wx);

            temp.linear(2,0) = 2 * (xz - wy);
            temp.linear(2,1) = 2 * (yz + wx);
            temp.linear(2,2) = 1 - 2 * (xx + yy);

            temp.translation = VectorN<T,3>::zero();

            return temp;
        }

        static constexpr AffineTransform3 from_trs(VectorN<T,3> t, Quaternion<T> r, VectorN<T,3> s)noexcept{
            AffineTransform3 temp = AffineTransform3::from_rotation(r);

            for (int col = 0; col < 3; ++col) {
                temp.linear(0,col) *= s[col];
                temp.linear(1,col) *= s[col];
                temp.linear(2,col) *= s[col];
            }

            temp.translation = t;

            return temp;
            
        }

        static constexpr AffineTransform3 orthographic(T l, T r, T b, T t, T n, T f){
            AffineTransform3 result;
            result.linear = Matrix<T,3>::identity();

            result.linear(0,0) = T(2) / (r - l);
            result.linear(1,1) = T(2) / (t - b);
            result.linear(2,2) = T(-2) / (f - n);

            result.translation[0] = -(r + l) / (r - l);
            result.translation[1] = -(t + b) / (t - b);
            result.translation[2] = -(f + n) / (f - n);

            return result;
        }

        static constexpr AffineTransform3 perspective(T fovY, T aspect, T nearZ, T farZ){
            float f = 1.0f / tanf(fovY * 0.5f);

            AffineTransform3 M;

            M(0,0) = f / aspect;  
            M(0,1) = 0;  M(0,2) = 0;
            M(0,3) = 0;
            M(1,0) = 0;
            
            M(1,1) = f;  
            M(1,2) = 0;
            M(1,3) = 0;

            M(2,0) = 0;           
            M(2,1) = 0;
            M(2,2) = (farZ + nearZ) / (nearZ - farZ);
            M(2,3) = (2 * farZ * nearZ) / (nearZ - farZ);

            M(3,0) = 0;           
            M(3,1) = 0;  
            M(3,2) = -1;                          
            M(3,3) = 0;

            return M;
        }

    };
}