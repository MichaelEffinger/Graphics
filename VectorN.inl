#pragma once

#include "VectorN.hpp"
#include <type_traits>
#include <cmath>

template <typename T, int N> requires std::is_arithmetic_v<T>
template <typename... Args> requires (sizeof...(Args) == N) && (std::conjunction_v<std::is_convertible<Args,T>...>)
constexpr VectorN<T, N>::VectorN(Args... args)
{
    // TODO: implement
}

template <typename T, int N> requires std::is_arithmetic_v<T>
T& VectorN<T, N>::operator[](int index)
{
    // TODO: implement
    return array[index];
}

template <typename T, int N> requires std::is_arithmetic_v<T>
const T& VectorN<T, N>::operator[](int index) const
{
    // TODO: implement
    return array[index];
}

template <typename T, int N> requires std::is_arithmetic_v<T>
const bool VectorN<T, N>::operator==(const VectorN<T, N>& other) const
{
    // TODO: implement
    return false;
}

template <typename T, int N> requires std::is_arithmetic_v<T>
VectorN<T, N> VectorN<T, N>::operator+(const VectorN<T, N>& other) const
{
    // TODO: implement
    return *this;
}

template <typename T, int N> requires std::is_arithmetic_v<T>
VectorN<T, N> VectorN<T, N>::operator-(const VectorN<T, N>& other) const
{
    // TODO: implement
    return *this;
}

template <typename T, int N> requires std::is_arithmetic_v<T>
VectorN<T, N> VectorN<T, N>::operator/(const T scalar) const
{
    // TODO: implement
    return *this;
}

template <typename T, int N> requires std::is_arithmetic_v<T>
VectorN<T, N> VectorN<T, N>::operator*(const T scalar) const
{
    // TODO: implement
    return *this;
}

template <typename T, int N> requires std::is_arithmetic_v<T>
VectorN<T, N> VectorN<T, N>::hadamardProduct(const VectorN<T, N>& other) const
{
    // TODO: implement
    return *this;
}

template <typename T, int N> requires std::is_arithmetic_v<T>
VectorN<T, N> VectorN<T, N>::hadamardDivide(const VectorN<T, N>& other) const
{
    // TODO: implement
    return *this;
}

template <typename T, int N> requires std::is_arithmetic_v<T>
T VectorN<T, N>::length()
{
    // TODO: implement
    return T{};
}

template <typename T, int N> requires std::is_arithmetic_v<T>
T VectorN<T, N>::lengthSquared()
{
    // TODO: implement
    return T{};
}

template <typename T, int N> requires std::is_arithmetic_v<T>
const T VectorN<T, N>::angle(const VectorN<T, N>& other) const
{
    // TODO: implement
    return T{};
}

template <typename T, int N> requires std::is_arithmetic_v<T>
VectorN<T, N> VectorN<T, N>::normalized()
{
    // TODO: implement
    return *this;
}

template <typename T, int N> requires std::is_arithmetic_v<T>
void VectorN<T, N>::normalize()
{
    // TODO: implement
}

template <typename T, int N> requires std::is_arithmetic_v<T>
template <int M, std::enable_if_t<(M < N), int>>
constexpr VectorN<T, N>::VectorN(const VectorN<T, M>& other)
{
    // TODO: implement
}

template <typename T, int N> requires std::is_arithmetic_v<T>
template <int M, std::enable_if_t<(M > N), int>>
constexpr VectorN<T, N>::VectorN(const VectorN<T, M>& other)
{
    // TODO: implement
}

