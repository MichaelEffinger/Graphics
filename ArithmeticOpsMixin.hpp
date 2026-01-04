

#pragma once
#include <cassert>
#include <algorithm>
#include <functional>
#include "config.hpp"


namespace ES{
    template<class Child, typename T, std::size_t N>
    class ArithmeticOpsMixin{

    protected:
        constexpr Child& derived() {return static_cast<Child&>(*this);}
        constexpr const Child& derived() const {return static_cast<const Child&>(*this);}   

    public:
        [[nodiscard]] constexpr Child operator+(Child rhs) const noexcept requires requires { Child::can_component_add(); }{
            return derived().zip(rhs,std::plus{});
        }

        constexpr Child& operator+=(Child rhs) noexcept requires requires { Child::can_component_add(); }{
            return derived().zip_in_place(rhs,std::plus{});
        }

        [[nodiscard]] constexpr Child operator+(T scalar) const noexcept requires requires {Child::can_scalar_add();}{
            Child temp_col;
            std::transform(derived().cbegin(),derived().cend(),temp_col.begin(),[scalar](T in) {return in+scalar;});
            return temp_col;
        }

        constexpr Child& operator+=(T scalar) noexcept requires requires {Child::can_scalar_add();}{
            std::transform(derived().cbegin(),derived().cend(),derived().begin(),[scalar](T in) {return in+scalar;});
            return derived();
        }

        [[nodiscard]] constexpr Child operator-(Child rhs) const noexcept requires requires { Child::can_component_subtract(); }{
            return derived().zip(rhs,std::minus{});
        }

        constexpr Child& operator-=(Child rhs) noexcept requires requires { Child::can_component_subtract(); }{
            return derived().zip_in_place(rhs,std::minus{});
        }

        [[nodiscard]] constexpr Child operator-(T scalar) const noexcept requires requires {Child::can_scalar_subtract();}{
            Child temp_col;
            std::transform(derived().cbegin(),derived().cend(),temp_col.begin(),[scalar](T in) {return in-scalar;});
            return temp_col;
        }

        constexpr Child& operator-=(T scalar) noexcept requires requires {Child::can_scalar_subtract();}{
            std::transform(derived().cbegin(),derived().cend(),derived().begin(),[scalar](T in) {return in-scalar;});
            return derived();
        }

        [[nodiscard]] constexpr Child operator*(Child rhs) const noexcept requires requires { Child::can_component_multiply(); }{
            return derived().zip(rhs,std::multiplies{});
        }

        
        constexpr Child& operator*=(Child rhs) noexcept requires requires { Child::can_component_multiply(); }{
            return derived().zip_in_place(rhs,std::multiplies{});
        }
        
        [[nodiscard]] constexpr Child operator*(T scalar) const noexcept requires requires {Child::can_scalar_multiply();}{
            Child temp_col;
            std::transform(derived().cbegin(),derived().cend(),temp_col.begin(),[scalar](T in) {return in*scalar;});
            return temp_col;
        }
        
        [[nodiscard]] friend constexpr Child operator*(T scalar, Child pos) requires requires {Child::can_scalar_multiply();}{
            Child tempPos;
            std::transform(pos.begin(),pos.end(),tempPos.begin(), [scalar](T in){return in * scalar;});
            return tempPos;
        }

        constexpr Child& operator*=(T scalar) noexcept requires requires {Child::can_scalar_multiply();}{
            std::transform(derived().cbegin(),derived().cend(),derived().begin(),[scalar](T in) {return in*scalar;});
            return derived();
        }

        [[nodiscard]] constexpr Child operator/(Child rhs)const noexcept requires requires { Child::can_component_divide(); }{
            return derived().zip(rhs, [](T a, T b) { assert(b !=0 && "Divide by zero in component division"); return (b != 0) ? (a / b) : T{0}; });
        }

        constexpr Child& operator/=(Child rhs)noexcept requires requires { Child::can_component_divide(); }{
            return derived().zip_in_place(rhs, [](T a, T b) { assert(b !=0 && "Divide by zero in component division"); return (b != 0) ? (a / b) : T{0}; });
        }

        [[nodiscard]] constexpr Child operator/(T scalar) const noexcept requires requires {Child::can_scalar_divide();}{
            Child tempCol;
            std::transform(derived().cbegin(), derived().cend(),tempCol.begin(),[scalar](T in) {assert(scalar !=0 && "Divide by zero in operator/"); return (scalar != 0) ? (in / scalar) : T{0}; });
            return tempCol;
        }

        constexpr Child& operator/=(T scalar)noexcept requires requires {Child::can_scalar_divide();}{
            std::transform(derived().begin(),derived().end(),derived().begin(),[scalar](T in) {assert(scalar !=0 && "Divide by zero in operator/"); return (scalar != 0) ? (in / scalar) : T{0}; });
            return derived();
        }

        [[nodiscard]] constexpr Child lerp(Child rhs, real t) const noexcept requires requires {Child::can_lerp();} {
            return derived().zip(rhs,[t](T a, T b) {return a+(b-a)*t;});
        }

        constexpr Child& lerp_in_place(Child rhs, real t) noexcept requires requires {Child::can_lerp();} {
            return derived().zip_in_place(rhs, [t](T a, T b) { return a + (b - a) * t;});
        }

        [[nodiscard]] constexpr Child clamp(T minVal, T maxVal) noexcept requires requires {Child::can_clamp();}{
            Child tempVec;
            std::transform(derived().begin(),derived().end(), tempVec.begin(),[minVal,maxVal](T in){return std::clamp(in, minVal, maxVal);});
            return tempVec;
        }

        constexpr Child& clamp_in_place(T minVal, T maxVal) noexcept requires requires {Child::can_clamp();}{
            std::transform(derived().begin(), derived().end(), derived().begin(),[minVal, maxVal](T in) { return std::clamp(in, minVal, maxVal);});
            return derived();
        }

        [[nodiscard]] constexpr Child operator-() const noexcept requires requires {Child::can_negate();} {
            Child result;
            std::transform(derived().begin(), derived().end(), result.begin(), std::negate<>());
            return result;
        }      

    };

}