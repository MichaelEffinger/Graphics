#pragma once

#include "ArithmeticOpsMixin.hpp"
#include "VectorN.hpp"
#include "PointN.hpp"
#include <cmath>
#include <algorithm>
#include <cassert>
#include "ContainerN.hpp"
#include "PointN.hpp"


namespace ES{


template <typename T>
class VectorH : public ContainerN<VectorH<T>,T,4>, public ArithmeticOpsMixin<VectorH<T>, T, 4> {
    static const int N = 4;
    using ContainerN<VectorH,T,N>::data_;
    
    public:
    using ContainerN<VectorH,T,N>::zip_in_place;
    using ContainerN<VectorH,T,N>::zip;
    using ContainerN<VectorH,T,N>::zip_reduce;
    using ContainerN<VectorH,T,N>::begin;
    using ContainerN<VectorH,T,N>::end;
    using ContainerN<VectorH,T,N>::data;
    using ContainerN<VectorH,T,N>::cend;
    using ContainerN<VectorH,T,N>::cbegin;
    using ContainerN<VectorH,T,N>::ContainerN;

    VectorH(PointN<T,3> point, T w = T{1}){
        std::copy(point.cbegin(),point.cend(), data_.begin());
        w() = w;
    }

    VectorH(VectorN<T,3> direction, T w = T{0}){
        std::copy(direction.cbegin(),direction.cend(), data_.begin());
        w() = w;
    }
    /** @defgroup accessors Accessors
    *  @brief Convenient element accessors for VectorN (x, y, z, w).
    *  @{
    */

    /** @brief Returns the x component of the vector.*/
    [[nodiscard]] constexpr auto&& x(this auto&& self) noexcept{
        return std::forward_like<decltype(self)>(self[0]);
    }

    /** @brief Returns the y component of the vector.*/
    [[nodiscard]] constexpr auto&& y(this auto&& self) noexcept{
       return std::forward_like<decltype(self)>(self[1]);
    }

    /** @brief Returns the z component of the vector.*/
    [[nodiscard]] constexpr auto&& z(this auto&& self) noexcept {
       return std::forward_like<decltype(self)>(self[2]);
    }

    /** @brief Returns the w component of the vector.*/
    [[nodiscard]] constexpr auto&& w(this auto&& self) noexcept{
        return std::forward_like<decltype(self)>(self[3]);
    }

    /** @} */


    /** @brief Component-wise additoin */
    [[nodiscard]] constexpr VectorH operator+(const VectorH rhs) const noexcept {
        assert((!w() || !rhs.w()) && "Cannot add point to point");
        return zip(rhs,std::plus{});
    }

    /** @brief Component-wise in place addition */
    constexpr VectorH& operator+=(VectorH rhs) noexcept {
        assert((!w() || !rhs.w()) && "Cannot add point to point");
        return zip_in_place(rhs,std::plus());
    }

    /** @brief Component-wise subtraction */
    [[nodiscard]] constexpr VectorH operator-(const VectorH rhs) const noexcept {
        assert((w() || !rhs.w()) && "Cannot subtract point from direction");
        return zip(rhs,std::minus());
    }

    /** @brief Component-wise in place subtraction */
    constexpr VectorH& operator-=(const VectorH rhs) noexcept {
        assert((w() || !rhs.w())&& "Cannot subtract point from direction");
        return zip_in_place(rhs,std::minus());
    }

    /** @brief Scalar multiplication */
    [[nodiscard]] constexpr VectorH operator*(T scalar) const noexcept {
        VectorH tempVec;
        std::transform(begin(),end(),tempVec.begin(),[scalar](T in){return in * scalar;});
        return tempVec;
    }

    /** @brief Scalar in place multiplication */
    constexpr VectorH& operator*=(T scalar) noexcept {
        std::transform(begin(),end(),begin(),[scalar](T in){return in * scalar;});
        return *this;
    }

