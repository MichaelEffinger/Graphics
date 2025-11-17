#pragma once

#include "VectorN.hpp"
#include "PointN.hpp"
#include <cmath>
#include <algorithm>
#include <cassert>
#include "ContainerN.hpp"
#include "PointN.hpp"


// note Feel free to use code from this class as many returns will work corretly, but beware this entire class will become a container CTRP subclass soon 
namespace ES{



template <typename T, std::size_t dummy = 4>
class VectorH : public ContainerN<VectorH,T,4> {
    static const int N = 4;
    
    public:
    [[deprecated("This will be completely demolished with refactor, DONT USE")]] ES::VectorN<float,3> xyz_;
    [[deprecated("This will be iniliated with the refactor, DONT USE")]] float w_ = T{1};
    using ContainerN<VectorH,T,N>::zip_in_place;
    using ContainerN<VectorH,T,N>::zip;
    using ContainerN<VectorH,T,N>::zip_reduce;
    using ContainerN<VectorH,T,N>::begin;
    using ContainerN<VectorH,T,N>::end;
    using ContainerN<VectorH,T,N>::cend;
    using ContainerN<VectorH,T,N>::cbegin;
    using ContainerN<VectorH,T,N>::data_;
    using ContainerN<VectorH,T,N>::ContainerN;

    [[nodiscard]] constexpr auto&& x(this auto&& self) noexcept{
        return std::forward_like<decltype(self)>(self[0]);
    }


    [[nodiscard]] constexpr auto&& y(this auto&& self) noexcept{
       return std::forward_like<decltype(self)>(self[1]);
    }

    [[nodiscard]] constexpr auto&& z(this auto&& self) noexcept {
       return std::forward_like<decltype(self)>(self[2]);
    }

    [[nodiscard]] constexpr auto&& w(this auto&& self) noexcept{
        return std::forward_like<decltype(self)>(self[3]);
    }


    [[nodiscard]] constexpr VectorH operator+(const VectorH rhs) const noexcept {
        assert((!w() || !rhs.w()) && "Cannot add point to point");
        return zip(rhs,std::plus{});
    }

    constexpr VectorH& operator+=(VectorH rhs) noexcept {
        assert((!w() || !rhs.w()) && "Cannot add point to point");
        return zip_in_place(rhs,std::plus());
    }

    [[nodiscard]] constexpr VectorH operator-(const VectorH rhs) const noexcept {
        assert((w() || !rhs.w()) && "Cannot subtract point from direction");
        return zip(rhs,std::minus());
    }

    constexpr VectorH& operator-=(const VectorH rhs) noexcept {
        assert((w() || !rhs.w())&& "Cannot subtract point from direction");
        return zip_in_place(rhs,std::minus());
    }


    [[nodiscard]] constexpr VectorH operator*(T scalar) const noexcept {
        VectorH tempVec;
        std::transform(begin(),end(),tempVec.begin(),[scalar](T in){return in * scalar;});
        return tempVec;
    }

    constexpr VectorH& operator*=(T scalar) noexcept {
        std::transform(begin(),end(),begin(),[scalar](T in){return in * scalar;});
        return *this;
    }

    [[nodiscard]] constexpr VectorH operator/(T scalar) const noexcept {
        assert(scalar != T{0} && "divide by zero error in VectorH operator/");
        if(scalar == T{0}){
            return VectorH(T{0},T{0},T{0},w());
        }
        VectorH tempVec;
        std::transform(cbegin(),cend(),tempVec.begin(), [scalar](T in){return in /scalar;});
        return tempVec;
    }

    constexpr VectorH& operator/=(T scalar) noexcept {
        assert(scalar != T{0} && "divide by zero error in VectorH operator/=");
        if(scalar == T{0}){
            *this = {0,0,0,w()};
        }
        std::transform(begin(),end(),begin(),[scalar](T in){return in/scalar;});
        return *this;
    }

    
    constexpr VectorH hadamardProduct(const VectorH rhs) const noexcept {
        assert((!w() && !rhs.w())  && "Hadamard product requires both Hvectors be directions");
        return zip(rhs,std::multiplies());
    }

