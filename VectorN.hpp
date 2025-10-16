#pragma once

#include <type_traits>
#include <array>

template <typename T, int N> requires std::is_arithmetic_v<T>
class VectorN{
    std::array<T, N> array{};

    template <typename... Args>
    requires (sizeof...(Args) == N) && (std::conjunction_v<std::is_convertible<Args,T>...>)
    constexpr VectorN(Args... args);


    template <typename Self>
    decltype(auto) operator[](this Self&& self, std::size_t index);
    constexpr T& operator[](int index);
    constexpr const T& operator[](int index) const;

    constexpr bool operator==(const VectorN<T, N>& other) const;

    constexpr VectorN<T, N> operator+(const VectorN<T, N>& other) const;
    constexpr VectorN<T, N> operator-(const VectorN<T, N>& other) const;
    constexpr VectorN<T, N> operator/(const T& scalar) const;
    constexpr VectorN<T, N> operator*(const T& scalar) const;

    constexpr VectorN<T, N> hadamardProduct(const VectorN<T, N>& other) const;
    constexpr VectorN<T, N> hadamardDivide(const VectorN<T, N>& other) const;

    T length() const;
    T lengthSquared() const;
    constexpr T angle(const VectorN<T, N>& other) const;

    VectorN<T, N> normalized() const;
    void normalize();

    template<int M, std::enable_if_t<(M < N), int> = 0>
    constexpr VectorN(const VectorN<T, M>& other);

    template<int M, std::enable_if_t<(M > N), int> = 0>
    explicit constexpr VectorN(const VectorN<T, M>& other);
};
