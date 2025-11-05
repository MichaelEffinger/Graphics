// Ryan Seavey
// Michael Effinger

#pragma once

#include <type_traits>
#include <array>
#include <cassert>
#include <cmath>
#include <algorithm>
#include <functional>
#include "ES_math.hpp"
#include "ES_concepts.hpp"

namespace ES {

/**
 * @brief Fixed-size N-dimensional mathematical vector.
 *
 * Provides component-wise arithmetic, geometric operations (dot, cross, magnitude,
 * normalization), interpolation (lerp, slerp), projection and reflection utilities,
 * and element-wise operations (Hadamard product/division). Supports construction
 * from N parameters, or N-J sized Vector + J extra parameters.
 *
 * This type is intended for numeric computation and enforces that `T` is
 * arithmetic. Dimension `N` is known at compile time and determines supported
 * coordinate accessors (`x()`, `y()`, `z()`, `w()` when available).
 *
 * @note Supports convenience aliases i.e. `Vector3<float>` `Vector2<double>`
 *
 * @tparam T  Arithmetic scalar type used for storage (e.g. int, float, double).
 * @tparam N  Number of elements in the vector (must be > 0).
 */
 
template <typename T, std::size_t N> requires std::is_arithmetic_v<T>
class VectorN{
protected:
    std::array<T, N> data_;
public:
    using VectorThis = VectorN<T, N>;
    using value_type = T;
    using size_type = std::size_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type *;
    using iterator = std::array<T,N>::iterator;
    using const_iterator = std::array<T,N>::const_iterator;
    using reverse_iterator = std::array<T,N>::reverse_iterator;
    using const_reverse_iterator = std::array<T,N>::const_reverse_iterator;


    constexpr VectorN() noexcept = default;


    /**
    * @brief Constructs a VectorN from N individual values.
    *
    * This is a variadic template constructor that takes exactly N arguments.
    * Each argument is converted to the vector's element type T using static_cast.
    * VectorN can be constructed using either `()` or `{}` initialization syntax.
    *
    * @tparam Args Parameter pack representing each element of the vector.
    *         Must have exactly N arguments.
    * @param args The values to initialize the vector elements.
    *
    * @note This constructor is `constexpr` and `noexcept` if all element
    *       constructions are noexcept.
    *
    *  @example
    * VectorN<float, 3> v(1.0f, 2.0f, 3.0f);  // creates a 3D vector
    */
    template <typename... Args> requires (sizeof...(Args) == N)
    constexpr VectorN(Args&&... args) noexcept((std::is_nothrow_constructible_v<T, Args&&> && ...)) : data_{static_cast<T>(Args(args))...} {}

    
    /**
     * @brief Construct a VectorN form an N-J sized vector along with J other parameters
     * 
     * This is a variadic template contsructor that takes exactly one smaller Vector of
     * size N-J along with J other parameters. These together must be of size exactly N.
     * The parameter arguments are static converted to to T.
     *
     * @tparam M The size of vectorN, which is the first parameter  
     * @tparam U Parameeter pack representing each of the remaining elements. 
     *         Must have exactly size M-N
     * @param smaller The vector of the first values to initialize in the vector
     * @param extras The remaining values to initialize in the vector
     *
     * @note This constructor is `constexpr` and `noexcept` if all elements
     *       if all element construction are noexcept.
     *
     * @example 
     * VectorN<float,2> v2(1.0f,2.0f);
     * VectorN<float,5> v5(v2,3.0f,4.0f,5.0f);
     */
    template<size_t M, typename... U> requires (sizeof...(U) == N - M)
    constexpr VectorN(const VectorN<T, M>& smaller, U... extras) noexcept((std::is_nothrow_constructible_v<T,U&&>&&...)): data_{} {      
        std::copy(smaller.cbegin(), smaller.cend(), data_.begin());
        size_t index = M;
        ((data_[index++] = T(static_cast<T>(extras))), ...);
    }

