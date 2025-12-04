#pragma once
#include <type_traits>
#include <cassert>
#include <cmath>
#include <type_traits>
#include <algorithm>
#include <functional>
#include "ContainerN.hpp"

namespace ES{
    template<class Child, typename T, std::size_t N>
    class ColorOpsMixin{

    protected:
        constexpr Child& derived() {return static_cast<Child&>(*this);}
        constexpr const Child& derived() const {return static_cast<const Child&>(*this);}
        
    public:

        [[nodiscard]] constexpr Child operator+(Child rhs) const noexcept{
            return derived().zip(rhs,std::plus{});
        }

        constexpr Child& operator+=(Child rhs) noexcept{
            return derived().zip_in_place(rhs,std::plus{});
        }

        [[nodiscard]] constexpr Child operator-(Child rhs)const noexcept{
            return derived().zip(rhs,std::minus{});
        }

        constexpr Child & operator-=(Child rhs) noexcept{
            return derived().zip_in_place(rhs,std::minus{});
        }
        
        [[nodiscard]] constexpr Child operator*(Child rhs) const noexcept{
            return derived().zip(rhs,std::multiplies{});
        }

        constexpr Child& operator*=(Child rhs) noexcept{
            return derived().zip_in_place(rhs,std::multiplies{});
        }

        [[nodiscard]] constexpr Child operator/(Child rhs)const noexcept{
            return derived().zip(rhs, [](T a, T b) { assert(b !=0 && "Divide by zero in component division"); return (b != 0) ? (a / b) : T{0}; });
        }

        constexpr Child& operator/=(Child rhs)noexcept{
            return derived().zip_in_place(rhs, [](T a, T b) { assert(b !=0 && "Divide by zero in component division"); return (b != 0) ? (a / b) : T{0}; });
        }

        [[nodiscard]] constexpr Child operator*(T scalar) const noexcept{
            Child temp_col;
            std::transform(derived().cbegin(),derived().cend(),temp_col.begin(),[scalar](T in) {return in*scalar;});
            return temp_col;
        }
        
        [[nodiscard]] constexpr friend Child operator*(T scalar, Child lhs) noexcept{
            Child temp_col;
            std::transform(lhs.cbegin(),lhs.cend(),temp_col.begin(),[scalar](T in) {return in*scalar;} );
            return temp_col;
        }

        constexpr Child& operator*=(T scalar)noexcept{
            std::transform(derived().begin(),derived().end(),derived().begin(),[scalar](T in){return in * scalar;});
            return derived();
        }

        [[nodiscard]] constexpr Child operator/(T scalar) const noexcept{
            Child tempCol;
            std::transform(derived().cbegin(), derived().cend(),tempCol.begin(),[scalar](T in) {assert(scalar !=0 && "Divide by zero in operator/"); return (scalar != 0) ? (in / scalar) : T{0}; });
            return tempCol;
        }

        constexpr Child& operator/=(T scalar)noexcept{
            std::transform(derived().cbegin(),derived().cend(),derived().begin(),[scalar](T in) {assert(scalar !=0 && "Divide by zero in operator/"); return (scalar != 0) ? (in / scalar) : T{0}; });
            return derived();
        }

        [[nodiscard]] constexpr Child lerp(Child rhs, T t) const noexcept{
            return derived().zip(rhs,[t](T a, T b) {return a+(b-a)*t;});
        }

        constexpr Child& lerp_in_place(Child rhs, T t) noexcept{
            return derived().zip_in_place(rhs,[t](T a,T b) {return a+(b-a)*t;});
        }

        [[nodiscard]] constexpr Child clamp(T lower, T upper) const noexcept{
            Child temp_col;
            std::transform(derived().cbegin(),derived().cend(), temp_col.begin(),[lower,upper](T in){return std::clamp(in, lower, upper);});
            return temp_col;
        }

        constexpr Child& clamp_in_place(T lower, T upper)noexcept{
            std::transform(derived().begin(),derived().end(),derived().begin(),[lower,upper](T in){return std::clamp(in, lower, upper);});
            return derived();
        }

        [[nodiscard]] constexpr Child adjust_brightness(T factor) requires (Child::is_alpha()) {
            Child temp_col;
            std::transform(derived().begin(),derived().end()-1,temp_col.begin(),[factor](T in){return in * factor;});
            temp_col.tail() = derived().tail();
            return temp_col;
        }

        constexpr Child adjust_brightness_in_place(T factor) requires (Child::is_alpha()) {
            std::transform(derived().begin(),derived().end()-1,derived().begin(),[factor](T in){return in * factor;});
            return derived();
        }
        
        [[nodiscard]] constexpr Child adjust_brightness(T factor) requires (Child::is_standard()){
            Child temp_col;
            std::transform(derived().cbegin(),derived().cend(),temp_col.begin(),[factor](T in){return in * factor;});
            return temp_col;
        }

        constexpr Child adjust_brightness_in_place(T factor) requires(Child::is_standard()){
            std::transform(derived().begin(),derived().end(),derived().begin(),[factor](T in){return in * factor;});
            return derived();
        }

    };
    
    
    class RGB : public ContainerN<RGB,float,3>, public ColorOpsMixin<RGB,float,3> {

 public:
        constexpr static bool is_alpha(){
            return true;
        }

        [[nodiscard]] constexpr auto&& R(this auto&& self) {
            return std::forward_like<decltype(self)>(self[1]);
        }
        [[nodiscard]] constexpr auto&& G(this auto&& self) noexcept {
            return std::forward_like<decltype(self)>(self[1]);
        }
        [[nodiscard]] constexpr auto&& B(this auto&& self) noexcept{
            return std::forward_like<decltype(self)>(self[1]);
        }


    };





}