    constexpr VectorH& hadamardProduct_in_place(const VectorH rhs) noexcept {
        assert((!w() && !rhs.w())  && "Hadamard product requires both Hvecotrs be directions");
        return zip_in_place(rhs,std::multiplies());
    }

    [[nodiscard]] constexpr VectorH hadamardDivide(const VectorH rhs) const noexcept {
        assert((!w() && !rhs.w()) && "Hadamard divide requires both Hvectors be directions");
        return zip(rhs, [](T a, T b) { assert(b !=0 && "Divide by zero in hadamardDivide"); return (b != 0) ? (a / b) : T{0}; });
    }

    constexpr VectorH& hadamardDivide_in_place(const VectorH rhs) noexcept {
        assert((!w() && !rhs.w()) && "Hadamard divide requires both Hvectors be directions");
        return zip_in_place(rhs, [](T a, T b) { assert(b !=0 && "Divide by zero in hadamardDivide"); return (b != 0) ? (a / b) : T{0}; });
    }


    constexpr T dot(const VectorH& rhs) const noexcept {
        assert((!w() && !rhs.w()) && "Dot product requires two directions");
        return zip_reduce(rhs, 0,[](T accum, T l, T r){return accum+(l*r);});
    }

    [[nodiscard]] constexpr VectorH cross(const VectorH& rhs) const noexcept {
        assert((!w() && !rhs.w()) && "Cross product requires two directions" );
        return VectorH{
        data_[1] * rhs.data_[2] - data_[2] * rhs.data_[1],
        data_[2] * rhs.data_[0] - data_[0] * rhs.data_[2],
        data_[0] * rhs.data_[1] - data_[1] * rhs.data_[0], 
        T{0}};
    }

    constexpr VectorH& cross_in_place(const VectorH& rhs) noexcept {
        assert((w() == 0 && w() == 0) && "Cross product requires two directions" );
        x() = data_[1] * rhs.data_[2] - data_[2] * rhs.data_[1];
        y() = data_[2] * rhs.data_[0] - data_[0] * rhs.data_[2];
        z() = data_[0] * rhs.data_[1] - data_[1] * rhs.data_[0];
        return *this;
    }


    [[nodiscard]] constexpr T magnitude() const noexcept {
        return std::sqrt(std::fabs(zip_reduce(*this, 0,[](T accum, T l, T r){return accum+(l*r);})));
    }

    [[nodiscard]] constexpr T magnitudeSquared() const noexcept {
        return std::fabs(zip_reduce(*this, 0,[](T accum, T l, T r){return accum+(l*r);}));
    }

    constexpr VectorH& normalize() noexcept {
        return VectorN(*this).normalize_in_place();
    }

    [[nodiscard]] constexpr VectorH normalize_in_place() const noexcept {
        T mag = magnitude();
        if(mag == 0){
            assert(false && "Divide by zero error in normalize_in_place calculation");
            std::fill(begin(),end(),T{0});
            return *this;
        }
        std::transform(begin(), end(), begin(), [mag](T in){ return in / mag; });
        return *this;
    }


    [[nodiscard]] [[deprecated("Non implemented")]] constexpr bool operator==(const VectorH& rhs) const noexcept {
        return w_ == rhs.w_ && xyz_ == rhs.xyz_;
    }

    [[nodiscard]] [[deprecated("Non implemented")]] constexpr bool operator!=(const VectorH& rhs) const noexcept {
        return !(*this == rhs);
    }



    [[nodiscard]] [[deprecated("Non implemented")]] constexpr VectorH homogenize() const noexcept {
        if (w_ == 0) {
            return *this;
        }
        return VectorH(xyz_/w_,1);
    }

    [[deprecated("Non implemented")]] constexpr VectorH& homogenized() noexcept {
        if (w_ == 0) {
            return *this;
        }
        xyz_/= w_;
        w_ = 1;
        return *this;
    }

    [[deprecated("Non implemented")]] [[nodiscard]] T angle(const VectorH){
        return T{0};
    }

    [[deprecated("Non implemented")]] [[nodiscard]] VectorH slerp(const VectorH){
        VectorH myVec;
        return myVec;
    }

