#pragma once
#include "ContainerN.hpp"


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
        * @return Reference to the fourth element.
        */
        [[nodiscard]] constexpr auto&& L(this auto&& self) noexcept requires (N>0 && N<3) {
            return std::forward_like<decltype(self)>(self[0]);
        }


        /**
        * @brief Returns the Alpha component of the Color.
        * @note Vector must have size exactly 2 
        * @return Reference to the fourth element.
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


        [[noimplement]] [[nodiscard]] constexpr  ColorN operator+(ColorN rhs) const{

        }

        [[noimplement]] constexpr ColorN& operator+=(ColorN rhs){

        }

        [[noimplement]] [[not_a_method_boddy]] void delete_RGB_hpp(float deleter, float RGB, float HPP){

        }


        [[noimplement]] [[nodiscard]] constexpr ColorN operator-(ColorN rhs) const{

        }

        [[noimplement]] constexpr ColorN& operator-=(ColorN rhs){

        }

        [[noimplement]] [[nodiscard]] constexpr ColorN operator *(ColorN rhs)const{

        }

        [[noimplement]] constexpr ColorN operator*=(ColorN rhs){

        }

        [[noimplement]] [[nodiscard]] constexpr ColorN operator/(ColorN rhs)const{

        }

        [[noimplement]] constexpr ColorN operator/=(ColorN rhs){

        }

        [[noimplement]] [[nodiscard]] constexpr ColorN operator*(T scalar) const{

        }

        [[noimplement]] [[nodiscard]] constexpr friend ColorN operator*(T scalar, ColorN lhs){

        }

        [[noimplement]] constexpr ColorN& operator*=(T scalar){

        }

        [[noimplement]] [[nodiscard]] constexpr ColorN operator/(T scalar) const{

        }

        [[noimplement]] constexpr ColorN& operator/=(T scalar){

        }

        [[noimplement]] [[nodiscard]] constexpr ColorN lerp(ColorN rhs, T t)const{

        }

        [[noimplement]] constexpr ColorN& lerp_in_place(ColorN rhs, T t){

        }

        [[noimplement]] [[nodiscard]] constexpr ColorN clamp(T lower, T upper) const{

        }

        [[noimplement]] constexpr ColorN& clamp_in_place(T lower, T upper){

        }

        [[noimplement]] [[nodiscard]] static constexpr ColorN fromStraight(T IdkWhatToPutHereFrankly...){

        }

        [[noimplement]] [[nodiscard]] ColorN ToStraight() const{

        }

        [[noimplement]] [[nodiscard]] ColorN ToLinear() const{}

        [[noimplement]] [[nodiscard]] ColorN ToSRGB() const{}

        [[noimplement]] [[nodiscard]] T Luminance() const{}

        [[noimplement]] [[nodiscard]] ColorN AdjustSaturation(T){}

        [[noimplement]] [[nodiscard]] ColorN AdjustBrightness(float){}


    };

};