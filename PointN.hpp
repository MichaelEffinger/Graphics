#include <type_traits>
#include <cassert>
#include <functional>
#include "ES_math.hpp"
#include "VectorN.hpp"
#include "ContainerN.hpp"

#pragma once


namespace ES {
    template <typename T, std::size_t N> requires(std::is_arithmetic_v<T>)
    class PointN: public ContainerN<PointN<T,N>,T,N>, public ArithmeticOpsMixin<PointN<T,N>,T,N>  {
    
    private:
        using ContainerN<PointN,T,N>::data_;
        
    public:
        using ContainerN<PointN,T,N>::zip_in_place;
        using ContainerN<PointN,T,N>::data;
        using ContainerN<PointN,T,N>::zip;
        using ContainerN<PointN,T,N>::zip_reduce;
        using ContainerN<PointN,T,N>::begin;
        using ContainerN<PointN,T,N>::end;
        using ContainerN<PointN,T,N>::cbegin;
        using ContainerN<PointN,T,N>::cend;
        using ContainerN<PointN,T,N>::ContainerN;
        using ArithmeticOpsMixin<PointN,T,N>::lerp_in_place;
        using ArithmeticOpsMixin<PointN,T,N>::lerp;



        constexpr static void can_scalar_multiply(){return;};
        constexpr static void can_scalar_divide(){return;};
        constexpr static void can_lerp(){return;};
        constexpr static void can_clamp(){return;};



         /**
        * @brief Construct a ContainerN form an N-J sized Container along with J
        * other parameters
        *
        * This is a variadic template contsructor that takes exactly one smaller
        * Container of size N-J along with J other parameters. These together must
        * be of size exactly N. The parameter arguments are static converted to to
        * T.
        *
        * @tparam M The size of ContainerN, which is the first parameter
        * @tparam U Parameeter pack representing each of the remaining elements.
        *         Must have exactly size M-N
        * @param smaller The Container of the first values to initialize in the
        * Container
        * @param extras The remaining values to initialize in the Container
        *
        * @note This constructor is `constexpr` and `noexcept` if all elements
        *       if all element construction are noexcept.
        *
        * @example
        * ContainerN<float,2> v2(1.0f,2.0f);
        * ContainerN<float,5> v5(v2,3.0f,4.0f,5.0f);
        */
        template <size_t M, typename... U> requires(sizeof...(U) == N - M)
        constexpr PointN(const VectorN<T, M> &smaller, U... extras) noexcept((std::is_nothrow_constructible_v<T, U &&> && ...)) : ContainerN<PointN, T, N>{} {
            std::copy(smaller.cbegin(), smaller.cend(), data_.begin());
            size_t index = M;
            ((data_[index++] = T(static_cast<T>(extras))), ...);
        }



        /** @defgroup accessors Accessors
        *  @brief Convenient element accessors for PointN (x, y, z, w, head, tail).
        *  @{
        */

        /**
        * @brief Returns the x component of the Point.
        * @note Point must be smaller than 5
        * @return Reference to the first element.
        */
        [[nodiscard]] constexpr auto&& x(this auto&& self) noexcept requires (N>0 && N<5) {
            return std::forward_like<decltype(self)>(self[0]);
        }

        /**
        * @brief Returns the y component of the Point.
        * @note Point must be smaller than 5, and bigger than 1
        * @return Reference to the second element.
        */
        [[nodiscard]] constexpr auto&& y(this auto&& self) noexcept requires (N>1 && N<5) {
            return std::forward_like<decltype(self)>(self[1]);
        }

        /**
        * @brief Returns the z component of the Point.
        * @note Point must be smaller than 5, and bigger than 2
        * @return Reference to the third element.
        */
        [[nodiscard]] constexpr auto&& z(this auto&& self) noexcept requires (N>2 && N<5) {
            return std::forward_like<decltype(self)>(self[2]);
        }

        /**
        * @brief Returns the w component of the Point.
        * @note Point must have size exactly 4
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
            return math::zip(*this,rhs,std::plus{});
        }
        
        /** 
        * @brief Component wise friend function addition. Adds a position to a vector
        * @return new Position 
        */
        [[nodiscard]] friend constexpr PointN operator+(ES::VectorN<T,N> lhs, PointN rhs) noexcept {
            return math::zip(rhs, lhs, std::plus{});;
        }

        /** 
        * @brief Component wise in place function addition. Adds a position to a vector
        * @return Reference to self post modification
        */
        constexpr PointN &operator+=(ES::VectorN<T,N> rhs)noexcept {
            return zip_in_place(rhs,std::plus{});
        }
        /**
        * @brief Component wise subtraction. Subtracts a PositoinN from a PointN
        * @return VectorN of same size and type as original PointN
        */
        [[nodiscard]] constexpr ES::VectorN<T,N> operator-(PointN rhs) const noexcept {
           VectorN<T,N> tempVec;
            return math::zip_into(*this,rhs,tempVec,std::minus{});
        }

        /**
        * @brief Component wise subtraction. Subtracts a VectorN from this PointN.
        * @return PointN after subtraction.
        */
        [[nodiscard]] constexpr PointN operator-(ES::VectorN<T,N> rhs)const noexcept{
            return zip(rhs,std::minus{});
        }

        /**
        * @brief Component wise in place subtraction. Subtracts a VectorN from this PointN.
        * @return Reference to self post modification.
        */
        constexpr PointN& operator-=(ES::VectorN<T,N> rhs) noexcept{
            return zip_in_place(rhs,std::minus{});
        }

        /**
        * @brief compute the distance from this Position to another Position
        * @param rhs the other position to mesure distance to
        * @return Scalar distance between the two positions
        */
        [[nodiscard]] constexpr T distance(const PointN& rhs) const noexcept {
             return std::sqrt(zip_reduce(rhs, T{0}, [](T accum, T l, T r){T d = l - r; return accum + d*d;}));
        }

        /**
        * @brief compute the squared distance from this Position to another position
        * @param rhs the other position to mesure distance to
        * @return squared distance as scalar of same type as Position
        */
        [[nodiscard]] constexpr T distance_squared(const PointN& rhs) const noexcept {
             return zip_reduce(rhs, T{0}, [](T accum, T l, T r){T d = l - r; return accum + d*d;});
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