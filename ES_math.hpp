#pragma once
#include <cmath>
#include "ES_concepts.hpp"
#include <type_traits>
#include <limits> //Do we do this in house? To what end? I cry.
import ES_math;

namespace ES::math {
template <typename T> struct default_epsilon;

    template <> struct default_epsilon<float> {
        static constexpr float value = 1e-5f;
    };
    template <> struct default_epsilon<double> {
        static constexpr double value = 1e-9;
    };

    template <> struct default_epsilon<long double> {
        static constexpr long double value = 1e-12L;
    };

    template <std::integral T> struct default_epsilon<T> {
        static constexpr T value = 0;
    };


    template <typename T, typename H>
    constexpr bool approx_equal(T lhs, H rhs, T epsilon = default_epsilon<T>::value) noexcept {
        return ES::math::abs(lhs - static_cast<T>(rhs)) <= epsilon;
    }

    template <typename Container1, typename Container2, typename BinaryOp> requires(concepts::Iterable<Container1> && concepts::Iterable<Container2>)
    [[nodiscard]] constexpr Container1 zip(Container1 lhs, Container2 rhs, BinaryOp op) noexcept {
        Container1 resultant;
        auto liter = lhs.cbegin(), riter = rhs.cbegin();
        auto oiter = resultant.begin();
        while (liter != lhs.cend()) {
            *oiter = op(*liter, *riter);
            ++liter, ++riter, ++oiter;
        }
        return resultant;
    }

    template <typename Container1, typename Container2, typename Container3, typename BinaryOp> requires(concepts::Iterable<Container1> && concepts::Iterable<Container2> && concepts::Iterable<Container3>)
    constexpr Container3 &zip_into(const Container1 lhs, const Container2 rhs, Container3& dest, BinaryOp op) noexcept {
        auto liter = lhs.cbegin(), riter = rhs.cbegin();
        auto oiter = dest.begin();
        while (liter != lhs.cend()) {
            *oiter = op(*liter, *riter);
            ++liter, ++riter, ++oiter;
        }
        return dest;
    }

    template <typename Container1, typename Container2, typename BinaryOp, typename T = typename Container1::value_type>
    [[nodiscard]] constexpr T zip_reduce(const Container1 lhs, const Container2 rhs, T initial, concepts::FoldExpr<T> auto&& exp) noexcept {
        auto liter = lhs.cbegin(), riter = rhs.cbegin();
        while(liter != lhs.cend()){
            initial = exp(initial, *liter, *riter);
            ++liter, ++riter;
        }
        return initial;
    }


    template <typename Container1, typename Container2, typename Container3, typename BinaryOp> requires(concepts::Iterable<Container1> && concepts::Iterable<Container2> && concepts::Iterable<Container3>)
    [[nodiscard]] constexpr Container1 tri_zip(Container1 lhs, Container2 mhs, Container3 rhs, BinaryOp op) noexcept {
        Container1 resultant;
        auto liter = lhs.cbegin(), riter = rhs.cbegin(), miter = mhs.cbegin();
        auto oiter = resultant.begin();
        while (liter != lhs.cend()) {
            *oiter = op(*liter, *miter, *riter);
            ++liter, ++miter, ++riter, ++oiter;
        }
        return resultant;
    }

    template<typename T> [[nodiscard]] constexpr T absolute_value(const T N) noexcept {
        if (N == 0) return N; //zero's signedness is important to keep, or so I am told...
        return N > 0 ? N : N * -1;
    }








    //template values for pi tau and half_pi, just there exact representation, little here little there
    template<typename T> inline constexpr T pi       = T(3.14159265358979323846264338327950288L); 
    template<typename T> inline constexpr T tau      = T(6.28318530717958647692528676655900576L); 
    template<typename T> inline constexpr T half_pi  = T(1.57079632679489661923132169163975144L); 



    

}