    /** @brief Scalar division */
    [[nodiscard]] constexpr VectorH operator/(T scalar) const noexcept {
        assert(scalar != T{0} && "divide by zero error in VectorH operator/");
        if(scalar == T{0}){
            return VectorH(T{0},T{0},T{0},w());
        }
        VectorH tempVec;
        std::transform(cbegin(),cend(),tempVec.begin(), [scalar](T in){return in /scalar;});
        return tempVec;
    }

    /** @brief Scalar in place division */
    constexpr VectorH& operator/=(T scalar) noexcept {
        assert(scalar != T{0} && "divide by zero error in VectorH operator/=");
        if(scalar == T{0}){
            return (*this = {T{0},T{0},T{0},w()});
        }
        std::transform(begin(),end(),begin(),[scalar](T in){return in/scalar;});
        return *this;
    }

    /** @brief Component-wise multiplication */
    constexpr VectorH hadamard_product(const VectorH rhs) const noexcept {
        assert((!w() && !rhs.w())  && "Hadamard product requires both Hvectors be directions");
        return zip(rhs,std::multiplies());
    }
    /** @brief Component-wise in place multiplication */
    constexpr VectorH& hadamard_product_in_place(const VectorH rhs) noexcept {
        assert((!w() && !rhs.w())  && "Hadamard product requires both Hvecotrs be directions");
        return zip_in_place(rhs,std::multiplies());
    }

    /** @brief Component-wise division */
    [[nodiscard]] constexpr VectorH hadamard_divide(const VectorH rhs) const noexcept {
        assert((!w() && !rhs.w()) && "Hadamard divide requires both Hvectors be directions");
        return zip(rhs, [](T a, T b) { assert(b !=0 && "Divide by zero in hadamardDivide"); return (b != 0) ? (a / b) : T{0}; });
    }

    /** @brief Component-wise in place division */
    constexpr VectorH& hadamard_divide_in_place(const VectorH rhs) noexcept {
        assert((!w() && !rhs.w()) && "Hadamard divide requires both Hvectors be directions");
        return zip_in_place(rhs, [](T a, T b) { assert(b !=0 && "Divide by zero in hadamardDivide"); return (b != 0) ? (a / b) : T{0}; });
    }


    /**
    * @brief Computes the dot product of this vector with another.
    *
    * The dot product is the sum of the component-wise multiplications:
    *
    * @param rhs The other vector to compute the dot product with.
    * @return The scalar dot product of the two vectors.
    *
    * @note Both vectors must be directions.
    */
    constexpr T dot(const VectorH rhs) const noexcept {
        assert((!w() && !rhs.w()) && "Dot product requires two directions");
        return zip_reduce(rhs, 0,[](T accum, T l, T r){return accum+(l*r);});
    }


    /**
     * @brief Computers the cross product of this vector with another
     *
     * returns a vector that is perpendicular to both input vectors
     * and its magnitude corresponds to the area of the parallelogram 
     * spanned by the two vectors
     *
     * @param rhs the other vector to compute the cross product with.
     * @return a VectorH cross product of the two Vectors
     *
     * @note Both vectors must be directions
     */
    [[nodiscard]] constexpr VectorH cross(const VectorH rhs) const noexcept {
        assert((!w() && !rhs.w()) && "Cross product requires two directions" );
        return VectorH{
        data_[1] * rhs.data_[2] - data_[2] * rhs.data_[1],
        data_[2] * rhs.data_[0] - data_[0] * rhs.data_[2],
        data_[0] * rhs.data_[1] - data_[1] * rhs.data_[0], 
        T{0}};
    }

    /**
    * @brief Computes the cross product of this vector with another in-place.
    *
    * The vector is modified to become the cross product of itself and `rhs`.
    * The resulting vector is perpendicular to both input vectors and its 
    * magnitude corresponds to the area of the parallelogram spanned by the two vectors.
    *
    * @param rhs The other vector to compute the cross product with.
    * @return Reference to this vector after modification.
    *
    * @note Both vectors must be directions
    */
    constexpr VectorH& cross_in_place(const VectorH rhs) noexcept {
        assert((!w() && !rhs.w()) && "Cross product requires two directions" );
        T tx = data_[1] * rhs.data_[2] - data_[2] * rhs.data_[1];
        T ty = data_[2] * rhs.data_[0] - data_[0] * rhs.data_[2];
        T tz = data_[0] * rhs.data_[1] - data_[1] * rhs.data_[0];
        *this = {tx,ty,tz, T{0}};
        return *this;
    }

