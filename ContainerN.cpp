#pragma once
#include <cstddef>
#include <type_traits>
#include <array>
#include <cassert>

template<typename Child, size_t N>
class ContainerN{

    protected:
    std::array<typename Child::value_type, N> data_;

    template <typename... Args> requires (sizeof...(Args) == N)
    constexpr ContainerN(Args&&... args) noexcept((std::is_nothrow_constructible_v<T, Args&&> && ...)) : data_{static_cast<T>(args)...} {}

    template<size_t M, typename... U> requires (sizeof...(U) == N - M)
    constexpr ContainerN(const ContainerN<T, M>& smaller, U... extras) noexcept((std::is_nothrow_constructible_v<T,U&&>&&...)): data_{} {      
        std::copy(smaller.cbegin(), smaller.cend(), data_.begin());
        size_t index = M;
        ((data_[index++] = T(static_cast<T>(extras))), ...);
    }


    [[nodiscard]] constexpr auto&& operator[](this auto&& self, std::size_t index)noexcept{
        assert(index < N && "Vector index out of range");
        return std::forward_like<decltype(self)>(self.data_[index]);
    }

    [[nodiscard]] constexpr auto&& at(this auto&& self, std::size_t index)noexcept{
        assert(index < N && "Vector index out of range");
        if(index >=N){
            index = N-1;
        }
        return std::forward_like<decltype(self)>(self.data_[index]);
    }
  
};