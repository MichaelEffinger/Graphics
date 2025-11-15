#pragma once
#include <cmath>
#include "ES_concepts.hpp"
#include <type_traits>

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

    /**
     * @brief A VERY well-behaved floating point floor function.
     * @param[in] N that which shall be floored!
     */
    [[nodiscard]] constexpr float floor(const float N) noexcept {
        //± 2^23 is where decimals die... fun fact: that immediately prior can support ±0.5. Anything above 2^24 jumps by two from then on...
        static constexpr float NOMORE = 8388608;
        if (absolute_value(N) >= NOMORE) return N;
        //positives truncate, which is identical to flooring.
        //Negatives truncate, so -3.9 becomes -3.0 instead of flooring which goes -3.9 -> -4.0.
        //This is why we need to subtract that magic one. So it becomes -3.9 -> -3.0 -> -4.0.
        const float trunk = static_cast<float>(static_cast<long long>(N));
        if (trunk == N) return N; // if we already were an integer, just return.
        return N + (N < 0) * 1.f; //cheeky boolean math
    }

    /**
     * Well-behaved floating point modulo.
     * @param N
     * @param M
     * @return the real number result of N % M
     */
    [[nodiscard]] constexpr float modulo(const float N, const float M) noexcept {
        return N - M * ES::math::floor(N/M);
    }

}
