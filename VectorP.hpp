#pragma once

#include "VectorN.hpp"
#include "VectorH.hpp"
#include <memory>

// GPU-safe padded vec3 (16-byte aligned)



struct alignas(8) Vector2P: public VectorN<float,2>{};


struct alignas(16) Vector3P : public VectorN<float, 3> {
    float pad_ = 1.0f;

    constexpr Vector3P() = default;

    constexpr Vector3P(float x, float y, float z) : VectorN<float, 3>({x, y, z}), pad_(1.0f) {}


    constexpr Vector3P(VectorN<float,3>);

    VectorH toVectorH(float W) const noexcept {
        VectorH h = *reinterpret_cast<const VectorH*>(this);
        h.w() = W;
        return h;
    }

    VectorH toVectorH_In_Place(float W) const noexcept {
        VectorH h = *reinterpret_cast<const VectorH*>(this);
        h.w() = W;
        return h;
    }

    VectorH& toVectorH_In_Place() noexcept {
        return *reinterpret_cast<VectorH*>(this);
    }



};
struct alignas(16) Vector4P: public VectorN<float,4>{};

