module; 
//
// Created by Ryan Seavey on 8/27/2026.
// Other code scrapped out of original header made by Michael Effinger & Ryan Seavey throughout the year
//

#include <type_traits>
#include <compare>
#include <stdexcept>
#include <cmath>
#include <limits> //TODO: in house? To what end? I cry.

export module ES_math;

import ES.concepts;
#define NDCR [[nodiscard]] constexpr

export namespace ES::math {
    /**
     * An exception type made to represent when IEE754 (inevitably) fails us.
     * This ought to be thrown when an operation fails plainly due to the current floating point type running out of precision.
     */
    struct precision_exhausted : std::logic_error {
        using std::logic_error::logic_error;
    };

    /**
     * Technically a spaceship operator tester under the hood.
     * @return True if the number is ±NaN, False otherwise.
     */
    template<typename T>
    NDCR bool is_NaN(const T N) noexcept {
        return N <=> static_cast<T>(0) == std::partial_ordering::unordered;
    }

    /**
     * @note Does not indicate signedness, at all.
     * @return True if N is ±inf, False otherwise.
     */
    template<typename T>
    NDCR bool is_infinity(const T N) noexcept {
        return std::numeric_limits<T>::max() < N
        || std::numeric_limits<T>::lowest() > N;
    }

    /**
    * Literally just `return is_infinity(N) || is_NaN(N);` with a catchy name.
    * @tparam T Hopefully a type that could be hideous.
    * @param N Our (possibly) ugly duckling.
    * @return True the input is ±inf or ±NaN, False otherwise.
    */
    template<typename T>
    NDCR bool is_hideous(const T N) noexcept {
        return is_infinity(N) || is_NaN(N);
    }


    /**
     * Absolute value written by yours truly.
     * @tparam T The type of number you're working with. Template deduction handles this, so no touchy!
     * @param N The numero you want castrated in a signedness sense.
     * @return Your spayed value.
     */
    template<typename T>
    NDCR T abs(const T N) noexcept {
        //Believe it or not, chopping off the sign of an unsigned value does nothing!
        if constexpr(std::is_unsigned_v<T>) return N;
        /*IEEE says -0.0 and 0.0 are different values, yet literally every operation treats them as the same.
         * However, math.h's abs requires -0.0 becomes 0.0, yet I cannot test for that!
         * So, if the value is zero (because == of 0.0 and -0.0 is true) then I choose the safe bet and just overwrite both with 0.0.
         */
        if constexpr(std::is_floating_point_v<T>) if (N == static_cast<T>(0)) return static_cast<T>(0);
        return N < 0 ? -N : N;
    }

    /**
     * I wanna heal. I wanna feel...
     * @tparam T The type you would love rounded.
     * @param N The number you would love rounded.
     * @return Take a wild guess.
     */
    template<typename T>
    NDCR T round(const T N) noexcept {
        if constexpr (std::is_integral_v<T>) return N;
        if (is_hideous(N)) return N;
        if (N == static_cast<T>(0)) return N;
        const T neg = N < 0 ? -1 : 1;
        if (N + neg * std::numeric_limits<T>::round_error() == N) return N;
        return static_cast<T>(static_cast<long long>(N + neg * static_cast<T>(0.5)));
    }

    /**
     * A very stupid, O(n) implementation of exponents. I am not winning any awards here.
     * Only accepts arguments that are numbers.
     * Only accepts exponents that are whole and non-negative.
     * I repeat, all exponents must be from the set 𝕎.
     * @tparam T The base's type.
     * @tparam U The exponent's type.
     * @param base The x in x to the power of y. Must be a number.
     * @param exp The y in x to the power of y. Must be whole and non-negative.
     * @return base to the power of exp.
     *
     * @throw std::invalid_argument if you input a negative or decimal as the exponent.
     * @throw std::invalid_argument if either (or both) arguments are not numbers (keep in mind: infinity is not a number).
     * @throw ES::math::precision_exhausted if the operation cannot continue as the exponent is so large it cannot be decremented.
     */
    template<typename T, typename U>
    NDCR T pow(const T base, U exp){
        if (round(exp) != exp) throw std::invalid_argument("ES::math::pow(), `exp` must be a whole number.");
        if (round(exp) < static_cast<U>(0)) throw std::invalid_argument("ES::math::pow(), `exp` must be non-negative.");
        if (is_hideous(base) || is_hideous(exp)) throw std::invalid_argument("ES::math::pow(), an argument (or both) must be a number.");
        if constexpr (not std::is_integral_v<U>) if (exp - 1 == exp) throw precision_exhausted("ES::math::pow(), `exp` must decrement even in extremis. (Presumably, it is so large that it has run out of precision.)");
        if (exp == static_cast<U>(0)) return static_cast<T>(1); //x^0 where the result is always one.
        if (exp == static_cast<U>(1)) return base; //x^1 where the result is always x. Technically redundant in this function, oh well.
        if (base == static_cast<T>(0) || base == static_cast<T>(1)) return base; //0^n or 1^n always is just itself.

        T retval{base};
        for(; exp >= static_cast<U>(2); --exp) retval *= base;
        return retval;
    }

    //for later if an end user ever actually desires a non-negative exponential accepting function and uses it whereupon I update this and something breaks.
    template<typename T, typename U>
    NDCR T stupid_pow(const T base, U exp) {
        return pow<T, U>(base, exp);
    }


    template <typename T> struct default_epsilon;