    /**
    * @brief Provides unchecked element access by index.
    *
    * Uses forwarding to preserve value category (lvalue/rvalue) of the caller.
    * Asserts in debug builds if `index >= N`.
    *
    * @param index Index of the element to access (0-based).
    * @return Reference to the element at the given index, preserving constness/value category.
    */ 
    [[nodiscard]] constexpr auto&& operator[](this auto&& self, std::size_t index)noexcept{
        assert(index < N && "Vector index out of range");
        return std::forward_like<decltype(self)>(self.data_[index]);
    }

    /**
    * @brief Provides clamped element access by index.
    *
    * Uses forwarding to preserve value category (lvalue/rvalue) of the caller.
    * Asserts in debug builds if `index >= N`.
    *
    * @param index Index of the element to access (0-based).
    * @return Reference to the element at the given index, preserving constness/value category.
    */ 
    [[nodiscard]] constexpr auto&& at(this auto&& self, std::size_t index)noexcept{
        assert(index < N && "Vector index out of range");
        if(index >=N){
            index = N-1;
        }
        return std::forward_like<decltype(self)>(self.data_[index]);
    }

    /**
    * @brief Returns the number of elements in the vector.
    * @return Number of elements (compile-time constant N).
    */
    [[nodiscard]] static constexpr std::size_t size()noexcept{
        return N;
    }

    /** @defgroup accessors Accessors
    *  @brief Convenient element accessors for VectorN (x, y, z, w, head, tail).
    *  @{
    */

    /**
    * @brief Returns the x component of the vector.
    * @note Vector must be smaller than 5
    * @return Reference to the first element.
    */
    [[nodiscard]] constexpr auto&& x(this auto&& self) noexcept requires (N>0 && N<5) {
        return std::forward_like<decltype(self)>(self[0]);
    }

    /**
    * @brief Returns the y component of the vector.
    * @note Vector must be smaller than 5, and bigger than 1
    * @return Reference to the second element.
    */
    [[nodiscard]] constexpr auto&& y(this auto&& self) noexcept requires (N>1 && N<5) {
        return std::forward_like<decltype(self)>(self[1]);
    }

    /**
    * @brief Returns the z component of the vector.
    * @note Vector must be smaller than 5, and bigger than 2
    * @return Reference to the third element.
    */
    [[nodiscard]] constexpr auto&& z(this auto&& self) noexcept requires (N>2 && N<5) {
        return std::forward_like<decltype(self)>(self[2]);
    }

    /**
    * @brief Returns the w component of the vector.
    * @note Vector must have size exactly 4
    * @return Reference to the fourth element.
    */
    [[nodiscard]] constexpr auto&& w(this auto&& self) noexcept requires (N==4) {
        return std::forward_like<decltype(self)>(self[3]);
    }

    /**
    * @brief Returns the first element (head) of the vector.
    * @return Reference to the first element.
    */
    [[nodiscard]] constexpr auto&& head(this auto&& self) noexcept requires (N > 0) {
        return std::forward_like<decltype(self)>(self[0]);
    }

    /**
    * @brief Returns the last element (tail) of the vector.
    * @return Reference to the last element.
    */
    [[nodiscard]] constexpr auto&& tail(this auto&& self) noexcept requires (N > 0) {
        return std::forward_like<decltype(self)>(self[N-1]);
    }

    /** @} */


    /** @defgroup iterators Iterator Access
    *  @brief Begin/end iterator access for VectorN.
    *  @{
    */

    /** @brief Returns iterator to first element. See `cbegin` for const iterators. */
    [[nodiscard]] constexpr auto begin() noexcept { return data_.begin(); }
    /** @overload */
    [[nodiscard]] constexpr auto begin() const noexcept { return data_.begin(); }
    /** @overload */
    [[nodiscard]] constexpr auto cbegin() const noexcept { return data_.cbegin(); }

