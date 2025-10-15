#pragma once

#include <type_traits>
#include <array>





template <typename T, int N> requires std::is_arithmetic_v<T>
class VectorN{
    
    std::array<T,N> array{};

    template <typename... Args> requires (sizeof...(Args) == N) &&(std::conjunction_v<std::is_convertible<Args,T>...>)
    
    constexpr VectorN(Args... args);

    T& operator[](int index);

    const T& operator[](int index) const;

    const bool operator==(const VectorN<T, N>& other)const;

    VectorN<T,N> operator+(const VectorN<T,N>&other)const;

    VectorN<T,N> operator-(const VectorN<T,N>&other)const;

    VectorN<T,N> operator/(const T scalar)const;

    VectorN<T,N> operator*(const T scalar)const;

    VectorN<T,N> hadamardProduct(const VectorN<T,N>&other)const;

    VectorN<T,N> hadamardDivide(const VectorN<T,N>&other)const;

    T length();

    const T angle(const VectorN<T,N>&other)const;

    VectorN<T,N> normalized();

    void normalize();

    template<int M, std::enable_if_t<(M < N), int> = 0>
    constexpr VectorN(const VectorN<T, M>& other);

    template<int M, std::enable_if_t<(M > N), int> = 0>
    explicit constexpr VectorN(const VectorN<T, M>& other);





};

#include "VectorN.inl"