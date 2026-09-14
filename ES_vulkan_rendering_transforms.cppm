module;


export module ES.vk.rendering_transforms;

import ES.Matrix;
import ES.Angle;


namespace ES::vk{

    [[nodiscard]] Matrix<float, 4> perspective(ES::Angle<in_radians,float> fov, float aspect_ratio, float near_plane, float far_plane){
        const auto tan_half_fov = std::tan(fov.get() /2);
        auto f_len = 1/tan_half_fov;
        Matrix<float, 4> result;
        result[0,0] = f_len/aspect_ratio;
        result[1,1] = -f_len;
        result[2,2] = far_plane/(near_plane-far_plane);
        result[2,3] = -1;
        result[3,2] = (near_plane * far_plane)/(near_plane - far_plane);
        return result;

    }

    [[nodiscard]] Matrix<float, 4> orthographic(float left, float right, float bottom, float top, float near_plane, float far_plane);

    [[nodiscard]] Matrix<float, 4> look_at(Point3<float> eye, Point3<float> target,Vector3<float> up);
    
}
