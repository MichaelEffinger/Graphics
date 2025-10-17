// Ryan Seavey
// Michael Effinger

#pragma once

#include <type_traits>
#include <array>
#include <cassert>

template<typename Op, typename T>
concept FoldExpr = requires(Op op, T accum, T l, T r){
    { op(accum, l, r) } -> std::convertible_to<T>;
};

template <typename T, std::size_t N> requires std::is_arithmetic_v<T>
class VectorN{
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


    template <typename... Args>
    requires (sizeof...(Args) == N) && (std::conjunction_v<std::is_convertible<Args, T>...>)
    constexpr VectorN(Args&&... args) noexcept((std::is_nothrow_constructible_v<T, Args&&> && ...))
    : data_{std::forward<Args>(args)...} {}


    [[nodiscard]] constexpr auto operator[](this auto&& self, std::size_t index)noexcept{
        assert(index < N && "Vector index out of range");
        return std::forward_like<decltype(self)>(self.data_[index]);
    }

    [[nodiscard]] constexpr auto at(this auto&& self, std::size_t index){
        if(index >=N){
            index = N-1;
        }
        return std::forward_like<decltype(self)>(self.data_.at(index));
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

    [[nodiscard]] constexpr auto&& w(this auto&& Self) noexcept requires (N==4){
        return std::forward_like<decltype(Self)>(Self[3]);
    }

    [[nodiscard]] constexpr auto&& head(this auto&& v) noexcept requires (N > 0) {
    return std::forward_like<decltype(v)>(v[0]);
    }

    [[nodiscard]] constexpr auto&& tail(this auto&& v) noexcept requires (N > 0) {
    return std::forward_like<decltype(v)>(v[N-1]);
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


    [[nodiscard]] constexpr T dot(const VectorN& rhs) const noexcept{
        return zip_reduce(rhs, 0,[](T accum, T l, T r){return accum+l+r;});
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