    template <> struct default_epsilon<float> {
        static constexpr float value = 1e-5f;
    };
    template <> struct default_epsilon<double> {
        static constexpr double value = 1e-9;
    };

    template <> struct default_epsilon<long double> {
        static constexpr long double value = 1e-12L;
    };

    template <std::integral T> struct default_epsilon<T> {
        static constexpr T value = 0;
    };

    template <typename T>
    inline constexpr T constexpr_abs(T value) noexcept {
        return value < T{0} ? -value : value;
    }

    template <typename T, typename H>
    inline constexpr bool approx_equal(T lhs, H rhs, T epsilon = default_epsilon<T>::value) noexcept {
        return constexpr_abs(lhs - static_cast<T>(rhs)) <= epsilon;
    }

    template <typename Container1, typename Container2, typename BinaryOp> requires(concepts::Iterable<Container1> && concepts::Iterable<Container2>)
    [[nodiscard]] constexpr Container1 zip(Container1 lhs, Container2 rhs, BinaryOp op) noexcept {
        Container1 resultant;
        auto liter = lhs.cbegin(), riter = rhs.cbegin();
        auto oiter = resultant.begin();
        while (liter != lhs.cend()) {
            *oiter = op(*liter, *riter);
            ++liter, ++riter, ++oiter;
        }
        return resultant;
    }

    template <typename Container1, typename Container2, typename Container3, typename BinaryOp> requires(concepts::Iterable<Container1> && concepts::Iterable<Container2> && concepts::Iterable<Container3>)
    constexpr Container3 &zip_into(const Container1 lhs, const Container2 rhs, Container3& dest, BinaryOp op) noexcept {
        auto liter = lhs.cbegin(), riter = rhs.cbegin();
        auto oiter = dest.begin();
        while (liter != lhs.cend()) {
            *oiter = op(*liter, *riter);
            ++liter, ++riter, ++oiter;
        }
        return dest;
    }


    template <typename Container1, typename Container2, typename BinaryOp, typename T = typename Container1::value_type>
    [[nodiscard]] constexpr T zip_reduce(const Container1 lhs, const Container2 rhs, T initial, concepts::FoldExpr<T> auto&& exp) noexcept {
        auto liter = lhs.cbegin(), riter = rhs.cbegin();
        while(liter != lhs.cend()){
            initial = exp(initial, *liter, *riter);
            ++liter, ++riter;
        }
        return initial;
    }


    template <typename Container1, typename Container2, typename Container3, typename BinaryOp> requires(concepts::Iterable<Container1> && concepts::Iterable<Container2> && concepts::Iterable<Container3>)
    [[nodiscard]] constexpr Container1 tri_zip(Container1 lhs, Container2 mhs, Container3 rhs, BinaryOp op) noexcept {
        Container1 resultant;
        auto liter = lhs.cbegin(), riter = rhs.cbegin(), miter = mhs.cbegin();
        auto oiter = resultant.begin();
        while (liter != lhs.cend()) {
            *oiter = op(*liter, *miter, *riter);
            ++liter, ++miter, ++riter, ++oiter;
        }
        return resultant;
    }

    template<typename T> [[nodiscard]] constexpr T absolute_value(const T N) noexcept {
        if (N == 0) return N; //zero's signedness is important to keep, or so I am told...
        return N > 0 ? N : N * -1;
    }

    template <typename T> concept float_or_double = std::is_same_v<T, float> || std::is_same_v<T, double>;


    /**
     * @brief A VERY well-behaved floating point floor function.
     * @param[in] N that which shall be floored!
     * @tparam F a float or a double...
     */
    template <float_or_double F> [[nodiscard]] constexpr F floor(const F N) noexcept {
        //± 2^23 is where decimals die... fun fact: that immediately prior can support ±0.5. Anything above 2^24 jumps by two from then on...
        static constexpr F NOMORE = 1LL << std::numeric_limits<F>::digits; //So, 2^Mantissa gives the exact point where decimals die.
        if (absolute_value(N) >= NOMORE) return N; //long long is 2^64-1, yet the mantissa peters out at 2^23 or 2^52, so anything greater cannot have a decimal so it cannot floor.
        //positives truncate, which is identical to flooring.
        //Negatives truncate, so -3.9 becomes -3.0 instead of flooring which goes -3.9 -> -4.0.
        //This is why we need to subtract that magic one. So it becomes -3.9 -> -3.0 -> -4.0.
        const F trunk = static_cast<F>(static_cast<long long>(N)); //since we already removed all N > 2^52 or 2^23, we know it is less than 2^64, meaning we're golden to cast like this.
                                                                                                    //as casting to an integral type whose limits are smaller than the current value of a floating point type is UB!
        if (trunk == N) return N; // if we already were an integer, just return.
        return trunk + (trunk < 0) * 1.f; //cheeky boolean math
    }

    /**
     * @brief Well-behaved floating point modulo.
     * @param N Dividend
     * @param M Divisor
     * @return the real number result of N % M
     */
    template <float_or_double F, float_or_double F2> [[nodiscard]] constexpr F modulo(const F N, const F2 M) noexcept {
        return N - M * ES::math::floor(N/M);
    }


    //template values for pi tau and half_pi, just there exact representation, little here little there
    template<typename T> inline constexpr T pi       = T(3.14159265358979323846264338327950288L); 
    template<typename T> inline constexpr T tau      = T(6.28318530717958647692528676655900576L); 
    template<typename T> inline constexpr T half_pi  = T(1.57079632679489661923132169163975144L); 


}
