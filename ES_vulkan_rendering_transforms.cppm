module;

#include <cmath>

export module ES.vk.rendering_transforms;

import ES.Angle;
import ES.VectorN;
import ES.PointN;
import ES.Matrix;


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

    [[nodiscard]] Matrix<float, 4> orthographic(float left, float right, float top, float bottom, float near_plane, float far_plane){
        Matrix<float,4> result;
        result[0,0] = 2/(right-left);
        result[1,1] = 2/(top - bottom);
        result[2,2] = 1/(far_plane-near_plane);
        result[3,0] = -(right + left)/(right - left);
        result[3,1] = -(top + bottom)/(top-bottom);
        result[3,2] = -near_plane/(far_plane-near_plane);
        result[3,3] = 1;

        return result;

    }

    [[nodiscard]] Matrix<float, 4> look_at(Point3<float> eye, Point3<float> target, Vector3<float> up) {
        const Vector3<float> z_axis = (eye - target).normalize();
        const Vector3<float> x_axis = up.cross(z_axis).normalize();
        const Vector3<float> y_axis = z_axis.cross(x_axis);

        return Matrix<float, 4>::from_rows(
            Vector4<float>{x_axis, -x_axis.dot(eye.to_vector()) },
            Vector4<float>{y_axis, -y_axis.dot(eye.to_vector()) },
            Vector4<float>{z_axis, -z_axis.dot(eye.to_vector()) },
            Vector4<float>{ 0.0f,0.0f,0.0f,1.0f}
        );
    }
    
}
