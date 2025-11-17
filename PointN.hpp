#include <type_traits>
#include <cassert>
#include <functional>
#include "ES_math.hpp"
#include "VectorN.hpp"
#include "ContainerN.hpp"

#pragma once


namespace ES {
    template <typename T, std::size_t N> requires(std::is_arithmetic_v<T>)
    class PointN: public ContainerN<PointN,T,N> {
    
    private:
        using ContainerN<PointN,T,N>::data_;
        
    public:
        using ContainerN<PointN,T,N>::zip_in_place;
        using ContainerN<PointN,T,N>::zip;
        using ContainerN<PointN,T,N>::zip_reduce;
        using ContainerN<PointN,T,N>::begin;
        using ContainerN<PointN,T,N>::end;
        using ContainerN<PointN,T,N>::cbegin;
        using ContainerN<PointN,T,N>::cend;
        using ContainerN<PointN,T,N>::ContainerN;



        /** @defgroup accessors Accessors
        *  @brief Convenient element accessors for VectorN (x, y, z, w, head, tail).
        *  @{
        */

        /**
        * @brief Returns the x component of the vector.
        * @note Vector must be smaller than 5
        * @return Reference to the first element.
        */
        [[nodiscard]] constexpr auto&& x(this auto&& self) noexcept requires (N>0 && N<5) {
            return std::forward_like<decltype(self)>(self[0]);
        }

        /**
        * @brief Returns the y component of the vector.
        * @note Vector must be smaller than 5, and bigger than 1
        * @return Reference to the second element.
        */
        [[nodiscard]] constexpr auto&& y(this auto&& self) noexcept requires (N>1 && N<5) {
            return std::forward_like<decltype(self)>(self[1]);
        }

        /**
        * @brief Returns the z component of the vector.
        * @note Vector must be smaller than 5, and bigger than 2
        * @return Reference to the third element.
        */
        [[nodiscard]] constexpr auto&& z(this auto&& self) noexcept requires (N>2 && N<5) {
            return std::forward_like<decltype(self)>(self[2]);
        }

        /**
        * @brief Returns the w component of the vector.
        * @note Vector must have size exactly 4
        * @return Reference to the fourth element.
        */
        [[nodiscard]] constexpr auto&& w(this auto&& self) noexcept requires (N==4) {
            return std::forward_like<decltype(self)>(self[3]);
        }

        /** @} */

        /** 
        * @brief Component wise addition. Adds a vector to a position
        * @return new Position 
        */
        [[nodiscard]] constexpr PointN operator+(ES::VectorN<T,N> rhs)const noexcept{
            return math::zip(*this,rhs,std::plus());
        }
        
        /** 
        * @brief Component wise friend function addition. Adds a position to a vector
        * @return new Position 
        */
        [[nodiscard]] friend constexpr PointN operator+(ES::VectorN<T,N> lhs, PointN rhs) noexcept {
            return math::zip(rhs, lhs, std::plus());;
        }

        /** 
        * @brief Component wise in place function addition. Adds a position to a vector
        * @return Reference to self post modification
        */
        constexpr PointN &operator+=(ES::VectorN<T,N> rhs)noexcept {
            return zip_in_place(rhs,std::plus());
        }
        /**
        * @brief Component wise subtraction. Subtracts a PositoinN from a PointN
        * @return VectorN of same size and type as original PointN
        */
        [[nodiscard]] constexpr ES::VectorN<T,N> operator-(PointN rhs) const noexcept {
           VectorN<T,N> tempVec;
            return math::zip_into(*this,rhs,tempVec,std::minus());
        }

        /**
        * @brief Component wise subtraction. Subtracts a VectorN from this PointN.
        * @return PointN after subtraction.
        */
        [[nodiscard]] constexpr PointN operator-(ES::VectorN<T,N> rhs)const noexcept{
            return zip(rhs,std::minus());
        }

        /**
        * @brief Component wise in place subtraction. Subtracts a VectorN from this PointN.
        * @return Reference to self post modification.
        */
        constexpr PointN& operator-=(ES::VectorN<T,N> rhs) noexcept{
            return zip_in_place(rhs,std::minus());
        }


        /**
         * @brief scalar  multiplication for position
         * @return Position after mutilpying
         */
        [[nodiscard]] constexpr PointN operator*(T scalar)noexcept{
            PointN tempPos;
            std::transform(cbegin(),cend(),tempPos.begin(),[scalar](T in){return in * scalar;});
            return tempPos;
        }

