#pragma once

#include "VectorN.hpp"
#include <cmath>
#include <algorithm>
#include <cassert>

struct Type {
    static constexpr float DIRECTION = 0.0f;
    static constexpr float POINT = 1.0f;
};

class VectorH {
    VectorN<float,3> xyz_;
    float w_ = Type::POINT;

public:

    constexpr VectorH() noexcept = default;

    constexpr VectorH(float x, float y, float z, float w) noexcept
        : xyz_(x, y, z), w_(w) {}

    constexpr VectorH(const VectorN<float,3>& vec3, float w) noexcept
        : xyz_(vec3), w_(w) {}



    [[nodiscard]] constexpr auto& x() noexcept { return xyz_.x(); }
    [[nodiscard]] constexpr auto& y() noexcept { return xyz_.y(); }
    [[nodiscard]] constexpr auto& z() noexcept { return xyz_.z(); }
    [[nodiscard]] constexpr auto& w() noexcept { return w_; }

    [[nodiscard]] constexpr const auto& x() const noexcept { return xyz_.x(); }
    [[nodiscard]] constexpr const auto& y() const noexcept { return xyz_.y(); }
    [[nodiscard]] constexpr const auto& z() const noexcept { return xyz_.z(); }
    [[nodiscard]] constexpr const auto& w() const noexcept { return w_; }


    [[nodiscard]] constexpr VectorH operator+(const VectorH& rhs) const noexcept {
        assert(w_ == rhs.w_ && "Cannot add point to direction or different w");
        return VectorH(xyz_ + rhs.xyz_, w_);
    }

    constexpr VectorH& operator+=(const VectorH& rhs) noexcept {
        assert(w_ == rhs.w_ && "Cannot add point to direction or different w");
        xyz_ += rhs.xyz_;
        return *this;
    }

    [[nodiscard]] constexpr VectorH operator-(const VectorH& rhs) const noexcept {
        assert(w_ == rhs.w_ && "Cannot subtract point from direction or mismatched w");
        return VectorH(xyz_ - rhs.xyz_, w_);
    }

    constexpr VectorH& operator-=(const VectorH& rhs) noexcept {
        assert(w_ == rhs.w_ && "Cannot subtract point from direction or mismatched w");
        xyz_ -= rhs.xyz_;
        return *this;
    }

    [[nodiscard]] constexpr VectorH operator*(float scalar) const noexcept {
        return VectorH(xyz_ * scalar, w_);
    }

    constexpr VectorH& operator*=(float scalar) noexcept {
        xyz_ *= scalar;
        return *this;
    }

    [[nodiscard]] constexpr VectorH operator/(float scalar) const noexcept {
        assert(scalar != 0.0f);
        return VectorH(xyz_ / scalar, w_);
    }

    constexpr VectorH& operator/=(float scalar) noexcept {
        assert(scalar != 0.0f);
        xyz_ /= scalar;
        return *this;
    }

    
    [[nodiscard]] constexpr VectorH hadamardProduct(const VectorH& rhs) const noexcept {
        assert(w_ == rhs.w_ && "Hadamard product requires matching w");
        return VectorH(xyz_.hadamardProduct(rhs.xyz_), w_);
    }

    constexpr VectorH& hadamardProduct_in_place(const VectorH& rhs) noexcept {
        assert(w_ == rhs.w_ && "Hadamard product requires matching w");
        xyz_.hadamardProduct_in_place(rhs.xyz_);
        return *this;
    }

    [[nodiscard]] constexpr VectorH hadamardDivide(const VectorH& rhs) const noexcept {
        assert(w_ == rhs.w_ && "Hadamard divide requires matching w");
        return VectorH(xyz_.hadamardDivide(rhs.xyz_), w_);
    }

    constexpr VectorH& hadamardDivide_in_place(const VectorH& rhs) noexcept {
        assert(w_ == rhs.w_ && "Hadamard divide requires matching w");
        xyz_.hadamardDivide_in_place(rhs.xyz_);
        return *this;
    }

    // Dot and cross
    [[nodiscard]] constexpr float dot(const VectorH& rhs) const noexcept {
        return xyz_.dot(rhs.xyz_);
    }

    [[nodiscard]] constexpr VectorH cross(const VectorH& rhs) const noexcept {
        assert(w_ == Type::DIRECTION && rhs.w_ == Type::DIRECTION);
        return VectorH(xyz_.cross(rhs.xyz_), Type::DIRECTION);
    }

    constexpr VectorH& cross_in_place(const VectorH& rhs) noexcept {
        assert(w_ == Type::DIRECTION && rhs.w_ == Type::DIRECTION);
        xyz_.cross_in_place(rhs.xyz_);
        return *this;
    }

    // Magnitude and normalization
    [[nodiscard]] constexpr float magnitude() const noexcept {
        return xyz_.magnitude();
    }

    [[nodiscard]] constexpr float magnitudeSquared() const noexcept {
        return xyz_.magnitudeSquared();
    }

    constexpr VectorH& normalize() noexcept {
        xyz_.normalize();
        return *this;
    }

    [[nodiscard]] constexpr VectorH normalized() const noexcept {
        return VectorH(xyz_.normalized(), w_);
    }

    // Equality
    [[nodiscard]] constexpr bool operator==(const VectorH& rhs) const noexcept {
        return w_ == rhs.w_ && xyz_ == rhs.xyz_;
    }

    [[nodiscard]] constexpr bool operator!=(const VectorH& rhs) const noexcept {
        return !(*this == rhs);
    }
};
