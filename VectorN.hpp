// Ryan Seavey
// Michael Effinger

#pragma once

#include <type_traits>
#include <array>
#include <cassert>
#include <cmath>
#include <algorithm>
#include <functional>
#include "math.hpp"

namespace ES {

template<typename Op, typename T>
concept FoldExpr = requires(Op op, T accum, T l, T r){
    { op(accum, l, r) } -> std::convertible_to<T>;
};


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
    * @return Reference to the first element.
    */
    [[nodiscard]] constexpr auto&& x(this auto&& self) noexcept requires (N>0 && N<5) {
        return std::forward_like<decltype(self)>(self[0]);
    }

    /**
    * @brief Returns the y component of the vector.
    * @return Reference to the second element.
    */
    [[nodiscard]] constexpr auto&& y(this auto&& self) noexcept requires (N>1 && N<5) {
        return std::forward_like<decltype(self)>(self[1]);
    }

    /**
    * @brief Returns the z component of the vector.
    * @return Reference to the third element.
    */
    [[nodiscard]] constexpr auto&& z(this auto&& self) noexcept requires (N>2 && N<5) {
        return std::forward_like<decltype(self)>(self[2]);
    }

    /**
    * @brief Returns the w component of the vector.
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
    [[nodiscard]] constexpr T zip_reduce(const VectorN rhs, T initial, FoldExpr<T> auto&& exp) const noexcept {
        auto liter = cbegin(), riter = rhs.cbegin();
        while(liter != cend()){
            initial = exp(initial, *liter, *riter);
            ++liter, ++riter;
        }
        return initial;
    }

    /** @} */



    [[nodiscard]] constexpr T dot(const VectorN rhs) const noexcept{
        return zip_reduce(rhs, 0,[](T accum, T l, T r){return accum+(l*r);});
    }
    

    [[nodiscard]] constexpr VectorN cross(const VectorN rhs) const noexcept requires (N == 3){
        return VectorN{
        data_[1] * rhs.data_[2] - data_[2] * rhs.data_[1],
        data_[2] * rhs.data_[0] - data_[0] * rhs.data_[2],
        data_[0] * rhs.data_[1] - data_[1] * rhs.data_[0]
        };
    }

     constexpr VectorN& cross_in_place(const VectorN rhs) noexcept requires (N == 3){
      T xt = data_[1] * rhs.data_[2] - data_[2] * rhs.data_[1],
        yt = data_[2] * rhs.data_[0] - data_[0] * rhs.data_[2],
        zt = data_[0] * rhs.data_[1] - data_[1] * rhs.data_[0];
        x() = xt, y() = yt, z() = zt;
        return *this;
    }

    [[nodiscard]] constexpr T magnitude() const noexcept{
        //Yes dot product should always be positive but floating point erros can make tiny negative: so I check it 
        return std::sqrt(std::fabs(dot(*this)));
    }
    [[nodiscard]] constexpr T magnitude_squared() const noexcept{
        return dot(*this);
    }

    constexpr VectorN& normalize_in_place() noexcept{
        T mag = magnitude();
        if(mag == 0){
            std::fill(begin(),end(),T{0});
            return *this;
        }
        std::transform(begin(), end(), begin(), [mag](T in){ return in / mag; });
        return *this;
    }
    

    [[nodiscard]] constexpr VectorN normalize() const noexcept{
        return VectorN(*this).normalize_in_place();
    }



    /** @defgroup arithmetic Arithmetic Operators
    *  @brief Component-wise arithmetic operations for VectorN.
    *  @{
    */

    [[nodiscard]] constexpr VectorN operator+(const VectorN rhs)const noexcept{
     return zip(rhs,std::plus{});
    }
    
    constexpr VectorN& operator+=(const VectorN rhs) noexcept{
        return zip_in_place(rhs, std::plus{});
    }
    
    [[nodiscard]] constexpr VectorN operator-(const VectorN rhs) const noexcept{
        return zip(rhs, std::minus{});
    }
    
    [[nodiscard]] constexpr VectorN operator-() const noexcept{
        VectorN self{*this};
        std::negate<> op;
        for(auto& i : self) i = op(i);
        return self;
    }

    constexpr VectorN& operator-=(const VectorN rhs) noexcept{
        return zip_in_place(rhs, std::minus{});
    }

    [[nodiscard]] constexpr VectorN operator*(const T scalar) const noexcept{
        VectorN tempVec;
        std::transform(begin(),end(),tempVec.begin(),[scalar](T in){return in * scalar;});
        return tempVec;
    }
    
    constexpr VectorN& operator*=(const T scalar) noexcept{
        std::transform(begin(),end(),begin(),[scalar](T in){return in * scalar;});
        return *this;
    }
    
    [[nodiscard]] constexpr VectorN operator/(const T scalar) const noexcept{
        VectorN tempVec;
        std::transform(begin(), end(), tempVec.begin(),[scalar](T in) {assert(scalar !=0 && "Divide by zero in operator/"); return (scalar != 0) ? (in / scalar) : T{0}; });
        return tempVec;
    }

    constexpr VectorN& operator/=(const T scalar) noexcept{
        std::transform(begin(), end(),begin(),[scalar](T in) {assert(scalar !=0 && "Divide by zero in operator/="); return (scalar != 0) ? (in / scalar) : T{0}; });
        return *this;
    }
    
    [[nodiscard]] friend constexpr VectorN operator*(const auto& scalar, const VectorN lhs) noexcept{
        VectorN tempVec;
        std::transform(lhs.begin(),lhs.end(),tempVec.begin(),[scalar](T in){return in * scalar;});
        return tempVec;
    }
    
