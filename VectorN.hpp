// Ryan Seavey
// Michael Effinger

#pragma once

#include <type_traits>
#include <array>
#include <cassert>
#include <cmath>
#include <algorithm>
#include <functional>


namespace ES {

template<typename Op, typename T>
concept FoldExpr = requires(Op op, T accum, T l, T r){
    { op(accum, l, r) } -> std::convertible_to<T>;
};

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

    template <typename... Args>
    requires (sizeof...(Args) == N) && (std::conjunction_v<std::is_convertible<Args, T>...>)
    constexpr VectorN(Args&&... args) noexcept((std::is_nothrow_constructible_v<T, Args&&> && ...))
    : data_{std::forward<Args>(args)...} {}


    [[nodiscard]] constexpr auto operator[](this auto&& self, std::size_t index)noexcept{
        assert(index < N && "Vector index out of range");
        return std::forward_like<decltype(self)>(self.data_[index]);
    }

    [[nodiscard]] constexpr auto at(this auto&& self, std::size_t index){
        assert(index < N && "Vector index out of range");
        if(index >=N){
            index = N-1;
        }
        return std::forward_like<decltype(self)>(self.data_[index]);
    }

    [[nodiscard]] static constexpr std::size_t size(){
        return N;
    }

    [[nodiscard]] constexpr auto&& x(this auto&& self) noexcept requires (N>0 && N<5){
        return std::forward_like<decltype(self)>(self[0]);
    }

    [[nodiscard]] constexpr auto&& y(this auto&& self) noexcept requires (N>1 && N<5){
        return std::forward_like<decltype(self)>(self[1]);
    }
    [[nodiscard]] constexpr auto&& z(this auto&& self) noexcept requires (N>2 && N<5){
        return std::forward_like<decltype(self)>(self[2]);
    }

    [[nodiscard]] constexpr auto&& w(this auto&& self) noexcept requires (N==4){
        return std::forward_like<decltype(self)>(self[3]);
    }

    [[nodiscard]] constexpr auto&& head(this auto&& self) noexcept requires (N > 0) {
    return std::forward_like<decltype(self)>(self[0]);
    }

    [[nodiscard]] constexpr auto&& tail(this auto&& self) noexcept requires (N > 0) {
    return std::forward_like<decltype(self)>(self[N-1]);
    }

    [[nodiscard]] constexpr auto begin() noexcept{return data_.begin();}
    [[nodiscard]] constexpr auto begin() const noexcept{return data_.begin();}
    [[nodiscard]] constexpr auto cbegin() const noexcept{return data_.cbegin();}
    [[nodiscard]] constexpr auto end() noexcept{return data_.end();}
    [[nodiscard]] constexpr auto end() const noexcept{return data_.end();}
    [[nodiscard]] constexpr auto cend() const noexcept{return data_.cend();}


    template<typename BinaryOp>
    [[nodiscard]] constexpr VectorN zip(const VectorN& rhs, BinaryOp op) const noexcept{
        VectorN resultant;
        auto liter = cbegin(), riter = rhs.cbegin();
        auto oiter = resultant.begin();
        while(liter != cend()){
            *oiter = op(*liter, *riter);
            ++liter, ++riter, ++oiter;
        }
        return resultant;
    }

    template<typename BinaryOp>
    constexpr VectorN& zip_in_place(const VectorN& rhs, BinaryOp op) noexcept{
        auto liter = begin();
        auto riter = rhs.cbegin();
        while(liter != end()){
            *liter = op(*liter, *riter);
            ++liter, ++riter;
        }
        return *this;
    }

    [[nodiscard]] constexpr T zip_reduce(const VectorN& rhs, T initial, FoldExpr<T> auto&& exp) const {
        auto liter = cbegin(), riter = rhs.cbegin();
        while(liter != cend()){
            initial = exp(initial, *liter, *riter);
            ++liter, ++riter;
        }
        return initial;
    }


    [[nodiscard]] constexpr T dot(const VectorN& rhs) const noexcept{
        return zip_reduce(rhs, 0,[](T accum, T l, T r){return accum+(l*r);});
    }
    

    [[nodiscard]] constexpr VectorN cross(const VectorN& rhs) const noexcept requires (N == 3){
        return VectorN{
        data_[1] * rhs.data_[2] - data_[2] * rhs.data_[1],
        data_[2] * rhs.data_[0] - data_[0] * rhs.data_[2],
        data_[0] * rhs.data_[1] - data_[1] * rhs.data_[0]
        };
    }

     constexpr VectorN& cross_in_place(const VectorN& rhs) noexcept requires (N == 3){
      T xt = data_[1] * rhs.data_[2] - data_[2] * rhs.data_[1],
        yt = data_[2] * rhs.data_[0] - data_[0] * rhs.data_[2],
        zt = data_[0] * rhs.data_[1] - data_[1] * rhs.data_[0];
        x() = xt, y() = yt, z() = zt;
        return *this;
    }

