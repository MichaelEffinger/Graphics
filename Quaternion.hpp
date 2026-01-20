#pragma once

#include "ContainerN.hpp"
#include "VectorN.hpp"


namespace ES{

    template <typename T>
    class Quaternion: public ContainerN<Quaternion<T>,T,4>, public ArithmeticOpsMixin<Quaternion<T>,T,4>{
        
    private:
        using ContainerN<Quaternion,T,4>::data_;
        
    public:
        using ContainerN<Quaternion,T,4>::zip_in_place;
        using ContainerN<Quaternion,T,4>::zip;
        using ContainerN<Quaternion,T,4>::zip_reduce;
        using ContainerN<Quaternion,T,4>::begin;
        using ContainerN<Quaternion,T,4>::end;
        using ContainerN<Quaternion,T,4>::cend;
        using ContainerN<Quaternion,T,4>::data;
        using ContainerN<Quaternion,T,4>::cbegin;
        using ContainerN<Quaternion,T,4>::ContainerN;
        using ArithmeticOpsMixin<Quaternion,T,4>::lerp;
        using ArithmeticOpsMixin<Quaternion,T,4>::lerp_in_place;
        using ArithmeticOpsMixin<Quaternion,T,4>::operator*;
        using ArithmeticOpsMixin<Quaternion,T,4>::operator*=;


        static constexpr  void can_scalar_multiply(){return;}
        static constexpr void can_scalar_divide(){return;}
        static constexpr void can_component_add(){return;}
        static constexpr void can_component_subtract(){return;}
        static constexpr void can_negate(){return;}
        static constexpr void can_lerp(){return;}

        // i component;
        [[nodiscard]] constexpr auto&& w(this auto&& self) noexcept{
            return std::forward_like<decltype(self)>(self[0]);
        }

        // j-component;
        [[nodiscard]] constexpr auto&& x(this auto&& self) noexcept{
            return std::forward_like<decltype(self)>(self[1]);
        }
        
        // k-coponent;
        [[nodiscard]] constexpr auto&& y(this auto&& self) noexcept{
            return std::forward_like<decltype(self)>(self[2]);
        }
        //scalar powerhouse;
        [[nodiscard]] constexpr auto&& z(this auto&& self) noexcept{
            return std::forward_like<decltype(self)>(self[3]);
        }

        constexpr Quaternion() noexcept = default;

        constexpr Quaternion(T W, T X, T Y, T Z) noexcept{
            x() = X;
            y() = Y;
            z() = Z;
            w() = W;
        }

        /*c++ 26 constexpr*/ Quaternion(VectorN<T,3> axis, Angle<in_radians,T> angle) noexcept{
            VectorN<T,3> normalized = axis.normalize();
            T angle_half = angle.get() * T{0.5};
            T sine = std::sin(angle_half);
            w() = std::cos(angle_half);
            x() = normalized.x()*sine;
            y() = normalized.y()*sine;
            z() = normalized.z()*sine; 
        }
        

        constexpr Quaternion(VectorN<T,4> vec) noexcept{
            x() = vec.x();
            y() = vec.y();
            z() = vec.z();
            w() = vec.w();
        }
        
        [[nodiscard]] constexpr VectorN<T,4> vector() const noexcept{
            VectorN<T,4> temp(w(),x(),y(),z()) ;
            return temp;
        }


        //hamilton product 
        [[nodiscard]] constexpr Quaternion operator*(Quaternion rhs) const noexcept{
            Quaternion temp;
            temp.w() = (w()*rhs.w() - x()*rhs.x() - y()*rhs.y() - z()*rhs.z());
            temp.x() = (w()*rhs.x() + x()*rhs.w() + y()*rhs.z() - z()*rhs.y());
            temp.y() = (w()*rhs.y() - x()*rhs.z() + y()*rhs.w() - z()*rhs.x());
            temp.z() = (w()*rhs.z() + x()*rhs.y() - y()*rhs.x() + z()*rhs.w());
            return temp;
        }
        //in place hamilton product, tomorrow is my election day
        constexpr Quaternion& operator*=(Quaternion rhs) noexcept{   
            T W = (w()*rhs.w() - x()*rhs.x() - y()*rhs.y() - z()*rhs.z());
            T X = (w()*rhs.x() + x()*rhs.w() + y()*rhs.z() - z()*rhs.y());
            T Y = (w()*rhs.y() - x()*rhs.z() + y()*rhs.w() - z()*rhs.x());
            T Z = (w()*rhs.z() + x()*rhs.y() - y()*rhs.x() + z()*rhs.w());
            w() = W;
            x() = X;
            y() = Y;
            z() = Z;
            return *this;
        }

