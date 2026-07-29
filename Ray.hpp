#include "PointN.hpp"
#include "VectorN.hpp"
#include "Matrix.hpp"
namespace ES{

    
    template <typename T>
    class Ray{
        
        public:

        PointN<T, 3> origin;
        VectorN<T,3> direction;
   
        constexpr Ray() = default;

        constexpr Ray(PointN<T,3> o, VectorN<T,3> d) noexcept : origin{o}, direction{d}{}


        [[nodiscard]] constexpr PointN<T,3> at(T t) const noexcept {
            return origin + direction * t;
        }

        [[nodiscard]] constexpr Ray normalize()const noexcept{
            return Ray(origin,direction.normalize());
        }

        constexpr Ray& normalize_in_place() noexcept{
            direction.normalize_in_place();
            return *this;
        }

        [[nodiscard]] constexpr bool operator==(Ray rhs)const noexcept{
            return (origin == rhs.origin && direction == rhs.direction);
        }

        [[nodiscard]] constexpr bool almost_equal(Ray rhs)const noexcept{
            return (origin.almost_equal(rhs.origin) && direction.almost_equal(rhs.direction));
        }

        [[nodiscard]] constexpr Ray transform(const Matrix<T,4>& matrix) const noexcept {
            return Ray(matrix * origin, matrix * direction);
        }

        constexpr Ray& transform_in_place(const Matrix<T,4>& matrix) noexcept{
            origin = matrix * origin;
            direction = matrix * direction;
            return *this;
        }

        [[nodiscard]] constexpr Ray translate(const VectorN<T,3> offset) const noexcept{
            return Ray(origin + offset, direction);
        }

        constexpr Ray& translate_in_place(const VectorN<T,3> offset) noexcept{
            origin+=offset;
            return *this;
        }

        [[nodiscard]] constexpr Ray scale(T scalar) const noexcept {
            return Ray(origin, direction * scalar);
        }

        constexpr Ray& scale_in_place(T scalar) noexcept {
            direction *= scalar;
            return *this;
        }

        [[nodiscard]] constexpr bool isValid() const noexcept {
            return direction.length() > T{0};
        }

        
      //-------------------------------------------------------Add Rotation Here----------------------------------------------------\\







    };
}