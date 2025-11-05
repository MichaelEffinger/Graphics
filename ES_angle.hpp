//
// Created by Comedic Genuis on 10/27/2025.
//

#ifndef COMPUTERGRAPHICS_ES_ANGLE_HPP
#define COMPUTERGRAPHICS_ES_ANGLE_HPP
#include <type_traits>
#include <numbers>
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

namespace ES::math {
    //Numeric only, hence C, function that converts a degrees into radians, both represented as plain old floating point types.
    template <std::floating_point T> [[nodiscard]] constexpr T c_deg_to_rad(T) noexcept;
    //Numeric only, hence C, function that converts radians into degrees, both represented as plain old floating point types.
    template <std::floating_point T> [[nodiscard]] constexpr T c_rad_to_deg(T) noexcept;
}

template<std::floating_point T>
constexpr T ES::math::c_deg_to_rad(const T z) noexcept {
    return z * std::numbers::pi_v<T> / T(180); //TODO: decouple it from numbers pi so it works for any arbitrary type that can multiply.
}

template<std::floating_point T>
constexpr T ES::math::c_rad_to_deg(const T z) noexcept {
    return z * T(180) / std::numbers::pi_v<T>; //TODO: decouple it from numbers pi so it works for any arbitrary type that can multiply.
}


template <typename T, typename V> requires std::is_same_v<T, ES::Secret::degree> || std::is_same_v<T, ES::Secret::radian>
class ES::angle {
    V angle_;
public:
    constexpr angle() = default;
    constexpr angle(V v) : angle_(v){}

    //implicit cast for angles of the same unit, but different type.
    template<typename oV>
    constexpr operator angle<T, oV>() const noexcept {
        return angle<T,oV>{angle_}; //ensures narrowing yells at you
    }

    //conversion that takes a radian type and makes it into a degree type!
    template<typename oV>
    constexpr operator angle<in_degrees, oV>() const noexcept requires (not std::is_same_v<T, in_degrees>){
        return angle<in_degrees, oV>(math::c_rad_to_deg(angle_));
    }

    //conversion that takes a degree type and makes it into a radian type!
    template<typename oV>
    constexpr operator angle<in_radians, oV>() const noexcept requires (not std::is_same_v<T, in_radians>){
        return angle<in_radians, oV>(math::c_deg_to_rad(angle_));
    }

    //funky operators
    [[nodiscard]] constexpr angle operator+(angle rhs) const { return angle{angle_ + rhs.angle_}; }
    [[nodiscard]] constexpr angle operator-(angle rhs) const { return angle{angle_ - rhs.angle_}; }
    [[nodiscard]] constexpr angle operator*(V scalar) const { return angle{angle_ * scalar}; }
    [[nodiscard]] constexpr angle operator/(V scalar) const { return angle{angle_ / scalar}; }
    [[nodiscard]] constexpr angle operator/(angle rhs) const { return angle{angle_ / rhs.angle_}; }
    //funky compound assignment operators
    constexpr angle& operator+=(angle rhs) { angle_ += rhs.angle_; return *this; }
    constexpr angle& operator-=(angle rhs) { angle_ -= rhs.angle_; return *this; }
    constexpr angle& operator*=(V scalar) { angle_ *= scalar; return *this; }
    constexpr angle& operator/=(V scalar) { angle_ /= scalar; return *this; }
    constexpr angle& operator/=(angle rhs) const { angle_ /= rhs.angle_; return *this; }
    //everyone's favorite spaceship
    [[nodiscard]] constexpr auto operator<=>(const angle&) const = default;
};

namespace ES::math::angle_literals {
    constexpr angle<in_radians, long double> operator""_rad(long double z) {
        return angle<Secret::radian, long double>{z};
    }

    constexpr angle<in_degrees, long double> operator""_deg(long double z) {
        return angle<Secret::degree, long double>{z};
    }

    constexpr angle<in_radians, long double> operator""_rad(unsigned long long z) {
        return angle<Secret::radian, long double>{static_cast<long double>(z)};
    }

    constexpr angle<in_degrees, long double> operator""_deg(unsigned long long z) {
        return angle<Secret::degree, long double>{static_cast<long double>(z)};
    }
}

#endif //COMPUTERGRAPHICS_ES_ANGLE_HPP