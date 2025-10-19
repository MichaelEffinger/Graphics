#include <array>
#include <string>
#include <iostream>
#include <functional>
#include <cmath>
#include <numeric>
#include <execution>



template<typename Op, typename T>
concept FoldExpr = requires(Op op, T accum, T l, T r){
    { op(accum, l, r) } -> std::convertible_to<T>;
};


template<typename T, std::size_t N>
class VectorN{
    std::array<T,N> data_;
public:
    //funky STL member types
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

    //Constructors
    constexpr VectorN() noexcept = default;
    constexpr VectorN(auto&&... args) noexcept requires (sizeof...(args) == N)
    : data_{std::forward<decltype(args)>(args)...}{}

    //Utility
    [[nodiscard]] constexpr auto operator[](this auto&& me, std::size_t n) noexcept{
        return std::forward_like<decltype(me)>(me.data_[n]);
    }

    [[nodiscard]] constexpr auto at(this auto&& me, std::size_t n){
        return std::forward_like<decltype(me)>(me.at(n));
    }

    [[nodiscard]] static constexpr std::size_t size(){
        return N;
    }

    [[nodiscard]] constexpr auto&& x(this auto&& me) noexcept requires (N > 0){
        return std::forward_like<decltype(me)>(me[0]);
    }

    [[nodiscard]] constexpr auto&& y(this auto&& me) noexcept requires (N > 1){
        return std::forward_like<decltype(me)>(me[1]);
    } 

    [[nodiscard]] constexpr auto&& z(this auto&& me) noexcept requires (N > 2){
        return std::forward_like<decltype(me)>(me[2]);
    }

    [[nodiscard]] constexpr auto&& w(this auto&& me) noexcept requires (N > 3){
        return std::forward_like<decltype(me)>(me[3]);
    } 

    [[nodiscard]] constexpr auto&& t(this auto&& me) noexcept requires (N > 0){
        constexpr std::size_t index = std::min(N - 1, 4uz);
        return std::forward_like<decltype(me)>(me[index]);
    } 

    //Range
    [[nodiscard]] constexpr auto begin() noexcept{return data_.begin();}
    [[nodiscard]] constexpr auto begin() const noexcept{return data_.begin();}
    [[nodiscard]] constexpr auto cbegin() const noexcept{return data_.cbegin();}
    [[nodiscard]] constexpr auto end() noexcept{return data_.end();}
    [[nodiscard]] constexpr auto end() const noexcept{return data_.end();}
    [[nodiscard]] constexpr auto cend() const noexcept{return data_.cend();}

    //Meta-Operations
    template<typename BinaryOp>
    [[nodiscard]] constexpr VectorN zip(const VectorN& rhs, BinaryOp op) const noexcept{
        VectorN resultant;
        auto liter = cbegin(), riter = rhs.cbegin(), oiter = resultant.begin();
        while(liter != cend()){
            *oiter = op(*liter, *riter);
            ++liter, ++riter, ++oiter;
        }
        return resultant;
    }

    template<typename BinaryOp>
    constexpr VectorN& zip_in_place(const VectorN& rhs, BinaryOp op) noexcept{
        auto liter = begin(), riter = rhs.cbegin();
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

    //Operations
    [[nodiscard]] constexpr T dot(const VectorN& rhs) const noexcept{
        return zip_reduce(rhs, 0, [](T accum, T l, T r){ return accum + l * r; });
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
        return std::sqrt(dot(*this));
    }

    constexpr VectorN& normalize() noexcept{
        T mag = magnitude();
        std::transform(begin(), end(), begin(), [mag](T in){ return in / mag; });
        return *this;
    }

    [[nodiscard]] constexpr VectorN normalize() const noexcept{
        return VectorN{*this}.normalize();
    }

    //Operators
    [[nodiscard]] constexpr VectorN operator+(const VectorN& rhs) const noexcept{
        return zip(rhs, std::plus{});
    }

    constexpr VectorN& operator+=(const VectorN& rhs) noexcept{
        return zip_in_place(rhs, std::plus{});
    }

    [[nodiscard]] constexpr VectorN operator-(const VectorN& rhs) const noexcept{
        return zip(rhs, std::minus{});
    }

    [[nodiscard]] constexpr VectorN operator-() const noexcept{
        VectorN me{*this};
        std::negate<> op;
        for(auto& i : me) i = op(i);
        return me;
    }

    constexpr VectorN& operator-=(const VectorN& rhs) noexcept{
        return zip_in_place(rhs, std::minus{});
    }

    [[nodiscard]] constexpr VectorN operator*(const VectorN& rhs) const noexcept{
        return zip(rhs, std::multiplies{});
    }

    constexpr VectorN& operator*=(const VectorN& rhs) noexcept{
        return zip_in_place(rhs, std::multiplies{});
    }

    [[nodiscard]] constexpr VectorN operator/(const VectorN& rhs) const noexcept{
        return zip(rhs, std::divides{});
    }

    constexpr VectorN& operator/=(const VectorN& rhs) noexcept{
        return zip_in_place(rhs, std::divides{});
    }

    [[nodiscard]] constexpr T operator%(const VectorN& rhs) const noexcept{
        return dot(rhs);
    }

    //Scalar Suite
    [[nodiscard]] constexpr VectorN operator*(const auto& scalar) const noexcept{
        VectorN me{*this};
        for(auto& i : me) i *= scalar;
        return me;
    }

    [[nodiscard]] friend constexpr VectorN operator*(const auto& scalar, const VectorN& lhs) noexcept{
        VectorN me{lhs};
        for(auto& i : me) i *= scalar;
        return me;
    }

    constexpr VectorN& operator*=(const auto& scalar) noexcept{
        for(auto& i : *this) i *= scalar;
        return *this;
    }

    [[nodiscard]] constexpr VectorN operator/(const auto& scalar) const noexcept{
        VectorN me{*this};
        for(auto& i : me) i /= scalar;
        return me;
    }

    [[nodiscard]] friend constexpr VectorN operator/(const auto& scalar, const VectorN& lhs) noexcept{
        VectorN me{lhs};
        for(auto& i : me) i = scalar/i;
        return me;
    }

    constexpr VectorN& operator/=(const auto& scalar) noexcept{
        for(auto& i : *this) i /= scalar;
        return *this;
    }

    template<int M> requires (M < N)
    constexpr VectorN(const VectorN<T, M>& other);

    template<int M> requires (M > N)
    explicit constexpr VectorN(const VectorN<T, M>& other);

};

template<typename... Args>
VectorN(Args...) -> VectorN<std::common_type_t<Args...>, sizeof...(Args)>;

template <typename T> using Vector2 = VectorN<T, 2>;
template <typename T> using Vector3 = VectorN<T, 3>;
template <typename T> using Vector4 = VectorN<T, 4>;



#ifdef GODBOLT
int main(){
    VectorN vec1{1, 2, 3}, vec2{4,5,6};
    (void)vec2.size();
    vec1 *= vec2;
    return vec1.normalize().cross_in_place(vec2).magnitude();
}
#endif