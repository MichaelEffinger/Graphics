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

    template <std::floating_point T> [[nodiscard]] constexpr T c_normalize_angle_rad(T) noexcept;
    template <std::floating_point T> [[nodiscard]] constexpr T c_normalize_angle_deg(T) noexcept;
}

template<std::floating_point T>
constexpr T ES::math::c_deg_to_rad(const T z) noexcept {
    return z * std::numbers::pi_v<T> / T(180); //TODO: decouple it from numbers pi so it works for any arbitrary type that can multiply.
}

template<std::floating_point T>
constexpr T ES::math::c_rad_to_deg(const T z) noexcept {
    return z * T(180) / std::numbers::pi_v<T>; //TODO: decouple it from numbers pi so it works for any arbitrary type that can multiply.
}

template <std::floating_point T> [[nodiscard]] constexpr T ES::math::c_normalize_angle_deg(T x) noexcept {
    return ES::Secret::normalize_angle_coeff(x, T{360}); //todo: make less hardcoded.
}

template <std::floating_point T> [[nodiscard]] constexpr T ES::math::c_normalize_angle_rad(T x) noexcept {
    return Secret::normalize_angle_coeff(x, T{std::numbers::pi_v<T> * 2}); //todo: use in-house
}

template <typename Unit, typename V> requires std::is_same_v<Unit, ES::Secret::degree> || std::is_same_v<Unit, ES::Secret::radian>
class ES::angle {
public: //fantastic usings and constants.
    //NONE OF THESE WORK WITH INTEGERS! TODO: FIX THAT!
    // static constexpr V PI = std::numbers::pi_v<V>; //TODO: use the in-house one
    // static constexpr V TAU = PI*2; ///< Objectively better than pi?
    // static constexpr V COTERMINAL{std::is_same_v<Unit, ES::Secret::degree> ? V{360}: V{TAU}};
    using value_type = V;
    using unit_type = Unit;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type *;
private:
    V angle_;
public:
    constexpr angle() noexcept = default;
    constexpr angle(V v) noexcept : angle_(v){}

    [[nodiscard]] constexpr auto&& get(this auto&& me) noexcept {
        return std::forward_like<decltype(me)>(me.angle_);
    }

    //implicit cast for angles of the same unit, but different type.
    template<typename oV>
    constexpr operator angle<Unit, oV>() const noexcept {
        return angle<Unit,oV>{angle_}; //ensures narrowing yells at you
    }

    //conversion that takes a radian type and makes it into a degree type!
    template<typename oV>
    constexpr operator angle<in_degrees, oV>() const noexcept requires (not std::is_same_v<Unit, in_degrees>){
        return angle<in_degrees, oV>(math::c_rad_to_deg(angle_));
    }

    //conversion that takes a degree type and makes it into a radian type!
    template<typename oV>
    constexpr operator angle<in_radians, oV>() const noexcept requires (not std::is_same_v<Unit, in_radians>){
        return angle<in_radians, oV>(math::c_deg_to_rad(angle_));
    }

    //funky operators
    [[nodiscard]] constexpr angle operator+(angle rhs) const { return angle{angle_ + rhs.angle_}; }
    [[nodiscard]] constexpr angle operator-(angle rhs) const { return angle{angle_ - rhs.angle_}; }
    [[nodiscard]] constexpr angle operator*(V scalar) const { return angle{angle_ * scalar}; }
    [[nodiscard]] constexpr angle operator/(V scalar) const { return angle{angle_ / scalar}; }
    [[nodiscard]] constexpr angle operator/(angle rhs) const { return angle{angle_ / rhs.angle_}; }
    [[nodiscard]] constexpr angle normalize() const { return std::is_same_v<Unit, in_radians> ? math::c_normalize_angle_rad(angle_) : math::c_normalize_angle_deg(angle_); }
    //funky compound assignment operators
    constexpr angle& operator+=(angle rhs) { angle_ += rhs.angle_; return *this; }
    constexpr angle& operator-=(angle rhs) { angle_ -= rhs.angle_; return *this; }
    constexpr angle& operator*=(V scalar) { angle_ *= scalar; return *this; }
    constexpr angle& operator/=(V scalar) { angle_ /= scalar; return *this; }
    constexpr angle& operator/=(angle rhs) const { angle_ /= rhs.angle_; return *this; }
    [[nodiscard]] constexpr angle& normalize_in_place() { angle_ = std::is_same_v<Unit, in_radians> ? math::c_normalize_angle_rad(angle_) : math::c_normalize_angle_deg(angle_); return *this; }

    //everyone's favorite spaceship
    template<typename oUnit, typename oV>
    [[nodiscard]] constexpr auto operator<=>(angle<oUnit, oV> const& rhs) const  {
        return angle_ <=> static_cast<angle<Unit, oV>>(rhs).angle_;
    }

    template<typename oUnit, typename oV>
    [[nodiscard]] constexpr auto operator==(angle<oUnit, oV> const& rhs) const  {
        return angle_ == static_cast<angle<Unit, oV>>(rhs).angle_;
    }
};

namespace ES::math::angle_literals {
    //LONG DOUBLES
    constexpr angle<in_radians, long double> operator""_radl(long double z) {
        return angle<Secret::radian, long double>{z};
    }

    constexpr angle<in_degrees, long double> operator""_degl(long double z) {
        return angle<Secret::degree, long double>{z};
    }

    constexpr angle<in_radians, long double> operator""_radl(unsigned long long z) {
        return angle<Secret::radian, long double>{static_cast<long double>(z)};
    }

    constexpr angle<in_degrees, long double> operator""_degl(unsigned long long z) {
        return angle<Secret::degree, long double>{static_cast<long double>(z)};
    }
    //TODO: add double and make this look better. It's assumed that float is normal, so it would take the form _radd (for double).

    //the default floats
    constexpr angle<in_radians> operator""_radf(long double z) {
        return angle<Secret::radian>{static_cast<float>(z)};
    }

    constexpr angle<in_degrees> operator""_deg(long double z) {
        return angle<Secret::degree>{static_cast<float>(z)};
    }

    constexpr angle<in_radians> operator""_rad(unsigned long long z) {
        return angle<Secret::radian>{static_cast<float>(z)};
    }

    constexpr angle<in_degrees> operator""_deg(unsigned long long z) {
        return angle<Secret::degree>{static_cast<float>(z)};
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