    /**
    * @brief Computes the magnitude (length) of this vector.
    * 
    * The magnitude is a scalar representing the Euclidean length of the vector:
    *
    * @return The magnitude as a scalar of the same type as the vector's elements.
    *
    * @note This function uses `sqrt`, which can be relatively expensive. 
    *       Use `magnitude_squared()` if you only need comparisons or repeated calculations.
    * @note needs 2 directions for this function
    */
    [[nodiscard]] constexpr T magnitude() const noexcept {
        assert((!w()) && "magnitude requires a direction" );
        return std::sqrt(std::fabs(zip_reduce(*this, 0,[](T accum, T l, T r){return accum+(l*r);})));
    }

    /**
    * @brief Computes the squared magnitude of this vector.
    * 
    * The squared magnitude is the sum of the squares of the vector's components:
    *
    * @return The squared magnitude as a scalar of the same type as the vector's elements.
    *
    * @note This avoids the `sqrt` operation and is therefore cheaper to compute.
    *       Use this for comparisons or repeated calculations where the actual magnitude is not required.
    * @note needs 2 directions for this function 
    */
    [[nodiscard]] constexpr T magnitudeSquared() const noexcept {
        assert((!w()) && "Magnitude requires a direction");
        return std::fabs(zip_reduce(*this, 0,[](T accum, T l, T r){return accum+(l*r);}));
    }


    /**
    * @brief compute the distance from this vector to another Position
    * @param rhs the other position to mesure vector to
    * @return Scalar vector between the two positions
    * @note needs 2 points 
    */
    [[nodiscard]] T distance(VectorH rhs) const noexcept{
        assert((w()&& rhs.w()) && "distance requires two points");
        return std::sqrt(zip_reduce(rhs, T{0}, [](T accum, T l, T r){T d = l - r; return accum + d*d;}));
    }

    /**
    * @brief compute the distance from this vector to another Position
    * @param rhs the other position to mesure vector to
    * @return Scalar vector between the two positions
    * @note needs 2 points avoids pricey sqrt function
    */
    [[nodiscard]] T distance_squared(VectorH rhs) const noexcept{
        assert((w() && rhs.w()) && "distance requires two points");
        return zip_reduce(rhs, T{0}, [](T accum, T l, T r){T d = l - r; return accum + d*d;});
    }

    /**
    * @brief Returns a normalized (unit-length) copy of this vector.
    * 
    * The returned vector has the same direction as this vector but a magnitude of 1.
    * If the vector has zero length, a zero vector is returned.
    *
    * @return A new VectorN of the same type with unit length.
    *
    * @note This involves a division by the vector's magnitude, which may be
    *       relatively expensive. For performance-critical code, consider
    *       checking magnitude before normalizing.
    */
    [[nodiscard]]constexpr VectorH normalize() const noexcept {
        return VectorH(*this).normalize_in_place();
    }

    /**
    * @brief Normalizes this vector in-place to unit length.
    * 
    * The vector is modified to have the same direction but a magnitude of 1.
    * If the vector has zero length, all elements are set to zero.
    *
    * @return Reference to this vector after normalization.
    *
    * @note This operation involves division by the vector's magnitude, which
    *       may be relatively expensive.
    */
    constexpr VectorH& normalize_in_place() noexcept {
        T mag = magnitude();
        if(mag == 0){
            assert(false && "Divide by zero error in normalize_in_place calculation");
            std::fill(begin(),end(),T{0});
            return *this;
        }
        std::transform(begin(), end(), begin(), [mag](T in){ return in / mag; });
        return *this;
    }

