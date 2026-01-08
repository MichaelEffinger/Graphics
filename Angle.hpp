#ifndef COMPUTERGRAPHICS_ES_ANGLE_HPP
#define COMPUTERGRAPHICS_ES_ANGLE_HPP
#include <type_traits>
#include <numbers>
#include <utility>

#include "ES_math.hpp"

namespace ES {
    namespace Secret {
        class radian{};
        class degree{};

        template <typename T> [[nodiscard]] constexpr T normalize_angle_coeff(T, T) noexcept;
    }

    template<typename Unit> concept radian_or_degree = std::same_as<Unit,Secret::degree> || std::same_as<Unit,Secret::radian>;
    template<typename T, typename U> concept not_same_as = not std::same_as<T,U>;

    template <radian_or_degree T, typename V = float>
    class Angle;

    using in_degrees = Secret::degree;
    using in_radians = Secret::radian;

    using AngleDeg = Angle<in_degrees>;
    using AngleRad = Angle<in_radians>;
}

namespace ES::math {
    template <std::floating_point T> [[nodiscard]] constexpr T c_deg_to_rad(T) noexcept;
    template <std::floating_point T> [[nodiscard]] constexpr T c_rad_to_deg(T) noexcept;

    template <std::floating_point T> [[nodiscard]] constexpr T c_normalize_angle_rad(T) noexcept;
    template <std::floating_point T> [[nodiscard]] constexpr T c_normalize_angle_deg(T) noexcept;
}

template<std::floating_point T>
constexpr T ES::math::c_deg_to_rad(const T z) noexcept {
    return z * std::numbers::pi_v<T> / T(180);
}

template<std::floating_point T>
constexpr T ES::math::c_rad_to_deg(const T z) noexcept {
    return z * T(180) / std::numbers::pi_v<T>;
}

template <std::floating_point T> [[nodiscard]] constexpr T ES::math::c_normalize_angle_deg(T x) noexcept {
    return ES::Secret::normalize_angle_coeff(x, T{360});
}

template <std::floating_point T> [[nodiscard]] constexpr T ES::math::c_normalize_angle_rad(T x) noexcept {
    return Secret::normalize_angle_coeff(x, T{std::numbers::pi_v<T> * 2});
}

template <ES::radian_or_degree Unit, typename V>
class ES::Angle {
public:
    using value_type = V;
    using unit_type = Unit;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type *;
private:
    V angle_;
public:
    constexpr Angle() noexcept = default;
    constexpr Angle(V v) noexcept : angle_(v) {}

    [[nodiscard]] constexpr auto&& get(this auto&& me) noexcept {
        return std::forward_like<decltype(me)>(me.angle_);
    }

    template<typename oV>
    constexpr operator Angle<Unit, oV>() const noexcept {
        return Angle<Unit,oV>{angle_};
    }

    [[nodiscard]] constexpr Angle<Unit,V> wrap_to(V lower, V upper) const noexcept{
        Angle temp(*this);
        return temp.wrap_to_in_place(lower, upper);
    }
    
    constexpr Angle<Unit,V>& wrap_to_in_place(V lower, V upper) noexcept {
        V range = upper - lower;
        angle_ = std::fmod(angle_ - lower, range);
        if (angle_ < V(0)) angle_ += range;
        angle_ += lower;
        return *this;
    }


    template<typename oV>
    constexpr operator Angle<in_degrees, oV>() const noexcept requires (not std::is_same_v<Unit, in_degrees>){
        return Angle<in_degrees, oV>(math::c_rad_to_deg(angle_));
    }

    template<typename oV>
    constexpr operator Angle<in_radians, oV>() const noexcept requires (not std::is_same_v<Unit, in_radians>){
        return Angle<in_radians, oV>(math::c_deg_to_rad(angle_));
    }

    [[nodiscard]] constexpr Angle operator+(Angle rhs) const { return Angle{angle_ + rhs.angle_}; }
    [[nodiscard]] constexpr Angle operator-(Angle rhs) const { return Angle{angle_ - rhs.angle_}; }
    [[nodiscard]] constexpr Angle operator*(V scalar) const { return Angle{angle_ * scalar}; }
    [[nodiscard]] constexpr Angle operator/(V scalar) const { return Angle{angle_ / scalar}; }
    [[nodiscard]] constexpr Angle operator/(Angle rhs) const { return Angle{angle_ / rhs.angle_}; }

    [[nodiscard]] constexpr Angle normalize() const { 
        return std::is_same_v<Unit, in_radians> ? Angle{math::c_normalize_angle_rad(angle_)} : Angle{math::c_normalize_angle_deg(angle_)}; 
    }

    constexpr Angle& operator+=(Angle rhs) { angle_ += rhs.angle_; return *this; }
    constexpr Angle& operator-=(Angle rhs) { angle_ -= rhs.angle_; return *this; }
    constexpr Angle& operator*=(V scalar) { angle_ *= scalar; return *this; }
    constexpr Angle& operator/=(V scalar) { angle_ /= scalar; return *this; }
    constexpr Angle& operator/=(Angle rhs) const { angle_ /= rhs.angle_; return *this; }

    [[nodiscard]] constexpr Angle& normalize_in_place() { 
        angle_ = std::is_same_v<Unit, in_radians> ? math::c_normalize_angle_rad(angle_) : math::c_normalize_angle_deg(angle_); 
        return *this; 
    }

    template<typename oUnit, typename oV>
    [[nodiscard]] constexpr auto operator<=>(Angle<oUnit, oV> const& rhs) const  {
        return angle_ <=> static_cast<Angle<Unit, oV>>(rhs).angle_;
    }

    template<typename oUnit, typename oV>
    [[nodiscard]] constexpr auto operator==(Angle<oUnit, oV> const& rhs) const  {
        return angle_ == static_cast<Angle<Unit, oV>>(rhs).angle_;
    }
};

namespace ES::math::angle_literals {
    constexpr Angle<in_radians, long double> operator""_radl(long double z) {
        return Angle<Secret::radian, long double>{z};
    }

    constexpr Angle<in_degrees, long double> operator""_degl(long double z) {
        return Angle<Secret::degree, long double>{z};
    }

    constexpr Angle<in_radians, long double> operator""_radl(unsigned long long z) {
        return Angle<Secret::radian, long double>{static_cast<long double>(z)};
    }

    constexpr Angle<in_degrees, long double> operator""_degl(unsigned long long z) {
        return Angle<Secret::degree, long double>{static_cast<long double>(z)};
    }

    constexpr Angle<in_radians> operator""_radf(long double z) {
        return Angle<Secret::radian>{static_cast<float>(z)};
    }

    constexpr Angle<in_degrees> operator""_deg(long double z) {
        return Angle<Secret::degree>{static_cast<float>(z)};
    }

    constexpr Angle<in_radians> operator""_rad(unsigned long long z) {
        return Angle<Secret::radian>{static_cast<float>(z)};
    }

    constexpr Angle<in_degrees> operator""_deg(unsigned long long z) {
        return Angle<Secret::degree>{static_cast<float>(z)};
    }
}

template <typename T> [[nodiscard]] constexpr T ES::Secret::normalize_angle_coeff(T x, const T COTERM) noexcept {
    x = math::modulo(x, COTERM);
    if (x < T(0)) {
        x += COTERM;
    }
    return x;
}

#endif //COMPUTERGRAPHICS_ES_ANGLE_HPP
