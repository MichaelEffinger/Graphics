//
// Created by Comedic Genuis on 10/27/2025.
//

#ifndef COMPUTERGRAPHICS_ES_ANGLE_HPP
#define COMPUTERGRAPHICS_ES_ANGLE_HPP
#include <type_traits>
#include <compare>

namespace ES {
    namespace Secret {
        class radian{};
        class degree{};
    }
    template <typename T, typename V = float> requires std::is_same_v<T, ES::Secret::degree> || std::is_same_v<T, ES::Secret::radian>
    class angle;
    using in_degrees = Secret::degree;
    using in_radians = Secret::radian;

    template <typename V = float>
    using angle_deg = angle<in_degrees, V>;
    template <typename V = float>
    using angle_rad = angle<in_radians, V>;
}

// TODO: make these into conversion functions
// inline long double operator""_rad(long double z) {
//     return z * std::numbers::pi_v<long double> / 180.L;
// }
//
// inline long double operator""_deg(long double z) {
//     return z * 180.L / std::numbers::pi_v<long double>;
// }
//

template <typename T, typename V> requires std::is_same_v<T, ES::Secret::degree> || std::is_same_v<T, ES::Secret::radian>
class ES::angle {
    V angle_;
public:
    angle() = default;
    angle(V v) : angle_(v){}

    //implicit cast for angles of the same unit but not of the same floating point type
    template<typename oV>
    operator angle<T, oV>() const {
        return {static_cast<oV>(angle_)};
    }


    // template<typename oV>
    // explicit operator angle<in_radians, oV>() const {
    //     return {operator""_deg(static_cast<oV>(angle_) )};
    // }


    constexpr angle operator+(angle rhs) const { return angle{angle_ + rhs.angle_}; }
    constexpr angle operator-(angle rhs) const { return angle{angle_ - rhs.angle_}; }
    constexpr angle operator*(V scalar) const { return angle{angle_ * scalar}; }
    constexpr angle operator/(V scalar) const { return angle{angle_ / scalar}; }
    constexpr angle operator/(angle rhs) const { return angle{angle_ / rhs.angle_}; }

    constexpr angle& operator+=(angle rhs) { angle_ += rhs.angle_; return *this; }
    constexpr angle& operator-=(angle rhs) { angle_ -= rhs.angle_; return *this; }
    constexpr angle& operator*=(V scalar) { angle_ *= scalar; return *this; }
    constexpr angle& operator/=(V scalar) { angle_ /= scalar; return *this; }
    constexpr angle& operator/=(angle rhs) const { angle_ /= rhs.angle_; return *this; }

    constexpr auto operator<=>(const angle&) const = default;

    friend angle<Secret::radian, long double> operator""_rad(long double z) {
        return angle<Secret::radian, long double>{z};
    }

    friend angle<Secret::radian, long double> operator""_deg(long double z) {
        return angle<Secret::radian, long double>{z};
    }

    //TODO: static? friend? T convert_deg_to_rad(T x){}

};

#endif //COMPUTERGRAPHICS_ES_ANGLE_HPP