//Michael Effinger
//Ryan Seavey

#pragma once
#include <cstddef>
#include <type_traits>
#include <array>
#include <cassert>
#include <algorithm>
#include "ES_concepts.hpp"
#include "ES_math.hpp"


namespace ES{
    //hmmmmm. this is curious 
    template<template<typename, std::size_t> class Child, typename T, std::size_t N>
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


        /** @defgroup iterators Iterator Access
        *  @brief Begin/end iterator access for ContainerN.
        *  @{
        */

        /** @brief Returns iterator to first element. See `cbegin` for const iterators. */
        [[nodiscard]] constexpr auto begin() noexcept { return data_.begin(); }
        /** @overload */
        [[nodiscard]] constexpr auto begin() const noexcept { return data_.begin(); }
        /** @overload */
        [[nodiscard]] constexpr auto cbegin() const noexcept { return data_.cbegin(); }

        /** @brief Returns iterator past the last element. See `cend` for const iterators. */
        [[nodiscard]] constexpr auto end() noexcept { return data_.end(); }
        /** @overload */
        [[nodiscard]] constexpr auto end() const noexcept { return data_.end(); }
        /** @overload */
        [[nodiscard]] constexpr auto cend() const noexcept { return data_.cend(); }
        /** @} */

        constexpr ContainerN() noexcept = default;
        /**
        * @brief Constructs a ContainerN from N individual values.
        *
        * This is a variadic template constructor that takes exactly N arguments.
        * Each argument is converted to the Container's element type T using static_cast.
        * ContainerN can be constructed using either `()` or `{}` initialization syntax.
        *
        * @tparam Args Parameter pack representing each element of the Container.
        *         Must have exactly N arguments.
        * @param args The values to initialize the Container elements.
        *
        * @note This constructor is `constexpr` and `noexcept` if all element
        *       constructions are noexcept.
        *
        *  @example
        * ContainerN<float, 3> v(1.0f, 2.0f, 3.0f);  // creates a 3D Container
        */
        template <typename... Args> requires (sizeof...(Args) == N)
        constexpr ContainerN(Args&&... args) noexcept((std::is_nothrow_constructible_v<T, Args&&> && ...)) : data_{static_cast<T>(args)...} {}

        /**
        * @brief Construct a ContainerN form an N-J sized Container along with J other parameters
        * 
        * This is a variadic template contsructor that takes exactly one smaller Container of
        * size N-J along with J other parameters. These together must be of size exactly N.
        * The parameter arguments are static converted to to T.
        *
        * @tparam M The size of ContainerN, which is the first parameter  
        * @tparam U Parameeter pack representing each of the remaining elements. 
        *         Must have exactly size M-N
        * @param smaller The Container of the first values to initialize in the Container
        * @param extras The remaining values to initialize in the Container
        *
        * @note This constructor is `constexpr` and `noexcept` if all elements
        *       if all element construction are noexcept.
        *
        * @example 
        * ContainerN<float,2> v2(1.0f,2.0f);
        * ContainerN<float,5> v5(v2,3.0f,4.0f,5.0f);
        */
        template<size_t M, typename... U> requires (sizeof...(U) == N - M)
        constexpr ContainerN(const Child<T,M>& smaller, U... extras) noexcept((std::is_nothrow_constructible_v<T,U&&>&&...)): data_{} {      
            std::copy(smaller.cbegin(), smaller.cend(), data_.begin());
            size_t index = M;
            ((data_[index++] = T(static_cast<T>(extras))), ...);
        }
        
        /**
        * @brief Provides unchecked element access by index.
        *
        * Uses forwarding to preserve value category (lvalue/rvalue) of the caller.
        * Asserts in debug builds if `index >= N`.
        *
        * @param index Index of the element to access (0-based).
        * @return Reference to the element at the given index, preserving constness/value category.
        */ 
        [[nodiscard]] constexpr T& operator[](std::size_t i) & noexcept {
            assert(i < N);
            return data_[i];
        }
        /** @overload */
        [[nodiscard]] constexpr const T& operator[](std::size_t i) const & noexcept {
            assert(i < N);
            return data_[i];
        }
        /** @overload */
        [[nodiscard]] constexpr T&& operator[](std::size_t i) && noexcept {
            assert(i < N);
            return std::move(data_[i]);
        }
        /** @overload */
        [[nodiscard]] constexpr const T&& operator[](std::size_t i) const && noexcept {
            assert(i < N);
            return std::move(data_[i]);
        }


        /**
        * @brief Provides clamped element access by index.
        *
        * Uses forwarding to preserve value category (lvalue/rvalue) of the caller.
        * Asserts in debug builds if `index >= N`.
        *
        * @param index Index of the element to access (0-based).
        * @return Reference to the element at the given index, preserving constness/value category.
        */ 
        [[nodiscard]] constexpr auto& at(std::size_t index) & noexcept {
            assert(index < N && "Container index out of range");
            if (index >= N) {
                index = N - 1;
            }
            return data_[index];
        }

        /** @overload */
        [[nodiscard]] constexpr const auto& at(std::size_t index) const & noexcept {
            assert(index < N && "Container index out of range");
            if (index >= N) {
                index = N - 1;
            }
            return data_[index];
        }

        /** @overload */
        [[nodiscard]] constexpr auto&& at(std::size_t index) && noexcept {
            assert(index < N && "Container index out of range");
            if (index >= N) {
                index = N - 1;
            }
            return std::move(data_[index]);
        }

