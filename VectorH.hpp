#pragma once

#include "VectorN.hpp"
#include <cmath>
#include <algorithm>
#include <cassert>


namespace ES{

struct Type {
    static constexpr float DIRECTION = 0.0f;
    static constexpr float POINT = 1.0f;
};

class VectorH {
    VectorN<float,3> xyz_;
    float w_ = Type::POINT;

public:

    constexpr VectorH() noexcept = default;

    constexpr VectorH(float X, float Y, float Z, float W) noexcept : xyz_(X, Y, Z), w_(W) {}

    constexpr VectorH(const VectorN<float,3>& vec3, float W) noexcept : xyz_(vec3), w_(W) {}

    [[nodiscard]] constexpr auto&& x(this auto&& self) noexcept{
        return std::forward_like<decltype(self)>(self.xyz_[0]);
    }

    [[nodiscard]] constexpr auto&& y(this auto&& self) noexcept{
        return std::forward_like<decltype(self)>(self.xyz_[1]);
    }

    [[nodiscard]] constexpr auto&& z(this auto&& self) noexcept {
        return std::forward_like<decltype(self)>(self.xyz_[2]);
    }

    [[nodiscard]] constexpr auto&& w(this auto&& self) noexcept{
        return std::forward_like<decltype(self)>(self.w_);
    }


    [[nodiscard]] constexpr VectorH operator+(const VectorH& rhs) const noexcept {
        assert(w_ !=0.0f && rhs.w_ !=0.0f && "Cannot add point to point");
        return VectorH(xyz_ + rhs.xyz_, rhs.w_+w_);
    }

    constexpr VectorH& operator+=(const VectorH& rhs) noexcept {
        assert(w_ !=0.0f && rhs.w_ !=0.0f && "Cannot add point to point");
        xyz_ += rhs.xyz_;
        return *this;
    }

    [[nodiscard]] constexpr VectorH operator-(const VectorH& rhs) const noexcept {
        assert(w_ ==0.0f && rhs.w_!=0.0f && "Cannot subtract point from direction");
        return VectorH(xyz_ - rhs.xyz_, w_);
    }

    constexpr VectorH& operator-=(const VectorH& rhs) noexcept {
        assert(w_ ==0.0f && rhs.w_!=0.0f&& "Cannot subtract point from direction");
        xyz_ -= rhs.xyz_;
        return *this;
    }

    [[nodiscard]] constexpr VectorH operator*(float scalar) const noexcept {
        return VectorH(xyz_ * scalar, w_*scalar);
    }

    constexpr VectorH& operator*=(float scalar) noexcept {
        xyz_ *= scalar;
        w_ *= scalar;
        return *this;
    }

    [[nodiscard]] constexpr VectorH operator/(float scalar) const noexcept {
        assert(scalar != 0.0f);
        if(scalar == 0.0f){
            return VectorH(0.0f,0.0f,0.0f,0.0f);
        }
        return VectorH(xyz_ / scalar,w_/scalar);
    }

    constexpr VectorH& operator/=(float scalar) noexcept {
        assert(scalar != 0.0f);
        if(scalar == 0.0f){
            *this = {0,0,0,0};
        }
        xyz_ /= scalar;
        w_ /= scalar;
        return *this;
    }

    
    [[nodiscard]] constexpr VectorH hadamardProduct(const VectorH& rhs) const noexcept {
        assert((w_ == 0 && rhs.w_ == 0 || w_ !=0 && rhs.w_ !=0)  && "Hadamard product requires matching w");
        return VectorH(xyz_.hadamardProduct(rhs.xyz_), w_);
    }

    constexpr VectorH& hadamardProduct_in_place(const VectorH& rhs) noexcept {
        assert((w_ == 0 && rhs.w_ == 0 || w_ !=0 && rhs.w_ !=0)  && "Hadamard product requires matching w");
        xyz_.hadamardProduct_in_place(rhs.xyz_);
        return *this;
    }

    [[nodiscard]] constexpr VectorH hadamardDivide(const VectorH& rhs) const noexcept {
        assert((w_ == 0 && rhs.w_ == 0 || w_ !=0 && rhs.w_ !=0) && "Hadamard divide requires matching w");
        return VectorH(xyz_.hadamardDivide(rhs.xyz_), w_);
    }

    constexpr VectorH& hadamardDivide_in_place(const VectorH& rhs) noexcept {
        assert((w_ == 0 && rhs.w_ == 0 || w_ !=0 && rhs.w_ !=0) && "Hadamard divide requires matching w");
        xyz_.hadamardDivide_in_place(rhs.xyz_);
        return *this;
    }


    [[nodiscard]] constexpr float dot(const VectorH& rhs) const noexcept {
        assert((w_ ==0 && rhs.w_ == 0) && "Dot product requires two directions" );
        return xyz_.dot(rhs.xyz_);
    }

    [[nodiscard]] constexpr VectorH cross(const VectorH& rhs) const noexcept {
        assert((w_ == 0 && rhs.w_ == 0) && "Cross product requires two directions" );
        return VectorH(xyz_.cross(rhs.xyz_), 0);
    }

    constexpr VectorH& cross_in_place(const VectorH& rhs) noexcept {
        assert((w_ == 0 && rhs.w_ == 0) && "Cross product requires two directions" );
        xyz_.cross_in_place(rhs.xyz_);
        return *this;
    }


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


    [[nodiscard]] constexpr bool operator==(const VectorH& rhs) const noexcept {
        return w_ == rhs.w_ && xyz_ == rhs.xyz_;
    }

    [[nodiscard]] constexpr bool operator!=(const VectorH& rhs) const noexcept {
        return !(*this == rhs);
    }



 [[nodiscard]] constexpr VectorH homogenize() const noexcept {
    if (w_ == 0) {
        return *this;
    }
    return VectorH(xyz_/w_,1);
}

constexpr VectorH& homogenized() noexcept {
    if (w_ == 0) {
        return *this;
    }
    xyz_/= w_;
    w_ = 1;
    return *this;
}





};

}