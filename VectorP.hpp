#pragma once

#include "VectorN.hpp"
#include "VectorH.hpp"

#include <cstring>
#include "config.hpp"




// at somepoint I am gonna refactor these to be a built in part of vectorN, now currentyl I am not sure how to do this or if this is possible, 
// but I am going to try to get the padding built into the vector3 and I am also going to try to get the alignas into the vec3, this is something
// that might be extermely hard but well, see,these might stick around too if this is deemed not possible by me
// okay now at this point, dont use this, we are just gonna kill this class and have the graphics mananger handle adding padding when the vector
// gets passed to chew on the vect3 

namespace ES{

struct alignas(sizeof(real)*2) Vector2P: public ES::VectorN<float,2>{};


struct alignas(sizeof(real)*4) Vector3P : public ES::VectorN<float, 3> {
    float pad_ = 1.0f;

    [[deprecated]] constexpr Vector3P() = default;

    [[deprecated]] constexpr Vector3P(float x, float y, float z) : VectorN<float, 3>({x, y, z}), pad_(1.0f) {}

    [[deprecated]] constexpr Vector3P(VectorN<float,3>);

    [[deprecated]] VectorH<float> toVectorH(float W) const noexcept {
        std::memcpy(&h, this, sizeof(Vector3P));
        h.w() = W;
        return h;
    }


};
struct alignas(sizeof(real)*4) Vector4P: public ES::VectorN<float,4>{};

}