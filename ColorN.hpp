#pragma once
#include <type_traits>
#include <cassert>
#include <cmath>
#include <type_traits>
#include <algorithm>
#include <functional>
#include "ContainerN.hpp"
#include "config.hpp"

namespace ES{
    
    template <typename T, std::size_t N> 
    class ColorN: ContainerN<ColorN,T,N>{


        public:
        using ContainerN<ColorN,T,N>::zip_in_place;
        using ContainerN<ColorN,T,N>::zip;
        using ContainerN<ColorN,T,N>::zip_reduce;
        using ContainerN<ColorN,T,N>::begin;
        using ContainerN<ColorN,T,N>::end;
        using ContainerN<ColorN,T,N>::cend;
        using ContainerN<ColorN,T,N>::cbegin;
        using ContainerN<ColorN,T,N>::data_;
        using ContainerN<ColorN,T,N>::ContainerN;

        /**
        * @brief Returns the Luminance component of the Color.
        * @note Vector must have size exactly 2 
        * @return Reference to the first element.
        */
        [[nodiscard]] constexpr auto&& L(this auto&& self) noexcept requires (N>0 && N<3) {
            return std::forward_like<decltype(self)>(self[0]);
        }


        /**
        * @brief Returns the Alpha component of the Color.
        * @note Vector must have size exactly 2 
        * @return Reference to the second element.
        */
        [[nodiscard]] constexpr auto&& A(this auto&& self) noexcept requires (N==2) {
            return std::forward_like<decltype(self)>(self[1]);
        }


        /**
        * @brief Returns the Red component.
        * @note Vector must be smaller than 5, and bigger than 1
        * @return Reference to the second element.
        */
        [[nodiscard]] constexpr auto&& R(this auto&& self) noexcept requires (N>2 && N<5) {
            return std::forward_like<decltype(self)>(self[0]);
        }

        /**
        * @brief Returns the Green component.
        * @note Vector must be smaller than 5, and bigger than 1
        * @return Reference to the second element.
        */
        [[nodiscard]] constexpr auto&& G(this auto&& self) noexcept requires (N>2 && N<5) {
            return std::forward_like<decltype(self)>(self[1]);
        }

        /**
        * @brief Returns the Blue component.
        * @note Vector must be smaller than 5, and bigger than 2
        * @return Reference to the third element.
        */
        [[nodiscard]] constexpr auto&& B(this auto&& self) noexcept requires (N>2 && N<5) {
            return std::forward_like<decltype(self)>(self[2]);
        }

        /**
        * @brief Returns the Alpha component of the vector.
        * @note Vector must have size exactly 4
        * @return Reference to the fourth element.
        */
        [[nodiscard]] constexpr auto&& A(this auto&& self) noexcept requires (N==4) {
            return std::forward_like<decltype(self)>(self[3]);
        }


        [[nodiscard]] constexpr  ColorN operator+(ColorN rhs) const{
            return zip(rhs,std::plus{});
        }

        constexpr ColorN& operator+=(ColorN rhs){
            return zip_in_place(rhs,std::plus());
        }

        [[nodiscard]] constexpr ColorN operator-(ColorN rhs) const{
            return zip(rhs, std::minus());
        }

        constexpr ColorN& operator-=(ColorN rhs){
            return zip_in_place(rhs,std::minus());
        }

        [[nodiscard]] constexpr ColorN operator*(ColorN rhs)const{
            return zip(rhs,std::multiplies());
        }

        constexpr ColorN operator*=(ColorN rhs){
            return zip_in_place(rhs,std::multiplies());
        }

        [[nodiscard]] constexpr ColorN operator/(ColorN rhs)const{
            return zip(rhs, [](T a, T b) { assert(b !=0 && "Divide by zero in component division"); return (b != 0) ? (a / b) : T{0}; });
        }

        constexpr ColorN operator/=(ColorN rhs){
            return zip_in_place(rhs, [](T a, T b) { assert(b !=0 && "Divide by zero in component division"); return (b != 0) ? (a / b) : T{0}; });
        }

        [[nodiscard]] constexpr ColorN operator*(T scalar) const{
            ColorN tempVec;
            std::transform(cbegin(),cend(),tempVec.begin(),[scalar](T in){return in * scalar;});
            return tempVec;
        }