    /** @brief Returns iterator past the last element. See `cend` for const iterators. */
    [[nodiscard]] constexpr auto end() noexcept { return data_.end(); }
    /** @overload */
    [[nodiscard]] constexpr auto end() const noexcept { return data_.end(); }
    /** @overload */
    [[nodiscard]] constexpr auto cend() const noexcept { return data_.cend(); }
    /** @} */


    /** @defgroup zip Element-wise Operations
    *  @brief Component-wise operations on VectorN using custom binary functions.
    *  @{
    */

    /**
    * @brief Returns a new vector by applying a binary operation component-wise.
    *
    * Applies `op` to each corresponding element of this vector and `rhs`,
    * producing a new VectorN with the results.
    *
    * @tparam BinaryOp Type of the binary operation (must be callable with `T, T`).
    * @param rhs The vector to combine with this vector.
    * @param op The binary operation to apply element-wise.
    * @return A new VectorN where each element is `op(this[i], rhs[i])`.
    */
    template<typename BinaryOp>
    [[nodiscard]] constexpr VectorN zip(VectorN rhs, BinaryOp op) const noexcept {
        VectorN resultant;
        auto liter = cbegin(), riter = rhs.cbegin();
        auto oiter = resultant.begin();
        while(liter != cend()){
            *oiter = op(*liter, *riter);
            ++liter, ++riter, ++oiter;
        }
        return resultant;
    }

    /**
    * @brief Applies a binary operation component-wise in-place.
    *
    * Modifies this vector by applying `op` to each corresponding element of
    * this vector and `rhs`.
    *
    * @tparam BinaryOp Type of the binary operation (must be callable with `T, T`).
    * @param rhs The vector to combine with this vector.
    * @param op The binary operation to apply element-wise.
    * @return Reference to this vector after modification.
    */
    template<typename BinaryOp>
    constexpr VectorN& zip_in_place(const VectorN rhs, BinaryOp op) noexcept {
        auto liter = begin();
        auto riter = rhs.cbegin();
        while(liter != end()){
            *liter = op(*liter, *riter);
            ++liter, ++riter;
        }
        return *this;
    }

    /**
    * @brief Reduces two vectors into a single value using a ternary fold expression.
    *
    * Iterates over each element of this vector and `rhs`, combining them with
    * `exp` starting from `initial`.
    *
    * @param rhs The vector to combine with this vector.
    * @param initial Initial value for the reduction.
    * @param exp A callable expression of the form `(T accum, T a, T b) -> T`.
    * @return The final accumulated value.
    */
    [[nodiscard]] constexpr T zip_reduce(const VectorN rhs, T initial, ES::concepts::FoldExpr<T> auto&& exp) const noexcept {
        auto liter = cbegin(), riter = rhs.cbegin();
        while(liter != cend()){
            initial = exp(initial, *liter, *riter);
            ++liter, ++riter;
        }
        return initial;
    }

    /** @} */




