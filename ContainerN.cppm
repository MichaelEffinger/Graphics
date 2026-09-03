//Michael Effinger
//Ryan Seavey
module;

#include <cstddef>
#include <type_traits>
#include <array>
#include <cassert>
#include <algorithm>

export module ES.ContainerN;

import ES.concepts;
import ES.meta;

export namespace ES{
    //hmmmmm. this is curious 
    template<class Child, typename T, std::size_t N>
    class ContainerN{
    protected:
    
    
    public:

        std::array<T, N> data_ = {};
        using ContainerThis = ContainerN<Child,T,N>;
        using value_type = T;
        using size_type = std::size_t;
        using reference = value_type&;
        using const_reference = const value_type&;
        using pointer = value_type*;
        using const_pointer = const value_type *;
        using iterator = typename std::array<T,N>::iterator;
        using const_iterator = typename std::array<T,N>::const_iterator;
        using reverse_iterator = typename std::array<T,N>::reverse_iterator;
        using const_reverse_iterator = typename std::array<T,N>::const_reverse_iterator;

        [[nodiscard]] constexpr auto begin() noexcept { return data_.begin(); }
        [[nodiscard]] constexpr auto begin() const noexcept { return data_.begin(); }
        [[nodiscard]] constexpr auto cbegin() const noexcept { return data_.cbegin(); }
        [[nodiscard]] constexpr auto end() noexcept { return data_.end(); }
        [[nodiscard]] constexpr auto end() const noexcept { return data_.end(); }
        [[nodiscard]] constexpr auto cend() const noexcept { return data_.cend(); }

        template <typename... Args> requires (sizeof...(Args) == N)
        constexpr ContainerN(Args&&... args) noexcept((std::is_nothrow_constructible_v<T, Args&&> && ...)) : data_{static_cast<T>(args)...} {}
        
        constexpr ContainerN() noexcept = default;

        [[nodiscard]] constexpr T& operator[](std::size_t i) & noexcept {
            assert(i < N);
            return data_[i];
        }

        [[nodiscard]] constexpr const T& operator[](std::size_t i) const & noexcept {
            assert(i < N);
            return data_[i];
        }

        [[nodiscard]] constexpr T&& operator[](std::size_t i) && noexcept {
            assert(i < N);
            return std::move(data_[i]);
        }

        [[nodiscard]] constexpr const T&& operator[](std::size_t i) const && noexcept {
            assert(i < N);
            return std::move(data_[i]);
        }

        [[nodiscard]] constexpr auto& data() & noexcept{
            return data_;
        }

        [[nodiscard]] constexpr const auto& data() const & noexcept{
            return data_;
        }

        [[nodiscard]] constexpr auto&& data() && noexcept {
            return std::move(data_);
        }

        [[nodiscard]] constexpr const auto&& data() const && noexcept{
            return std::move(data_);
        }

        [[nodiscard]] constexpr auto& at(std::size_t index) & noexcept {
            assert(index < N && "Container index out of range");
            if (index >= N) {
                index = N - 1;
            }
            return data_[index];
        }

        [[nodiscard]] constexpr const auto& at(std::size_t index) const & noexcept {
            assert(index < N && "Container index out of range");
            if (index >= N) {
                index = N - 1;
            }
            return data_[index];
        }

        [[nodiscard]] constexpr auto&& at(std::size_t index) && noexcept {
            assert(index < N && "Container index out of range");
            if (index >= N) {
                index = N - 1;
            }
            return std::move(data_[index]);
        }


        [[nodiscard]] constexpr const auto&& at(std::size_t index) const && noexcept {
            assert(index < N && "Container index out of range");
            if (index >= N) {
                index = N - 1;
            }
            return std::move(data_[index]);
        }

        [[nodiscard]] static constexpr std::size_t size()noexcept{
            return N;
        }

        [[nodiscard]] constexpr auto& head() & noexcept requires (N > 0) {
            return (*this)[0];
        }

        [[nodiscard]] constexpr const auto& head() const & noexcept requires (N > 0) {
            return (*this)[0];
        }

        [[nodiscard]] constexpr auto&& head() && noexcept requires (N > 0) {
            return std::move((*this)[0]);
        }

        [[nodiscard]] constexpr const auto&& head() const && noexcept requires (N > 0) {
            return std::move((*this)[0]);
        }

        [[nodiscard]] constexpr auto& tail() & noexcept requires (N > 0) {
            return (*this)[N - 1];
        }

        [[nodiscard]] constexpr const auto& tail() const & noexcept requires (N > 0) {
            return (*this)[N - 1];
        }

        [[nodiscard]] constexpr auto&& tail() && noexcept requires (N > 0) {
            return std::move((*this)[N - 1]);
        }

        [[nodiscard]] constexpr const auto&& tail() const && noexcept requires (N > 0) {
            return std::move((*this)[N - 1]);
        }

        template<typename U = Child, typename BinaryOp>
        [[nodiscard]] constexpr Child zip(meta::const_pass_t<U> rhs, BinaryOp op) const noexcept {
            Child resultant;
            auto liter = cbegin(), riter = rhs.cbegin();
            auto oiter = resultant.begin();
            while(liter != cend()){
                *oiter = op(*liter, *riter);
                ++liter, ++riter, ++oiter;
            }
            return resultant;
        }

        template<typename U = Child,typename BinaryOp>
        constexpr Child& zip_in_place(meta::const_pass_t<U> rhs, BinaryOp op) noexcept {
            auto liter = begin();
            auto riter = rhs.cbegin();
            while(liter != end()){
                *liter = op(*liter, *riter);
                ++liter, ++riter;
            }
            return static_cast<Child&>(*this);
            
        }

        template <typename U = Child>
        [[nodiscard]] constexpr bool operator==(meta::const_pass_t<U> other)const noexcept{
            return data() == other.data();
        }

        template <typename U = Child>
        [[nodiscard]] constexpr bool operator!=(meta::const_pass_t<U> other)const noexcept{
           return !operator==(other);
        }

        template <typename U = Child>
        [[nodiscard]] constexpr bool almost_equal(meta::const_pass_t<U> rhs, T epsilon = ES::math::default_epsilon<T>::value) const noexcept{
            for (std::size_t i = 0; i < N; ++i) {
            if (!math::approx_equal(data_[i], rhs[i], epsilon))
                return false;
            }
            return true;
        }

        template <typename U = Child>
        [[nodiscard]] constexpr T zip_reduce(meta::const_pass_t<U> rhs, T initial, ES::concepts::FoldExpr<T> auto&& exp) const noexcept {
            auto liter = cbegin(), riter = rhs.cbegin();
            while(liter != cend()){
                initial = exp(initial, *liter, *riter);
                ++liter, ++riter;
            }
            return initial;
        }


    };
}