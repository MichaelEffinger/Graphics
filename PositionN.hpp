#include <type_traits>
#include <array>
#include <cassert>
#include <cmath>
#include <algorithm>
#include <functional>
#include "ES_math.hpp"
#include "VectorN.hpp"

template <typename T, std::size_t N>
  requires(std::is_arithmetic_v<T>)
class PositionN {

  std::array<T, N> data_;

public:

    template <typename... Args> requires (sizeof...(Args) == N)
    constexpr PositionN(Args&&... args) noexcept((std::is_nothrow_constructible_v<T, Args&&> && ...)) : data_{static_cast<T>(args)...} {}

    template<size_t M, typename... U> requires (sizeof...(U) == N - M)
    constexpr PositionN(const PositionN<T, M>& smaller, U... extras) noexcept((std::is_nothrow_constructible_v<T,U&&>&&...)): data_{} {      
        std::copy(smaller.cbegin(), smaller.cend(), data_.begin());
        size_t index = M;
        ((data_[index++] = T(static_cast<T>(extras))), ...);
    }

    [[nodiscard]] constexpr auto&& x(this auto&& self) noexcept requires (N>0 && N<5) {
        return std::forward_like<decltype(self)>(self[0]);
    }

    [[nodiscard]] constexpr auto&& y(this auto&& self) noexcept requires (N>1 && N<5) {
        return std::forward_like<decltype(self)>(self[1]);
    }

    [[nodiscard]] constexpr auto&& z(this auto&& self) noexcept requires (N>2 && N<5) {
        return std::forward_like<decltype(self)>(self[2]);
    }

    [[nodiscard]] constexpr auto&& w(this auto&& self) noexcept requires (N==4) {
        return std::forward_like<decltype(self)>(self[3]);
    }

    [[nodiscard]] constexpr auto&& head(this auto&& self) noexcept requires (N > 0) {
        return std::forward_like<decltype(self)>(self[0]);
    }

    [[nodiscard]] constexpr auto&& tail(this auto&& self) noexcept requires (N > 0) {
        return std::forward_like<decltype(self)>(self[N-1]);
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






    [[nodiscard]] constexpr PositionN operator+(ES::VectorN<T,N> rhs)const noexcept{}
    
    [[nodiscard]] friend constexpr PositionN operator+(ES::VectorN<T,N> lhs, PositionN rhs) noexcept {}

    constexpr PositionN &operator+=(ES::VectorN<T,N> rhs)noexcept {}
    
    [[nodiscard]] constexpr ES::VectorN<T,N> operator-(PositionN rhs) const noexcept {}

    [[nodiscard]] constexpr PositionN operator-(ES::VectorN<T,N> rhs)const noexcept{}
    
    constexpr PositionN&  operator-=(ES::VectorN<T,N> rhs) noexcept{}

    
};