        /**
         * @brief scalar multiplication friend function for position
         * @return Positon after multiplying
         */
        [[nodiscard]] friend constexpr PointN operator*(T scalar, PointN pos){
            PointN tempPos;
            std::transform(pos.begin(),pos.end(),tempPos.begin(), [scalar](T in){return in * scalar;});
            return tempPos;
        }

        /**
         * @brief scalar in place multiplication for position
         * @return Position after mutilpying
         */
        constexpr PointN operator*=(T scalar)noexcept{
            std::transform(begin(),end(),begin(),[scalar](T in){return in* scalar;});
            return *this;
        }


        /**
        * @brief scalar division 
        * @note Division by zero triggers an assert in debug mode,
        *       but returns a zero position in release mode.
        */
        [[nodiscard]] constexpr PointN operator/(const T scalar) const noexcept{
            PointN tempPos;
            std::transform(begin(), end(), tempPos.begin(),[scalar](T in) {assert(scalar !=0 && "Divide by zero in operator/"); return (scalar != T{0}) ? (in / scalar) : T{0}; });
            return tempPos;
        }

        /**
        * @brief In-place component-wise division by const scaler
        * @note Division by zero triggers an assert in debug mode,
        *       but returns a zero position in release mode.
        */
        constexpr PointN& operator/=(const T scalar) noexcept{
            std::transform(begin(), end(),begin(),[scalar](T in) {assert(scalar !=0 && "Divide by zero in operator/="); return (scalar != 0) ? (in / scalar) : T{0}; });
            return *this;
        }
    
        /**
        * @brief compute the distance from this Position to another Position
        * @param rhs the other position to mesure distance to
        * @return Scalar distance between the two positions
        */
        [[nodiscard]] constexpr T distance(const PointN& rhs) const noexcept {
             return std::sqrt(std::fabs(zip_reduce(*this, 0,[](T accum, T l, T r){return accum+(l*r);})));
        }

        /**
        * @brief compute the squared distance from this Position to another position
        * @param rhs the other position to mesure distance to
        * @return squared distance as scalar of same type as Position
        */
        [[nodiscard]] constexpr T distance_squared(const PointN& rhs) const noexcept {
            return std::fabs(zip_reduce(*this, 0,[](T accum, T l, T r){return accum+(l*r);}));
        }

        /**
        * @brief linearly interpolate between this Position and another
        * @param rhs the target position to interpolate too
        * @param t interapolation factor, 0 returns this, 1 returns rhs
        * @return new PointN at the interpolated location
        */
        [[nodiscard]] constexpr PointN lerp(const PointN& rhs, T t) const noexcept {
            return zip(rhs,[t](T a, T b) {return a+(b-a)*t;});
        }

        /**
        * @brief in-place linearly interpolate this Position towards another
        * @param rhs the target position to interpolate to
        * @param t interpolation factor, 0 leaves this unchanged, 1 sets to rhs
        * @return reference to this Position post modification
        */
        constexpr PointN& lerp_in_place(PointN rhs, T t) noexcept{
            return zip_in_place(rhs, [t](T a,T b) {return a+(b-a)*t;});
        }

        /**
        * @brief find the mid point between this Position and another
        * @param rhs the other position
        * @return new PointN exactly in the middle of this and rhs
        */
        [[nodiscard]] constexpr PointN mid_point(const PointN& rhs) const noexcept {
            return lerp(rhs, T{0.5});
        }

        /**
        * @brief compute barycentric coordinates given 3 positions and weights
        * @param A first position
        * @param B second position
        * @param C third position
        * @param u weight for A
        * @param v weight for B
        * @param w weight for C
        * @return new PointN at the barycentric combination of A, B, C
        */
        [[nodiscard]] friend constexpr PointN barycentric(const PointN A, const PointN B, const PointN C,T u, T v, T w) noexcept {
            return math::tri_zip(A, B, C, [u,v,w](auto a, auto b, auto c) { return a*u + b*v + c*w; });
        }

        /**
         * @brief Returns a vector representing the vector between the origin and point
         * 
         * @return VectorN with each component as the distance from origin 
         */
        [[nodiscard]] constexpr VectorN<T,N> to_vector(){
            VectorN<T,N> tempVec;
            std::copy(data_.cbegin(), data_.cend(), tempVec.begin());
            return tempVec;
        }

    };
}  