#pragma once

#include "Matrix.hpp"
#include "VectorN.hpp"


namespace ES{
    template <typename T>
    class AffineTransform3{
        Matrix<T,3> linear;
        VectorN<T,3> translation;
        
    public:
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


    // these three rinky dink functions needs me to implement quaternions first
        // static constexpr AffineTransform3 from_rotation(const Quaternion<T>& quat) noexcept;
        // static constexpr AffineTransform3 from_trs(const VectorN<T,3>& t,const Quaternion<T>& r,const VectorN<T,3>& s) noexcept;
        // constexpr Quaternion<T> get_rotation() const noexcept;
    // the end
    };
}