    /**
     * @brief homogenizes and returns a copy of the homogenized vector
     *
     * The vector is modified to divide each of the components by the w component
     * only if the w component is non 0, if zero returns initial vector
     * 
     * @return Vector after normalization
     */
    [[nodiscard]] constexpr VectorH homogenize() const noexcept {
        if (w() == 0) {
            return *this;
        }
        return operator/(w());
    }

    /**
     * @brief homogenizes in place and returns a referenc to this vector
     *
     * The vector is modified to divide each of the components by the w component
     * only if the w component is non 0, if zero returns initial vector
     * 
     * @return referenc to vector after normalization
     */
    constexpr VectorH& homogenize_in_place() noexcept {
        if (w() == 0) {
            return *this; 
        }
        return operator/=(w());
    }

    [[nodiscard]] /*TODO: make constexpr*/ auto angle(const VectorH rhs) noexcept{
        VectorN thisH = this->homogenize();
        VectorN thatH = rhs.homogenize();
        return thisH.angle(thatH);
    }


    /**
    * @brief Performs spherical linear interpolation (SLERP) between this vector and another.
    * 
    * Interpolates smoothly along the shortest arc on the unit hypersphere between the two vectors.
    * Returns a new vector at fraction `t` along this path.
    *
    * @param rhs The target vector to interpolate towards.
    * @param t Interpolation factor in [0, 1], where 0 returns this vector and 1 returns `rhs`.
    * @return A new VectorN representing the interpolated vector.
    *
    * @note If the angle between vectors is very small (< 1e-6), this function returns this vector directly.
    *       This avoids division by near-zero and ensures numerical stability.
    * @note This function requires both vectors be directions
    */
    [[nodiscard]] VectorH slerp(const VectorH rhs, T t){
        assert(!w() && !rhs.w() && "slerp needs 2 direction vectors");
        T daught = dot(rhs);

        T theta = std::acos(daught);
        if (theta < math::default_epsilon<T>::value) {
            return *this;
        }

        T sinTheta = std::sin(theta);
        T w1 = std::sin((1 - t) * theta) / sinTheta;
        T w2 = std::sin(t * theta) / sinTheta;

        return zip(rhs, [w1, w2](T a, T b) {return std::fma(w1, a, w2 * b);});
    }

    /**
    * @brief Reflects this vector across a (presumed) unit normal vector.
    *
    * Computes the reflection of this vector across `rhs`
    *
    * @param rhs The normal vector to reflect across.
    * @return A new vector representing the reflected result.
    *
    * @note This version doesnt normalize `rhs`. If `rhs` is not a unit vector
    *       the result will be incorrect. Debug builds assert to catch this; release
    *       builds will not.
    * @warning There are **safe** overloads available that internally normalize `rhs`
    *          before computing the reflection. Use those if unsure.
    */
    [[nodiscard]] VectorH reflect(const VectorH rhs){
        assert(((!w() && !rhs.w())||(w() && rhs.w())) && "reflection must have matching hvector types");
        assert(rhs.magnitude() == 1 && "parameter vector must be a unit vector");
        T daught =  dot(rhs);
        return zip(rhs,[daught](T a, T b) {return a - 2 * daught * b;});
    }

    /**
    * @brief In-place reflection across a (presumed) unit normal vector.
    *
    * Modifies this vector to be its reflection across the given normal `rhs`.
    * Uses the same formula as `reflect()`, but avoids constructing a temporary.
    *
    * @param rhs The normal vector to reflect across. **Must already be normalized.**
    * @return A reference to this vector after modification.
    *
    * @note This version doesnt normalize `rhs`. If `rhs` is not unit-length,
    *       the result will be incorrect. Debug builds assert; release builds will not.
    * @warning If you are not absolutely certain `rhs` is normalized, use the
    *          corresponding safe normalization-enforcing reflect function.
    */
    VectorH& reflect_in_place(const VectorH rhs){
        assert(((!w() && !rhs.w())||(w() && rhs.w())) && "reflection must have matching hvector types");
        assert(rhs.magnitude() == 1 && "parameter vector must be a unit vector");
        T daught =  dot(rhs);
        return zip_in_place(rhs,[daught](T a, T b) {return a - 2 * daught * b;});
    }

