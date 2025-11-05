#include <cmath>
#include <type_traits>
#pragma once

    namespace ES::math {
         template<typename T>
        struct default_epsilon;

        template<>
        struct default_epsilon<float> {
            static constexpr float value = 1e-5f;
        };
        template<>
        struct default_epsilon<double> {
            static constexpr double value = 1e-9;
        };

        template<>
        struct default_epsilon<long double> {
            static constexpr long double value = 1e-12L;
        };

        template<std::integral T>
        struct default_epsilon<T> {
            static constexpr T value = 0;
        };

        template<typename T, typename H> requires (std::is_arithmetic_v<T>)
        inline bool approx_equal(T lhs, H rhs, T epsilon = default_epsilon<T>::value) noexcept {
            return std::fabs(lhs - static_cast<T>(rhs)) <= epsilon;
        }

    }   