    [[nodiscard]] constexpr VectorN hadamard_product(const VectorN rhs) const noexcept{
        return zip(rhs, std::multiplies{});
    }
    
    constexpr VectorN& hadamard_product_in_place(const VectorN rhs) noexcept{
        return zip_in_place(rhs, std::multiplies{});
    }
    
    [[nodiscard]] constexpr VectorN hadamard_divide(const VectorN rhs) const noexcept {
        return zip(rhs, [](T a, T b) { assert(b !=0 && "Divide by zero in hadamardDivide"); return (b != 0) ? (a / b) : T{0}; });
    }
    
    constexpr VectorN& hadamard_divide_in_place(const VectorN rhs) noexcept {
        return zip_in_place(rhs, [](T a, T b) { assert(b !=0 && "Divide by zero in hadamardDivide"); return (b != 0) ? (a / b) : T{0}; });
    }


/** @} */




    






    

    [[nodiscard]] constexpr bool operator==(const VectorN other)const noexcept{
        if(other.data_ == data_){
            return true;
        }
        return false;
    }



    [[nodiscard]] constexpr T angle(const VectorN<T, N> rhs) const noexcept{
        T thisMag = magnitude_squared();
        T thatMag = rhs.magnitude_squared();
        
        if(thisMag == 0 || thatMag == 0) {
            assert(false && "Divide by zero error in angle calculation");
            return T{0};
        }
        return std::acos(std::clamp(dot(rhs) / std::sqrt(thisMag * thatMag), T{-1}, T{1}));  
    }

    [[nodiscard]] constexpr VectorN<T,N+1>operator<<(T newElem) const noexcept{
        VectorN<T,N+1> tempVec;
        std::copy(begin(),end(),tempVec.begin());
        tempVec[N] = newElem;
        return tempVec;
    }


    template <std::size_t M> requires(M < N)
    [[nodiscard]] explicit constexpr operator VectorN<T,M>() const noexcept{
        VectorN<T,M> tempVec;
        std::copy_n(begin(),M ,tempVec.begin());
        return tempVec;
    }
    
    template <std::size_t M> requires (M>N)
    [[nodiscard]] explicit constexpr operator VectorN<T,M>() const noexcept{
        VectorN<T,M> tempVec;
        std::copy_n(begin(), N, tempVec.begin());
        std::fill_n(tempVec.begin() + N, M - N, T{0});
        return tempVec;
    }

    [[nodiscard]] constexpr VectorN lerp(VectorN rhs, T t) const noexcept{
         return zip(rhs,[t](T a, T b) {return a+(b-a)*t;});
    }

    constexpr VectorN& lerp_in_place(VectorN rhs, T t) noexcept {
        return zip_in_place(rhs, [t](T a, T b) { return a + (b - a) * t;});
    }
 
    [[nodiscard]] constexpr VectorN reflect(VectorN rhs) const noexcept{
        assert(rhs.magnitude() == 1);
        T daught =  dot(rhs);
        return zip(rhs,[daught](T a, T b) {return a - 2 * daught * b;});
    }
    
    VectorN& reflect_in_place(VectorN rhs)noexcept{
        assert(rhs.magnitude() == 1);
        T daught =  dot(rhs);
        return zip_in_place(rhs,[daught](T a, T b) {return a - 2 * daught * b;});
    }
    
    [[nodiscard]] VectorN reflect_safe(VectorN rhs)const noexcept{
        VectorN unitVector = rhs.normalize();
        T  daught = dot(unitVector);
        return zip(unitVector,[daught](T a, T b){return a-2*daught * b;});
    }
    
    VectorN& reflect_in_place_safe(VectorN rhs)noexcept{
        VectorN unitVector = rhs.normalize();
        T daught = dot(unitVector);
        return zip_in_place(unitVector,[daught](T a, T b){return a-2 * daught *b;});
    }

    
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
    
    constexpr VectorN& clamp_in_place(T minVal, T maxVal) noexcept {
        std::transform(begin(), end(), begin(),[minVal, maxVal](T in) { return std::clamp(in, minVal, maxVal);});
        return *this;
    }

    [[nodiscard]] constexpr VectorN clamp(T minVal, T maxVal) noexcept{
        VectorN tempVec;
        std::transform(begin(),end(), tempVec.begin(),[minVal,maxVal](T in){return std::clamp(in, minVal, maxVal);});
        return tempVec;
    }
    

    [[nodiscard]] bool almost_equal(VectorN rhs, T epsilon = ES::math::default_epsilon<T>())noexcept{
        VectorN differenceVector = *this - rhs;
        return std::all_of(differenceVector.begin(),differenceVector.end(),[epsilon](T in){return std::fabs(in) <= epsilon;});
    }

    [[nodiscard]] constexpr T distance(VectorN rhs) const noexcept {
        return (*this - rhs).magnitude();
    }

    [[nodiscard]] constexpr T distance_squared(VectorN rhs) const noexcept {
        return (*this - rhs).magnitude_squared();
    }


    [[nodiscard]] VectorN project_onto(VectorN rhs) const noexcept{
        assert(rhs.dot(rhs) !=0  && "Divide by zero error in Project onto method");
        return dot(rhs)/rhs.dot(rhs) * rhs;
    }

    [[nodiscard]] VectorN project_onto_in_place(VectorN rhs)noexcept{
        assert(rhs.dot(rhs)!=0 && "Divide by zero error in Project onto in place method");
        *this = dot(rhs)/rhs.dot(rhs) *rhs;
        return *this;
    }

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