    /**
    * @brief Computes the dot product of this vector with another.
    *
    * The dot product is the sum of the component-wise multiplications:
    *
    * @param rhs The other vector to compute the dot product with.
    * @return The scalar dot product of the two vectors.
    *
    * @note Both vectors must have the same dimension `N`.
    */
    [[nodiscard]] constexpr T dot(const VectorN rhs) const noexcept{
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
     * @return a Vector3 cross product of the two Vectors
     *
     * @note Both vectors must have N exactly 3
     */
    [[nodiscard]] constexpr VectorN cross(const VectorN rhs) const noexcept requires (N == 3){
        return VectorN{
        data_[1] * rhs.data_[2] - data_[2] * rhs.data_[1],
        data_[2] * rhs.data_[0] - data_[0] * rhs.data_[2],
        data_[0] * rhs.data_[1] - data_[1] * rhs.data_[0]
        };
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
    * @note Both vectors must have N exactly 3.
    */
     constexpr VectorN& cross_in_place(const VectorN rhs) noexcept requires (N == 3){
      T xt = data_[1] * rhs.data_[2] - data_[2] * rhs.data_[1],
        yt = data_[2] * rhs.data_[0] - data_[0] * rhs.data_[2],
        zt = data_[0] * rhs.data_[1] - data_[1] * rhs.data_[0];
        x() = xt, y() = yt, z() = zt;
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
    */
    [[nodiscard]] constexpr T magnitude() const noexcept{
        //Yes dot product should always be positive but floating point erros can make tiny negative: so I check it 
        return std::sqrt(std::fabs(dot(*this)));
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
    */
    [[nodiscard]] constexpr T magnitude_squared() const noexcept{
        return dot(*this);
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
    constexpr VectorN& normalize_in_place() noexcept{
        T mag = magnitude();
        if(mag == 0){
            std::fill(begin(),end(),T{0});
            return *this;
        }
        std::transform(begin(), end(), begin(), [mag](T in){ return in / mag; });
        return *this;
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
    [[nodiscard]] constexpr VectorN normalize() const noexcept{
        return VectorN(*this).normalize_in_place();
    }


    /** @defgroup arithmetic Arithmetic Operators
    *  @brief Component-wise & scalar arithmetic operations for VectorN.
    *  @{
    */

    /** @brief Component-wise addition. */
    [[nodiscard]] constexpr VectorN operator+(const VectorN rhs)const noexcept{
        return zip(rhs,std::plus{});
    }
    
    /** @brief In-place component-wise addition */
    constexpr VectorN& operator+=(const VectorN rhs) noexcept{
        return zip_in_place(rhs, std::plus{});
    }
    /** @brief Unary negation */
    [[nodiscard]] constexpr VectorN operator-() const noexcept{
        VectorN self{*this};
        std::negate<> op;
        for(auto& i : self) i = op(i);
        return self;
    }

    /** @brief Component-wise subtraction */
    [[nodiscard]] constexpr VectorN operator-(const VectorN rhs) const noexcept{
        return zip(rhs, std::minus{});
    }

    /** @brief In-place component-wise subtraction */
    constexpr VectorN& operator-=(const VectorN rhs) noexcept{
        return zip_in_place(rhs, std::minus{});
    }

    /** @brief Scalar multiplication */
    [[nodiscard]] constexpr VectorN operator*(const T scalar) const noexcept{
        VectorN tempVec;
        std::transform(begin(),end(),tempVec.begin(),[scalar](T in){return in * scalar;});
        return tempVec;
    }
    
    /** @brief In-place scalar multiplication */
    constexpr VectorN& operator*=(const T scalar) noexcept{
        std::transform(begin(),end(),begin(),[scalar](T in){return in * scalar;});
        return *this;
    }
    
    /**
     * @brief scalar division 
     * @note Division by zero triggers an assert in debug mode,
     *       but returns a zero vector in release mode.
     */
    [[nodiscard]] constexpr VectorN operator/(const T scalar) const noexcept{
        VectorN tempVec;
        std::transform(begin(), end(), tempVec.begin(),[scalar](T in) {assert(scalar !=0 && "Divide by zero in operator/"); return (scalar != 0) ? (in / scalar) : T{0}; });
        return tempVec;
    }

    /**
     * @brief In-place component-wise division by const scaler
     * @note Division by zero triggers an assert in debug mode,
     *       but returns a zero vector in release mode.
     */
    constexpr VectorN& operator/=(const T scalar) noexcept{
        std::transform(begin(), end(),begin(),[scalar](T in) {assert(scalar !=0 && "Divide by zero in operator/="); return (scalar != 0) ? (in / scalar) : T{0}; });
        return *this;
    }
    
    /**
    * @brief Scalar multiplication with the scalar on the left.
    *
    * Allows expressions like `2 * vec` by multiplying each component of `lhs` by `scalar`.
    *
    * @note This is a friend function to allow access to private members of VectorN.
    */
    [[nodiscard]] friend constexpr VectorN operator*(const auto& scalar, const VectorN lhs) noexcept{
        VectorN tempVec;
        std::transform(lhs.begin(),lhs.end(),tempVec.begin(),[scalar](T in){return in * scalar;});
        return tempVec;
    }
    
    /** @brief Component-wise multiplication */
    [[nodiscard]] constexpr VectorN hadamard_product(const VectorN rhs) const noexcept{
        return zip(rhs, std::multiplies{});
    }
    
    /** @brief In-place component-wise multiplication */
    constexpr VectorN& hadamard_product_in_place(const VectorN rhs) noexcept{
        return zip_in_place(rhs, std::multiplies{});
    }
    
    /**
     * @brief Component-wise division
     * @note Division by zero triggers an assert in debug mode,
     *       but value becomes zero when divided in release
     */
    [[nodiscard]] constexpr VectorN hadamard_divide(const VectorN rhs) const noexcept {
        return zip(rhs, [](T a, T b) { assert(b !=0 && "Divide by zero in hadamardDivide"); return (b != 0) ? (a / b) : T{0}; });
    }
  
    /**
     * @brief In-place component-wise division
     * @note Division by zero triggers an assert in debug mode,
     *       but value becomes zero when divided in release
     */    
    constexpr VectorN& hadamard_divide_in_place(const VectorN rhs) noexcept {
        return zip_in_place(rhs, [](T a, T b) { assert(b !=0 && "Divide by zero in hadamardDivide"); return (b != 0) ? (a / b) : T{0}; });
    }


/** @} */


    /** @brief Equality operator, checks if every component of vector is equal */
    [[nodiscard]] constexpr bool operator==(const VectorN other)const noexcept{
        if(other.data_ == data_){
            return true;
        }
        return false;
    }

    /**
    * @brief Computes the angle between this vector and another.
    * 
    * Uses the dot product to calculate the angle via:
    *     acos( dot(a, b) / (|a| * |b|) )
    *
    * @note If either vector has zero magnitude, this function asserts in debug
    *       and returns zero in release mode (to prevent division-by-zero).
    *
    * @warning One of the team members is currently implementing a templated
    *          angle type that can represent angles in either radians or degrees.
    *          When that change is complete, this function will return that
    *          angle type rather than a raw floating-point value. Users should
    *          expect this return type to change in a future update.
    *
    * @param rhs The vector to compute the angle to.
    * @return The angle between the two vectors in radians (for now).
    */
    [[nodiscard]] constexpr T angle(const VectorN<T, N> rhs) const noexcept{
        T thisMag = magnitude_squared();
        T thatMag = rhs.magnitude_squared();
        
        if(thisMag == 0 || thatMag == 0) {
            assert(false && "Divide by zero error in angle calculation");
            return T{0};
        }
        return std::acos(std::clamp(dot(rhs) / std::sqrt(thisMag * thatMag), T{-1}, T{1}));  
    }

    /**
    * @brief Returns a new vector with one additional element appended.
    *
    * Creates a new VectorN with size N+1, copying the existing components
    * and inserting the given value as the final element.
    *
    * @param newElem The value to append to the end of the new vector.
    * @return A new vector of dimension N+1.
    */
    [[nodiscard]] constexpr VectorN<T,N+1>operator<<(T newElem) const noexcept{
        VectorN<T,N+1> tempVec;
        std::copy(begin(),end(),tempVec.begin());
        tempVec[N] = newElem;
        return tempVec;
    }


    /**
    * @brief Converts this vector to a smaller vector, truncating extra components.
    *
    * Creates a VectorN<T, M> by copying the first M components of this vector.
    * Remaining components (if any) are simply discarded.
    *
    * @note This conversion is explicit to prevent accidental data loss.
    *
    * @tparam M The target dimension. Must satisfy M < N.
    * @return A new vector containing the first M components of this one.
    */
    template <std::size_t M> requires(M < N)
    [[nodiscard]] explicit constexpr operator VectorN<T,M>() const noexcept{
        VectorN<T,M> tempVec;
        std::copy_n(begin(),M ,tempVec.begin());
        return tempVec;
    }

    /**
    * @brief Converts this vector to a larger vector, zero-padding the new components.
    *
    * Creates a VectorN<T, M> by copying this vector's N components, and filling
    * the remaining (M - N) components with zero.
    *
    * @note This conversion is explicit to avoid unintentional implicit widening
    *       that may introduce silent zero values.
    *
    * @tparam M The target dimension. Must satisfy M > N.
    * @return A new vector where the first N elements are copied from this vector,
    *         and any additional elements are initialized to zero.
    */
    template <std::size_t M> requires (M>N)
    [[nodiscard]] explicit constexpr operator VectorN<T,M>() const noexcept{
        VectorN<T,M> tempVec;
        std::copy_n(begin(), N, tempVec.begin());
        std::fill_n(tempVec.begin() + N, M - N, T{0});
        return tempVec;
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
    [[nodiscard]] constexpr VectorN lerp(VectorN rhs, T t) const noexcept{
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
    constexpr VectorN& lerp_in_place(VectorN rhs, T t) noexcept {
        return zip_in_place(rhs, [t](T a, T b) { return a + (b - a) * t;});
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
    [[nodiscard]] constexpr VectorN reflect(VectorN rhs) const noexcept{
        assert(rhs.magnitude() == 1);
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
    VectorN& reflect_in_place(VectorN rhs)noexcept{
        assert(rhs.magnitude() == 1);
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
    [[nodiscard]] VectorN reflect_safe(VectorN rhs)const noexcept{
        VectorN unitVector = rhs.normalize();
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
    VectorN& reflect_in_place_safe(VectorN rhs)noexcept{
        VectorN unitVector = rhs.normalize();
        T daught = dot(unitVector);
        return zip_in_place(unitVector,[daught](T a, T b){return a-2 * daught *b;});
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
    [[nodiscard]] constexpr VectorN refract(VectorN rhs, T n1, T n2) const noexcept {
        T refractionRatio = n1 / n2;
        T cosi = -(dot(rhs));
        T k = 1.0f - refractionRatio * refractionRatio * (1.0f - cosi * cosi);
        if (k < 0.0f){
            VectorN tempVec;
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

    [[nodiscard]] constexpr VectorN& refract_in_place(VectorN rhs, T n1, T n2) noexcept {
        T refractionRatio = n1 / n2;
        T cosi = -(dot(rhs));
        T k = 1.0f - refractionRatio * refractionRatio * (1.0f - cosi * cosi);
        if (k < 0.0f){
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
    [[nodiscard]] constexpr VectorN refract_safe(VectorN rhs, T n1, T n2) const noexcept {  
        VectorN thisUnit = normalize();
        VectorN rhsUnit = rhs.normalize();

        T refractionRatio = n1 / n2;
        T cosi = -(thisUnit.dot(rhsUnit));
        T k = 1.0f - refractionRatio * refractionRatio * (1.0f - cosi * cosi);

        if (k < 0.0f){
            VectorN tempVec;
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
    [[nodiscard]] constexpr VectorN& refract_in_place_safe(VectorN rhs, T n1, T n2)noexcept {
        *this = normalize();
        VectorN rhsUnit = rhs.normalize();

        T refractionRatio = n1 / n2;
        T cosi = -(dot(rhsUnit));
        T k = 1.0f - refractionRatio * refractionRatio * (1.0f - cosi * cosi);

        if (k < 0.0f){
            std::fill(begin(),end(),T{0});
            return *this;
        }
        T sqrtK = std::sqrt(k);
        return zip_in_place(rhsUnit, [refractionRatio, cosi, sqrtK](T i, T n) {return i * refractionRatio + n * (refractionRatio * cosi - sqrtK);});
    }


    /**
     * @brief Clamps each value into range [minVal, maxVal]
     * 
     * @param minVal the lower range(lowest value that any vector value can be)
     * @param maxVal the upper range(highest value that any vector value can be)
     * @return a new vector with clamped values from the original vector
     */    
    [[nodiscard]] constexpr VectorN clamp(T minVal, T maxVal) noexcept{
        VectorN tempVec;
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
    constexpr VectorN& clamp_in_place(T minVal, T maxVal) noexcept {
        std::transform(begin(), end(), begin(),[minVal, maxVal](T in) { return std::clamp(in, minVal, maxVal);});
        return *this;
    }


    /**
     * @brief compares two vectors and returns true if there values are at least closer than epsilon
     * 
     * @param rhs the vector you are comparing to
     * @param epsilon how close the values need to be to be true. A default exists for 
     *                multiple floating point type
     * @return true if all values are closer than epsilon
     * @return false if any value is further than epsilon
     */
    [[nodiscard]] bool almost_equal(VectorN rhs, T epsilon = ES::math::default_epsilon<T>::value)noexcept{
        VectorN differenceVector = *this - rhs;
        return std::all_of(differenceVector.begin(),differenceVector.end(),[epsilon](T in){return std::fabs(in) <= epsilon;});
    }

    /**
    * @brief Computes the distance (length) of this vector.
    * 
    * The magnitude is a scalar representing the Euclidean length of the vector:
    *
    * @return The distance as a scalar of the same type as the vector's elements.
    *
    * @note This function uses `sqrt`, which can be relatively expensive. 
    *       Use `magnitude_squared()` if you only need comparisons or repeated calculations
    */

    [[nodiscard]] constexpr T distance(VectorN rhs) const noexcept {
        return (*this - rhs).magnitude();
    }

    /**
    * @brief Computes the squared distance of this vector.
    * 
    * The square distance is the sum of the squares of the vector's components:
    *
    * @return The squared distance as a scalar of the same type as the vector's elements.
    *
    * @note This avoids the `sqrt` operation and is therefore cheaper to compute.
    *       Use this for comparisons or repeated calculations where the actual magnitude is not required.
    */
    [[nodiscard]] constexpr T distance_squared(VectorN rhs) const noexcept {
        return (*this - rhs).magnitude_squared();
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
    [[nodiscard]] VectorN project_onto(VectorN rhs) const noexcept{
        assert(rhs.dot(rhs) !=0  && "Divide by zero error in Project onto method");
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
    [[nodiscard]] VectorN project_onto_in_place(VectorN rhs)noexcept{
        assert(rhs.dot(rhs)!=0 && "Divide by zero error in Project onto in place method");
        *this = dot(rhs)/rhs.dot(rhs) *rhs;
        return *this;
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
    */
    [[nodiscard]] VectorN slerp(const VectorN& rhs, T t)const noexcept{
        T daught = dot(rhs);

        T theta = std::acos(daught);
        if (theta < T(1e-6)) {
            return *this;
        }

        T sinTheta = std::sin(theta);
        T w1 = std::sin((1 - t) * theta) / sinTheta;
        T w2 = std::sin(t * theta) / sinTheta;

        return zip(rhs, [w1, w2](T a, T b) {return std::fma(w1, a, w2 * b);});
    }

    /** @brief returns a vector of size N with all Zero values*/
    [[nodiscard]] static constexpr VectorN zero() noexcept {
        VectorN tempVec{};
        std::fill(tempVec.begin(), tempVec.end(), T{0});
        return tempVec;
    }


};
template<typename... Args>
VectorN(Args...) -> VectorN<std::common_type_t<Args...>, sizeof...(Args)>;

template <typename T> using Vector2 = VectorN<T, 2>;
template <typename T> using Vector3 = VectorN<T, 3>;
template <typename T> using Vector4 = VectorN<T, 4>;

}