    /**
    * @brief Reflects this vector across another vector, safely normalizing the input.
    *
    * Computes the reflection of this vector across `rhs`, ensuring correctness even
    * when `rhs` is not already unit-length. Internally normalizes `rhs` before
    * applying the reflection formula:
    *
    * @param rhs The vector to reflect across. It will be normalized automatically.
    * @return A new vector representing the reflected result.
    *
    * @note This version is safer but slightly more expensive than `reflect(rhs)`
    *       because it computes normalization.
    */ 
    [[nodiscard]] VectorH reflect_safe(const VectorH rhs){
        assert(((!w() && !rhs.w())||(w() && rhs.w())) && "reflection must have matching hvector types");
        VectorH unitVector = rhs.normalize();
        T  daught = dot(unitVector);
        return zip(unitVector,[daught](T a, T b){return a-2*daught * b;});
    }

    /**
    * @brief In-place reflection across another vector, safely normalizing the input.
    *
    * Modifies this vector to be its reflection across `rhs`. If `rhs` is not already
    * normalized, it will be normalized internally, guaranteeing numerical correctness.
    *
    * @param rhs The vector to reflect across. It will be normalized automatically.
    * @return A reference to this vector after modification.
    *
    * @note This version avoids temporary allocation but still pays the cost of
    *       normalizing `rhs`. Use this when correctness matters but allocations do not.
    */    
    VectorH& reflect_in_place_safe(const VectorH rhs){
        assert(((!w() && !rhs.w())||(w() && rhs.w())) && "reflection must have matching hvector types");
        VectorH unitVector = rhs.normalize();
        T daught = dot(unitVector);
        return zip_in_place(unitVector,[daught](T a, T b){return a-2 * daught *b;});
    }

    /**
    * @brief Linearly interpolates between this vector and another.
    *
    * Computes a point between the two vectors based on the interpolation
    * parameter `t`, where:
    *  - `t = 0` returns this vector
    *  - `t = 1` returns `rhs`
    *  - values in between produce a proportional blend.
    *
    * @param rhs The vector to interpolate toward.
    * @param t Interpolation amount in the range [0, 1].
    * @return A new vector representing the interpolated result.
    *
    * @note This is a component-wise operation.
    */
    constexpr VectorH lerp(VectorH rhs, T t) const noexcept{
        assert(((!w() && !rhs.w())||(w() && rhs.w())) && "lerp must have matching hvector types");
        return zip(rhs,[t](T a, T b) {return a+(b-a)*t;});
    }

    /**
    * @brief In-place linear interpolation between this vector and another.
    *
    * Modifies the current vector to be the interpolated result. Behaves
    * the same as `lerp()`, but without creating a temporary result.
    *
    * @param rhs The vector to interpolate toward.
    * @param t Interpolation amount in the range [0, 1].
    * @return A reference to this vector after modification.
    *
    * @note Use this when avoiding temporaries matters.
    */
    constexpr VectorH& lerp_in_place(VectorH rhs, T t) noexcept {
        assert(((!w() && !rhs.w())||(w() && rhs.w())) && "lerp must have matching hvector types");
        return zip_in_place(rhs, [t](T a, T b) { return a + (b - a) * t;});
    }   

    /**
     * @brief Clamps each value into range [minVal, maxVal]
     * 
     * @param minVal the lower range(lowest value that any vector value can be)
     * @param maxVal the upper range(highest value that any vector value can be)
     * @return a new vector with clamped values from the original vector
     */    
    [[nodiscard]] constexpr VectorH clamp(T minVal, T maxVal) noexcept{
        VectorH tempVec;
        std::transform(begin(),end(), tempVec.begin(),[minVal,maxVal](T in){return std::clamp(in, minVal, maxVal);});
        return tempVec;
    }
    