        [[nodiscard]] constexpr friend ColorN operator*(T scalar, ColorN lhs){
            ColorN tempVec;
            std::transform(lhs.cbegin(),lhs.cend(),tempVec.begin(),[scalar](T in){return in * scalar;});
            return tempVec;
        }

        constexpr ColorN& operator*=(T scalar){
            std::transform(begin(),end(),begin(),[scalar](T in){return in * scalar;});
            return *this;
        }

        [[nodiscard]] constexpr ColorN operator/(T scalar) const{
            ColorN tempCol;
            std::transform(cbegin(), cend(), tempCol.begin(),[scalar](T in) {assert(scalar !=0 && "Divide by zero in operator/"); return (scalar != 0) ? (in / scalar) : T{0}; });
            return tempCol;
        }

        constexpr ColorN& operator/=(T scalar){
            std::transform(cbegin(), cend(), begin(),[scalar](T in) {assert(scalar !=0 && "Divide by zero in operator/"); return (scalar != 0) ? (in / scalar) : T{0}; });
            return *this;
        }

        [[noimplement]] [[nodiscard]] constexpr ColorN lerp(ColorN rhs, T t)const{}

        [[noimplement]] constexpr ColorN& lerp_in_place(ColorN rhs, T t){}

        [[noimplement]] [[nodiscard]] constexpr ColorN clamp(T lower, T upper) const{}

        [[noimplement]] constexpr ColorN& clamp_in_place(T lower, T upper){}
        
        [[noimplement]] auto to_linear_premultiplied(){}
        
        [[noimplement]] auto to_linear_straight(){}
        
        [[noimplement]] auto to_srgb_straight(){}
        
        [[noimplement]] auto to_srgb_premultiplied(){}
        
        [[noimplement]] [[nodiscard]] ColorN to_straight() const requires(N==2 || N==4){}
        
        [[noimplement]] [[nodiscard]] ColorN to_SRGB() const{}
        
        [[noimplement]] [[nodiscard]] T luminance() const{}
        
        [[noimplement]] [[nodiscard]] ColorN adjust_saturation(T){}
        
        [[noimplement]] [[nodiscard]] ColorN adjust_brightness(T){}
        
        
    };
    using RGB = ColorN<float,3>;
    using RGBA = ColorN<float,4>;
    using LA = ColorN<float,2>;
    using RGB8 = ColorN<int8_t,3>;
    using RGBA8 = ColorN<int8_t,4>;


    namespace factory{


        // I may be addicted to the fold
        template<typename... Args>
        static constexpr auto from_linear_straight(Args... args) requires (sizeof...(Args) == 2 || sizeof...(Args)==4) {
            ColorN<std::common_type_t<Args...>, sizeof...(Args)> tempColor(args...);
            auto a = tempColor.A();
            std::transform(tempColor.begin(), tempColor.end()-1,tempColor.begin(),[a](auto in){return in*a;});
            return tempColor; 
        }

    
        template<typename... Args>
        [[nodiscard]] static constexpr auto from_linear_premultiplied(Args... args) requires (sizeof...(Args) == 2 || sizeof...(Args)==4) {
             return ColorN<std::common_type_t<Args...>, sizeof...(Args)>(args...);
        }

        template<typename... Args>
        static constexpr auto from_linear(Args... args){
            return ColorN<std::common_type_t<Args...>, sizeof...(Args)>(args...);
        }

        template<typename... Args>
        [[noimplement]] static constexpr auto from__srgb(Args... args) requires (std::is_integral_v<std::common_type_t<Args...>>){
                ColorN<real,sizeof...(args)> tempcol(static_cast<real>(args / 255.0f)...);
        }

        template<typename... Args>
        [[noimplement]] static constexpr auto from__srgb(Args... args){
                
        }

    
        template<typename... Args>
        [[noimplement]] static constexpr auto from_srgb_straight(Args... args){}
        
        template<typename... Args>
        [[noimplement]] static constexpr auto from_srgb_premultiplied(Args... args){ }


        [[noimplement]] static constexpr auto from_hex(int hex){
            
        }
    }
    
};