#pragma once

#include "VectorN.hpp"
#include "VectorH.hpp"

#include <cstring>
#include "config.hpp"


namespace ES{

struct alignas(sizeof(real)*2) Vector2P: public ES::VectorN<float,2>{};


struct alignas(sizeof(real)*4) Vector3P : public ES::VectorN<float, 3> {
    float pad_ = 1.0f;

    constexpr Vector3P() = default;

    constexpr Vector3P(float x, float y, float z) : VectorN<float, 3>({x, y, z}), pad_(1.0f) {}

    constexpr Vector3P(VectorN<float,3>);

    VectorH toVectorH(float W) const noexcept {
        VectorH h;
        std::memcpy(&h, this, sizeof(Vector3P));
        h.w() = W;
        return h;
    }


};
struct alignas(sizeof(real)*4) Vector4P: public ES::VectorN<float,4>{};

}