#include <type_traits>
#include <array>
#include <cassert>
#include <cmath>
#include <algorithm>
#include <functional>
#include "ES_math.hpp"

template <typename T, std::size_t N>
  requires(std::is_arithmetic_v<T>)
class PositionN {
  std::array<T, N> data_{};

public:


  [[nodiscard]] constexpr PositionN operator+(PositionN rhs) const noexcept {}

  constexpr PositionN &operator+=(PositionN rhs) const noexcept {}

  [[nodiscard]] constexpr PositionN operator-(PositionN rhs) const noexcept {}

  [[nodiscard]] constexpr PositionN operator*(PositionN rhs) const noexcept {}

  [[nodiscard]] constexpr PositionN operator/(PositionN rhs) const noexcept {}
};