    /**
     * @brief Clams each value into range [minVal, maxVal] in-place
     * 
     * @param minVal the lower range(lowest value that any vector value can be)
     * @param maxVal the upper range(highest value that any vector value can be)
     * @return a reference to the modified value clamped in range
     */
    constexpr VectorH& clamp_in_place(T minVal, T maxVal) noexcept {
        std::transform(begin(), end(), begin(),[minVal, maxVal](T in) { return std::clamp(in, minVal, maxVal);});
        return *this;
    }


    /**
    * @brief Computes the refraction of this vector across a surface normal.
    *
    * This version assumes both this vector and `rhs` (the normal) are already normalized.
    * It applies Snell's law to determine the direction change when transitioning
    * between two media with refractive indices `n1` and `n2`.
    *
    * If total internal reflection occurs (no real refracted solution), this returns a zero vector.
    *
    * @param rhs The surface normal (must be unit-length).
    * @param n1  Refractive index of the medium the vector is currently in.
    * @param n2  Refractive index of the medium the vector is entering.
    * @return A refracted vector, or a zero vector if total internal reflection occurs.
    *
    * @warning This version doesnt normalize `rhs`. If `rhs` is not unit length,
    *          the result will be incorrect. See `refract_safe()` for an automatically normalized version.
    */
    [[nodiscard]] constexpr VectorH refract(VectorH rhs, T n1, T n2) const noexcept {
        assert(!w() && !rhs.w() && "refract requires two direction vectors");
        T refractionRatio = n1 / n2;
        T cosi = -(dot(rhs));
        T k = T{1} - refractionRatio * refractionRatio * (T{1} - cosi * cosi);
        if (k < T{0}){
            VectorH tempVec;
            std::fill(tempVec.begin(),tempVec.end(),T{0});
            return tempVec;
        }
        T sqrtK = std::sqrt(k);

        return zip(rhs, [refractionRatio, cosi, sqrtK](T i, T n) {return i * refractionRatio + n * (refractionRatio * cosi - sqrtK);});
    }

    /**
    * @brief In-place refraction across a (presumed) unit normal vector.
    *
    * Modifies this vector to be its refracted form when transitioning between
    * media with indices `n1` and `n2`. Uses Snell's law directly.
    *
    * If total internal reflection occurs, the vector becomes the zero vector.
    *
    * @param rhs The surface normal (must be unit-length).
    * @param n1  Refractive index of the medium the vector is currently in.
    * @param n2  Refractive index of the medium the vector is entering.
    * @return A reference to this vector after modification.
    *
    * @warning This version doesnt normalize `rhs`. If you are not sure `rhs` is
    *          normalized, use `refract_in_place_safe()` instead.
    */
    constexpr VectorH& refract_in_place(VectorH rhs, T n1, T n2) noexcept {
        assert(!w() && !rhs.w() && "refract requires two direction vectors");
        T refractionRatio = n1 / n2;
        T cosi = -(dot(rhs));
        T k = T{1} - refractionRatio * refractionRatio * (T{1}- cosi * cosi);
        if (k < T{0}){
            std::fill(begin(),end(),T{0});
            return *this;
        }       
        T sqrtK = std::sqrt(k);
        return zip_in_place(rhs, [refractionRatio, cosi, sqrtK](T i, T n) {return i * refractionRatio + n * (refractionRatio * cosi - sqrtK);});
    }

    /**
    * @brief Computes the refraction of this vector across a surface normal, safely.
    *
    * This version automatically normalizes both the input vector and the normal,
    * guaranteeing correctness even when the caller does not provide normalized inputs.
    *
    * If total internal reflection occurs, this return a zero vector.
    *
    * @param rhs The surface normal (normalization enforced internally).
    * @param n1  Refractive index of the medium the vector is currently in.
    * @param n2  Refractive index of the medium the vector is entering.
    * @return A refracted VectorN, or a zero vector if no valid refracted direction exists.
    */
    [[nodiscard]] constexpr VectorH refract_safe(VectorH rhs, T n1, T n2) const noexcept {
        assert(!w() && !rhs.w() && "refract requires two direction vectors");  
        VectorH thisUnit = normalize();
        VectorH rhsUnit = rhs.normalize();

        T refractionRatio = n1 / n2;
        T cosi = -(thisUnit.dot(rhsUnit));
        T k = T{1} - refractionRatio * refractionRatio * (T{1} - cosi * cosi);

        if (k < T{0}){
            VectorH tempVec;
            std::fill(tempVec.begin(),tempVec.end(),T{0});
            return tempVec;
        }

        T sqrtK = std::sqrt(k);
        return thisUnit.zip(rhsUnit, [refractionRatio, cosi, sqrtK](T i, T n) {return i * refractionRatio + n * (refractionRatio * cosi - sqrtK);});
    }