        /** @overload */
        [[nodiscard]] constexpr const auto&& at(std::size_t index) const && noexcept {
            assert(index < N && "Container index out of range");
            if (index >= N) {
                index = N - 1;
            }
            return std::move(data_[index]);
        }

        /**
        * @brief Returns the number of elements in the Container.
        * @return Number of elements (compile-time constant N).
        */
        [[nodiscard]] static constexpr std::size_t size()noexcept{
            return N;
        }

        /**
        * @brief Returns the first element (head) of the vector.
        * @return Reference to the first element.
        */
        [[nodiscard]] constexpr auto& head() & noexcept requires (N > 0) {
            return (*this)[0];
        }
        /** @overload */
        [[nodiscard]] constexpr const auto& head() const & noexcept requires (N > 0) {
            return (*this)[0];
        }
        /** @overload */
        [[nodiscard]] constexpr auto&& head() && noexcept requires (N > 0) {
            return std::move((*this)[0]);
        }
        /** @overload */
        [[nodiscard]] constexpr const auto&& head() const && noexcept requires (N > 0) {
            return std::move((*this)[0]);
        }

        /**
        * @brief Returns the last element (tail) of the vector.
        * @return Reference to the last element.
        */
        [[nodiscard]] constexpr auto& tail() & noexcept requires (N > 0) {
            return (*this)[N - 1];
        }
        /** @overload */
        [[nodiscard]] constexpr const auto& tail() const & noexcept requires (N > 0) {
            return (*this)[N - 1];
        }
        /** @overload */
        [[nodiscard]] constexpr auto&& tail() && noexcept requires (N > 0) {
            return std::move((*this)[N - 1]);
        }
        /** @overload */
        [[nodiscard]] constexpr const auto&& tail() const && noexcept requires (N > 0) {
            return std::move((*this)[N - 1]);
        }



        /** @defgroup zip Element-wise Operations
        *  @brief Component-wise operations on VectorN using custom binary functions.
        *  @{
        */

        /**
        * @brief Returns a new vector by applying a binary operation component-wise.
        *
        * Applies `op` to each corresponding element of this vector and `rhs`,
        * producing a new VectorN with the results.
        *
        * @tparam BinaryOp Type of the binary operation (must be callable with `T, T`).
        * @param rhs The vector to combine with this vector.
        * @param op The binary operation to apply element-wise.
        * @return A new VectorN where each element is `op(this[i], rhs[i])`.
        */
        template<typename other, typename BinaryOp>
        [[nodiscard]] constexpr Child<T,N> zip(other rhs, BinaryOp op) const noexcept {
            Child<T,N> resultant;
            auto liter = cbegin(), riter = rhs.cbegin();
            auto oiter = resultant.begin();
            while(liter != cend()){
                *oiter = op(*liter, *riter);
                ++liter, ++riter, ++oiter;
            }
            return resultant;
        }

        /**
        * @brief Applies a binary operation component-wise in-place.
        *
        * Modifies this vector by applying `op` to each corresponding element of
        * this vector and `rhs`.
        *
        * @tparam BinaryOp Type of the binary operation (must be callable with `T, T`).
        * @param rhs The vector to combine with this vector.
        * @param op The binary operation to apply element-wise.
        * @return Reference to this vector after modification.
        */
        template<typename other,typename BinaryOp>
        constexpr Child<T,N>& zip_in_place(const other rhs, BinaryOp op) noexcept {
            auto liter = begin();
            auto riter = rhs.cbegin();
            while(liter != end()){
                *liter = op(*liter, *riter);
                ++liter, ++riter;
            }
            return static_cast<Child<T,N>&>(*this);
            //return reinterpret_cast<Child<T,N>&>(*this);      ///ryan wont let me
            
        }

        /** @brief Equality operator, checks if every component of vector is equal */
        [[nodiscard]] constexpr bool operator==(const ContainerN other)const noexcept{
            if(other.data_ == data_){
                return true;
            }
            return false;
        }
        /** @brief Inequality operator, checks if every component of vector is equal */
        [[nodiscard]] constexpr bool operator!=(const ContainerN other)const noexcept{
           return !operator==(other);
        }

        /**
        * @brief compares two vectors and returns true if there values are at least closer than epsilon
        * 
        * @param rhs the vector you are comparing to
        * @param epsilon how close the values need to be to be true. A default exists for 
        *                multiple floating point type
        * @return true if all values are closer than epsilon
        * @return false if any value is further than epsilon
        */
        [[nodiscard]] bool almost_equal(ContainerN rhs, T epsilon = ES::math::default_epsilon<T>::value)noexcept{
            for (size_t i = 0; i < N; ++i) {
            if (!math::approx_equal(data_[i], rhs[i], epsilon))
                return false;
            }
            return true;
        }

        /**
        * @brief Reduces two vectors into a single value using a ternary fold expression.
        *
        * Iterates over each element of this vector and `rhs`, combining them with
        * `exp` starting from `initial`.
        *
        * @param rhs The vector to combine with this vector.
        * @param initial Initial value for the reduction.
        * @param exp A callable expression of the form `(T accum, T a, T b) -> T`.
        * @return The final accumulated value.
        */
        [[nodiscard]] constexpr T zip_reduce(const ContainerN rhs, T initial, ES::concepts::FoldExpr<T> auto&& exp) const noexcept {
            auto liter = cbegin(), riter = rhs.cbegin();
            while(liter != cend()){
                initial = exp(initial, *liter, *riter);
                ++liter, ++riter;
            }
            return initial;
        }

        /** @} */


    };
}