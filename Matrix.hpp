#include <type_traits>
#include <cassert>
#include <cmath>
#include <algorithm>
#include <functional>
#include <string>
#include "ContainerN.hpp"
#include <cstdint>
#include "ArithmeticOpsMixin.hpp"
#include "VectorN.hpp"

namespace ES{



    // I think I am going to go with column major... too popular in graphics to ignore
    template<typename T, std::size_t N, std::size_t M = N>
    class Matrix : public ArithmeticOpsMixin<Matrix<T,N,M>, T, N*M>, public ContainerN<Matrix<T,N,M>,T,N*M>{
       
        using ContainerN<Matrix,T,N>::data_;
        public: 

        using ContainerN<Matrix,T,N>::zip_in_place;
        using ContainerN<Matrix,T,N>::zip;
        using ContainerN<Matrix,T,N>::zip_reduce;
        using ContainerN<Matrix,T,N>::begin;
        using ContainerN<Matrix,T,N>::end;
        using ContainerN<Matrix,T,N>::cend;
        using ContainerN<Matrix,T,N>::data;
        using ContainerN<Matrix,T,N>::cbegin;
        using ContainerN<Matrix,T,N>::ContainerN;


        static constexpr void can_scalar_multiply(){return;}
        static constexpr void can_scalar_divide(){return;}
        static constexpr void can_component_add(){return;}
        static constexpr void can_component_subtract(){return;}
        static constexpr void can_lerp(){return;}
        static constexpr void can_negate(){return;}
        static constexpr void can_clamp



        constexpr T operator()(std::size_t row, std::size_t column) const noexcept{};
        constexpr T by_row(std::size_t row, std::size_t column) const noexcept{};
        constexpr T by_col(std::size_t column, std::size_t row)const noexcept {};

        constexpr T determinant() const noexcept {};
        constexpr VectorN<T,N> column(std::size_t I) const noexcept{};
        constexpr Matrix transpose() const noexcept{};
        constexpr Matrix inverse() const noexcept{};
        constexpr Matrix identity() const noexcept{};
        constexpr T trace() const noexcept{};
        constexpr Matrix adjugate() const noexcept{};


        auto get() noexcept {
            return data_;
        }

         auto get_row_major() {
            return data_;
        }

        auto get_col_major() {
            return data_;
        }


    };



}