    /**
    * @brief In-place safe refraction across a surface normal.
    *
    * Normalizes both this vector and the normal before applying Snell's law.
    * Ensures correct behavior even when the caller provides non-unit inputs.
    *
    * If total interna reflection occurs, the vector becomes the zero vector.
    *
    * @param rhs The surface normal (normalized internally).
    * @param n1  Refractive index of the medium the vector is currently in.
    * @param n2  Refractive index of the medium the vector is entering.
    * @return A reference to this vector after modification.
    */
    constexpr VectorH& refract_in_place_safe(VectorH rhs, T n1, T n2)noexcept {
        assert(!w() && !rhs.w() && "refract requires two direction vectors");
        *this = normalize();
        VectorH rhsUnit = rhs.normalize();

        T refractionRatio = n1 / n2;
        T cosi = -(dot(rhsUnit));
        T k = T{1} - refractionRatio * refractionRatio * (T{1} - cosi * cosi);

        if (k < T{0}){
            std::fill(begin(),end(),T{0});
            return *this;
        }
        T sqrtK = std::sqrt(k);
        return zip_in_place(rhsUnit, [refractionRatio, cosi, sqrtK](T i, T n) {return i * refractionRatio + n * (refractionRatio * cosi - sqrtK);});
    }

    /**
    * @brief Projects this vector onto another vector.
    * 
    * Returns a new vector representing the projection of this vector onto `rhs`.
    * The projection is calculated as `(dot(*this, rhs) / dot(rhs, rhs)) * rhs`.
    *
    * @param rhs The vector to project onto.
    * @return A new VectorN that is the projection of this vector onto `rhs`.
    *
    * @note Asserts in debug if `rhs` is a zero vector. Release builds may produce undefined behavior.
    */
    [[nodiscard]] VectorH project_onto(VectorH rhs) const noexcept{
        assert(rhs.dot(rhs) !=0  && "Divide by zero error in Project onto method");
        assert(!w() && !rhs.w() && "Project_onto requires two direction vectors");
        return dot(rhs)/rhs.dot(rhs) * rhs;
    }

    /**
    * @brief In-place projection of this vector onto another vector.
    * 
    * Modifies this vector to be its projection onto `rhs` using the formula
    * `(dot(*this, rhs) / dot(rhs, rhs)) * rhs`.
    *
    * @param rhs The vector to project onto.
    * @return A reference to this vector after modification.
    *
    * @note Asserts in debug if `rhs` is a zero vector. Release builds may produce undefined behavior.
    */
    VectorH project_onto_in_place(VectorH rhs)noexcept{
        assert(rhs.dot(rhs)!=0 && "Divide by zero error in Project onto in place method");
        assert(!w() && !rhs.w() && "Project_onto requires two direction vectors");
        *this = dot(rhs)/rhs.dot(rhs) *rhs;
        return *this;
    }

    /**
    * @brief Scalar multiplication with the scalar on the left.
    *
    * Allows expressions like `2 * vec` by multiplying each component of `lhs` by `scalar`.
    *
    * @note This is a friend function to allow access to private members of VectorN.
    */
    [[nodiscard]] friend constexpr VectorH operator*(const auto scalar, const VectorH lhs) noexcept{
        VectorH tempVec;
        std::transform(lhs.cbegin(),lhs.cend(),tempVec.begin(),[scalar](T in){return in * scalar;});
        return tempVec;
    }

};

}