    [[deprecated("Non implemented")]] [[nodiscard]] VectorH reflect(const VectorH){
        VectorH myVec;
        return myVec;
    }

    [[deprecated("Non implemented")]] VectorH reflect_in_place(const VectorH){
        return *this;
    }

    [[deprecated("Non implemented")]] [[nodiscard]] VectorH reflect_safe(const VectorH){
        VectorH myVec;
        return myVec;
    }

    [[deprecated("Non implemented")]] VectorH reflect_in_place_safe(const VectorH){
        return *this;
    }

    [[deprecated("Non implemented")]] [[nodiscard]] [[deprecated("Non implemented")]] constexpr VectorH lerp(VectorH rhs, T t) const noexcept{
        return zip(rhs,[t](T a, T b) {return a+(b-a)*t;});
    }

    [[deprecated("Non implemented")]] [[deprecated("Non implemented")]] constexpr VectorH& lerp_in_place(VectorH rhs, T t) noexcept {
        return zip_in_place(rhs, [t](T a, T b) { return a + (b - a) * t;});
    }     

    [[nodiscard]] [[deprecated("Non implemented")]] constexpr VectorH clamp(T minVal, T maxVal) noexcept{
        VectorH tempVec;
        std::transform(begin(),end(), tempVec.begin(),[minVal,maxVal](T in){return std::clamp(in, minVal, maxVal);});
        return tempVec;
    }
    
    [[deprecated("Non implemented")]] constexpr VectorH& clamp_in_place(T minVal, T maxVal) noexcept {
        std::transform(begin(), end(), begin(),[minVal, maxVal](T in) { return std::clamp(in, minVal, maxVal);});
        return *this;
    }



    [[deprecated("Non implemented")]] [[nodiscard]] constexpr VectorH refract(VectorH rhs, T n1, T n2) const noexcept {
        T refractionRatio = n1 / n2;
        T cosi = -(dot(rhs));
        T k = T{1} - refractionRatio * refractionRatio * (T{1} - cosi * cosi);
        if (k < T{0}){
            VectorN tempVec;
            std::fill(tempVec.begin(),tempVec.end(),T{0});
            return tempVec;
        }
        T sqrtK = std::sqrt(k);

        return zip(rhs, [refractionRatio, cosi, sqrtK](T i, T n) {return i * refractionRatio + n * (refractionRatio * cosi - sqrtK);});
    }

    [[deprecated("Non implemented")]] constexpr VectorH& refract_in_place(VectorH rhs, T n1, T n2) noexcept {
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

    [[deprecated("Non implemented")]] [[nodiscard]] constexpr VectorH refract_safe(VectorH rhs, T n1, T n2) const noexcept {  
        VectorN thisUnit = normalize();
        VectorN rhsUnit = rhs.normalize();

        T refractionRatio = n1 / n2;
        T cosi = -(thisUnit.dot(rhsUnit));
        T k = T{1} - refractionRatio * refractionRatio * (T{1} - cosi * cosi);

        if (k < T{0}){
            VectorN tempVec;
            std::fill(tempVec.begin(),tempVec.end(),T{0});
            return tempVec;
        }

        T sqrtK = std::sqrt(k);
        return thisUnit.zip(rhsUnit, [refractionRatio, cosi, sqrtK](T i, T n) {return i * refractionRatio + n * (refractionRatio * cosi - sqrtK);});
    }

    [[deprecated("Non implemented")]] constexpr VectorH& refract_in_place_safe(VectorH rhs, T n1, T n2)noexcept {
        *this = normalize();
        VectorN rhsUnit = rhs.normalize();

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



    [[deprecated("Non implemented")]] [[nodiscard]] VectorH project_onto(VectorH rhs) const noexcept{
        assert(rhs.dot(rhs) !=0  && "Divide by zero error in Project onto method");
        return dot(rhs)/rhs.dot(rhs) * rhs;
    }

    [[deprecated("Non implemented")]] VectorH project_onto_in_place(VectorH rhs)noexcept{
        assert(rhs.dot(rhs)!=0 && "Divide by zero error in Project onto in place method");
        *this = dot(rhs)/rhs.dot(rhs) *rhs;
        return *this;
    }






};

}