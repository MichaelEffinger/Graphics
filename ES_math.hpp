#include <cmath>
#include <iterator>
#include "ES_concepts.hpp"
#include <type_traits>
#pragma once

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

    template <typename T, typename H> requires(std::is_arithmetic_v<T>)
    inline bool approx_equal(T lhs, H rhs,T epsilon = default_epsilon<T>::value) noexcept {
        return std::fabs(lhs - static_cast<T>(rhs)) <= epsilon;
    }

    template <typename Container, typename BinaryOp> requires(ES::concepts::Iterable<Container>)
    [[nodiscard]] constexpr Container zip(Container lhs, Container rhs, BinaryOp op) noexcept {
        Container resultant;
        auto liter = lhs.cbegin(), riter = rhs.cbegin();
        auto oiter = resultant.begin();
        while (liter != lhs.cend()) {
            *oiter = op(*liter, *riter);
            ++liter, ++riter, ++oiter;
        }
        return resultant;
    }

    template <typename Container, typename BinaryOp>
        constexpr Container &zip_into(const Container lhs, const Container rhs, Container& dest, BinaryOp op) noexcept {
        auto liter = lhs.cbegin(), riter = rhs.cbegin();
        auto oiter = dest.begin();
        while (liter != lhs.cend()) {
            *oiter = op(*liter, *riter);
            ++liter, ++riter, ++oiter;
        }
        return dest;
    }

    template <typename Container, typename BinaryOp, typename T = typename Container::value_type>
    [[nodiscard]] constexpr T zip_reduce(const Container lhs, const Container rhs, T initial, ES::concepts::FoldExpr<T> auto&& exp) noexcept {
        auto liter = lhs.cbegin(), riter = rhs.cbegin();
        while(liter != lhs.cend()){
            initial = exp(initial, *liter, *riter);
            ++liter, ++riter;
        }
        return initial;
    }

}