    [[nodiscard]] constexpr T magnitude() const noexcept{
        //Yes dot product should always be positive but floating point erros can make tiny negative: so I check it 
        return std::sqrt(std::abs(dot(*this)));
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


    [[nodiscard]] constexpr VectorN operator+(const VectorN& rhs)const{
        return zip(rhs,std::plus{});
    }

    constexpr VectorN& operator+=(const VectorN& rhs) noexcept{
        return zip_in_place(rhs, std::plus{});
    }

    [[nodiscard]] constexpr VectorN operator-(const VectorN& rhs) const noexcept{
        return zip(rhs, std::minus{});
    }

    
    [[nodiscard]] constexpr VectorN operator-() const noexcept{
        VectorN self{*this};
        std::negate<> op;
        for(auto& i : self) i = op(i);
        return self;
    }

    constexpr VectorN& operator-=(const VectorN& rhs) noexcept{
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

    [[nodiscard]] constexpr VectorN hadamard_product(const VectorN& rhs) const noexcept{
        return zip(rhs, std::multiplies{});
    }

    constexpr VectorN& hadamard_product_in_place(const VectorN& rhs) noexcept{
        return zip_in_place(rhs, std::multiplies{});
    }

    [[nodiscard]] constexpr VectorN hadamard_divide(const VectorN& rhs) const noexcept {
        return zip(rhs, [](T a, T b) { assert(b !=0 && "Divide by zero in hadamardDivide"); return (b != 0) ? (a / b) : T{0}; });
    }

    constexpr VectorN& hadamard_divide_in_place(const VectorN& rhs) noexcept {
        return zip_in_place(rhs, [](T a, T b) { return (b != 0) ? (a / b) : T{0}; });
    }
    
    [[nodiscard]] friend constexpr VectorN operator*(const auto& scalar, const VectorN& lhs) noexcept{
        VectorN tempVec;
        std::transform(lhs.begin(),lhs.end(),tempVec.begin(),[scalar](T in){return in * scalar;});
        return tempVec;
    }

    [[nodiscard]] constexpr bool operator==(const VectorN& other)const noexcept{
        if(other.data_ == data_){
            return true;
        }
        return false;
    }



    [[nodiscard]] constexpr T angle(const VectorN<T, N>& rhs) const noexcept{
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


    template <int M> requires(M < N)
    [[nodiscard]] explicit constexpr operator VectorN<T,M>() const noexcept{
        VectorN<T,M> tempVec;
        std::copy_n(begin(),M ,tempVec.begin());
        return tempVec;
    }
    
    template <int M> requires (M>N)
    [[nodiscard]] constexpr operator VectorN<T,M>() const noexcept{
        VectorN<T,M> tempVec;
        std::copy_n(begin(), N, tempVec.begin());
        std::fill_n(tempVec.begin() + N, M - N, T{0});
        return tempVec;
    }

    [[nodiscard]] constexpr VectorN lerp(VectorN& rhs, T t) const noexcept{
         return zip(rhs,[t](T a, T b) {return a+(b-a)*t;});
    }

    constexpr VectorN& lerp_in_place(const VectorN& rhs, T t) noexcept {
        return zip_in_place(rhs, [t](T a, T b) { return a + (b - a) * t;});
    }
 
    [[nodiscard]] constexpr VectorN reflect(const VectorN& rhs) const noexcept{
        assert(rhs.magnitude() == 1);
        T daught =  dot(rhs);
        return zip(rhs,[daught](T a, T b) {return a - 2 * daught * b;});
    }
    
    VectorN& reflect_in_place(const VectorN& rhs)noexcept{
        assert(rhs.magnitude() == 1);
        T daught =  dot(rhs);
        return zip_in_place(rhs,[daught](T a, T b) {return a - 2 * daught * b;});
    }
    
    [[nodiscard]] VectorN reflect_safe(const VectorN& rhs)const noexcept{
        VectorN unitVector = rhs.normalize();
        T  daught = dot(unitVector);
        return zip(unitVector,[daught](T a, T b){return a-2*daught * b;});
    }
    
    VectorN& reflect_in_place_safe(const VectorN& rhs)noexcept{
        VectorN unitVector = rhs.normalize();
        T daught = dot(unitVector);
        return zip_in_place(unitVector,[daught](T a, T b){return a-2 * daught *b;});
    }

    
    [[nodiscard]] constexpr VectorN refract(const VectorN& rhs, T n1, T n2) const noexcept {
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


    [[nodiscard]] constexpr VectorN& refract_in_place(const VectorN& rhs, T n1, T n2) noexcept {
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
    
    
    [[nodiscard]] constexpr VectorN refract_safe(const VectorN& rhs, T n1, T n2) const noexcept {  
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

    
    [[nodiscard]] constexpr VectorN& refract_in_place_safe(const VectorN& rhs, T n1, T n2)noexcept {
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
    
    [[nodiscard]] constexpr VectorN& clamp_in_place(T minVal, T maxVal) noexcept {
        std::transform(begin(), end(), begin(),[minVal, maxVal](T in) { return std::clamp(in, minVal, maxVal);});
        return *this;
    }

    [[nodiscard]] constexpr VectorN clamp(T minVal, T maxVal) noexcept{
        VectorN tempVec;
        std::transform(begin(),end(), tempVec.begin(),[minVal,maxVal](T in){return std::clamp(in, minVal, maxVal);});
        return tempVec;
    }
    

    [[nodiscard]] bool almost_equal(VectorN rhs, T epsilon){
        VectorN differenceVector = *this - rhs;
        return std::all_of(differenceVector.begin(),differenceVector.end(),[epsilon](T in){return std::abs(in) <= epsilon;});
    }

    [[nodiscard]] constexpr T distance(const VectorN& rhs) const noexcept {
        return (*this - rhs).magnitude();
    }

    [[nodiscard]] constexpr T distance_squared(const VectorN& rhs) const noexcept {
        return (*this - rhs).magnitude_squared();
    }


    [[nodiscard]] VectorN project_onto(const VectorN& rhs) const noexcept{
        assert(rhs.dot(rhs) !=0  && "Divide by zero error in Project onto method");
        return dot(rhs)/rhs.dot(rhs) * rhs;
    }

    [[nodiscard]] VectorN project_onto_in_place(const VectorN& rhs)noexcept{
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