        [[nodiscard]] constexpr T length() const noexcept{
            return std::sqrt(length_squared());
        }
        [[nodiscard]] constexpr T length_squared() const noexcept{
            return (x()*x() + y()*y() + z()*z() + w()*w());
        }

        [[nodiscard]] constexpr Quaternion normalize() const noexcept{
            T len = length();
            assert(len != T{0} && "Zero length quaternion divide in normalize");
            if(len == T{0}) return Quaternion(T{0},T{0},T{0},T{0});
            return Quaternion(w()/len,x()/len,y()/len,z()/len);
        }

        constexpr Quaternion& normalize_in_place() noexcept{
            T len = length();
            assert(len != T{0} && "Zero length quaternion divide is normalize_in_place");
            if(len == T{0}){
                w() = x() = y() = z() = T{0};
                return *this;
            }
            w() /= len;
            x() /= len;
            y() /= len;
            z() /= len;
            return *this;
        }

        [[nodiscard]] constexpr Quaternion conjugate() const noexcept{
            return Quaternion(w(),-x(),-y(),-z());
        }

        constexpr Quaternion conjugate_in_place() noexcept{
            x() = -x();
            y() = -y();
            z() = -z();
            return *this;
        }

        
        [[nodiscard]] constexpr Quaternion inverse() const noexcept {
            Quaternion conjugate_ = conjugate();
            T conjugate_size = conjugate_.length_squared();
            assert(conjugate_size != T{0} && "Zero length quaternion division in inverse");
            if(conjugate_size == T{0}){
                return Quaternion(T{0},T{0},T{0},T{0});  
            } 
            return Quaternion(conjugate_.w() / conjugate_size, conjugate_.x() / conjugate_size, conjugate_.y() / conjugate_size, conjugate_.z() / conjugate_size);
        }



        constexpr Quaternion inverse_in_place() noexcept{
            Quaternion conjugate_ = conjugate();
            T conjugate_size = conjugate_.length_squared();
            assert(conjugate_size != T{0} && "Zero length quaternion division in inverse");
            if(conjugate_size == T{0}){
                w() = x() = y() = z() = T{0};
                return *this;
            }
            w() = conjugate_.w() / conjugate_size;
            x() = conjugate_.x() / conjugate_size;
            y() = conjugate_.y() / conjugate_size;
            z() = conjugate_.z() / conjugate_size;
            return *this;
        }


        [[nodiscard]] constexpr T dot(Quaternion rhs) const noexcept{
            return w()*rhs.w() + x()*rhs.x() + y()*rhs.y() + z()*rhs.z();
        }


        [[nodiscard]] constexpr VectorN<T,3> rotate(VectorN<T,3> vec) const noexcept{
            VectorN<T,3> q_vec(x(),y(),z());
            VectorN<T,3> crossed = q_vec.cross(vec)*T{2};
            return vec + crossed * w() + q_vec.cross(crossed);
        }
        

        [[nodiscard]] /* constexpr in c++26*/ Quaternion nlerp(Quaternion rhs, T t) const noexcept {
            if (dot(rhs) < T{0}){
                rhs = -rhs;
            }
            Quaternion q = lerp(rhs, t); 
            T qlen = q.length();
            return q.normalized();
        }

        [[nodiscard]] /* constexpr in c++26*/ Quaternion slerp(Quaternion rhs, T t) const {
            T dotv = dot(rhs);

            if (dotv < T(0)) {
                rhs = -rhs;
                dotv = -dotv;
            }

            dotv = std::clamp(dotv, T{-1}, T{1});

            if (dotv > T{0.9995}) {
                return nlerp(rhs, t);
            }

            T theta = std::acos(dotv);
            T sin_theta = std::sin(theta);

            return (std::sin((T(1) - t) * theta) / sin_theta) * (*this) + (std::sin(t * theta) / sin_theta) * rhs;
        }

       
        [[nodiscard]] static constexpr Quaternion identity() noexcept{
            Quaternion temp(T{1},T{0},T{0},T{0});
            return temp;
        }
        
    };

}