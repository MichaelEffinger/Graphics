#pragma once

#include <type_traits>
#include <array>





template <typename T, int N> requires std::is_arithmetic_v<T>
class VectorN{
    
    std::array<T,N> array{};

    template <typename... Args> requires (sizeof...(Args) == N) &&(std::conjunction_v<std::is_convertible<Args,T>...>)
    
    
    constexpr VectorN(Args... args);


};

#include "VectorN.inl"