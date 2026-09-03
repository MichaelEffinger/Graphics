module;

#include <random>
#include <numeric>
#include <ranges>
#include <iostream>
#include <stacktrace>
#include <chrono>
#include <string>

export module ES_easy;

// import std; maybe... one day...

#define NDCR [[nodiscard]] constexpr
#define NDAO [[nodiscard]] auto
#define NDCRAO [[nodiscard]] constexpr auto


/**
 *@namespace ES::easy Your one-stop-shop for hard things that require thought turned one line!
 */
export namespace ES::easy {

//----------------- Easy random! --------------------
    template<typename L, typename  R>
    [[nodiscard]] std::common_type_t<L,R> random(L low, R high);

    /**
     * Generates a value from [0, High] if integral, or [0, High) if floating point. (Blame the committee for that one.)
     * @param High the upper bound.
     * @return A value from [0, High] if integral, or [0, High) if floating point.
     */
    template<typename L>
    [[nodiscard]] L random(L High);

    template<std::floating_point F, std::size_t Bits>
    [[nodiscard]] F random();

    template<typename  L, typename R>
    [[nodiscard]] auto random_seeded_callable(std::uint32_t, L, R);

    template<typename  L>
    [[nodiscard]] auto random_seeded_callable(std::uint32_t, L);

    template<std::floating_point F, std::size_t Bits>
    [[nodiscard]] auto random_seeded_callable(std::uint32_t);

    template<typename T>
    [[nodiscard]] bool coin_flip(T odds_of_heads = 0.5);

//------------------ Easy ranges! ------------------------
    template<std::ranges::range R>
    NDCRAO sum_range(R &&);

    template<std::ranges::range R>
    void shuffle(R&&);

    /**
     * Step right up! Get yourself an element from this range at random!
     * @tparam R a range, specifically an object with begin() and end().
     * @return a random object in the range
     */
    template<std::ranges::range R>
    [[nodiscard]] decltype(auto) raffle(R&);

    /**
     * Finds the min and max element of a non-empty range and shoves them into a pair.
     * @return A pair of (min, max)
     * @throws std::out_of_range if the array is empty.
     */
    template<std::ranges::range R>
    NDCRAO min_max(R &r);

//------------------ DEBUG ----------------
    void snap_stacktrace(std::ostream &where_to_print = std::cerr, const std::stacktrace& trace = std::stacktrace::current());

    /**
     * Easy timer that takes a function and returns how many seconds it took.
     * @tparam func The callable to be tested.
     * @tparam Args The (optional) args to send to the callable.
     * @return The time, in seconds, it took for this function to complete.
     */
    template<std::invocable func, typename... Args>
    [[nodiscard]] double time_it(func&&, Args&&...);

//----------------- FORMATTING CHARACTERS ---------------
    template<std::ranges::range R>
    NDCRAO capitalize_range(R&&);

    template<std::ranges::range R>
    constexpr R& capitalize_range_in_place(R &);

    template<std::ranges::range R>
    NDCRAO lowercase_range(R&&);

    template<std::ranges::range R>
    constexpr R& lowercase_range_in_place(R &);

    template<std::ranges::range R>
    NDCRAO trim_whitespace(R&&);

    template<std::ranges::range R>
    constexpr R& trim_whitespace_in_place(R&);


}

//----------------------- impl? detail? Secret! ---------------------
namespace ES::easy::Secret {
    using quick_engine = std::minstd_rand;
    std::minstd_rand &get_quick_engine() {
        thread_local quick_engine eng{std::random_device()()};
        return eng;
    }

    template<typename L, typename R>
    using uniform_dist = std::conditional_t<std::is_integral_v<std::common_type_t<L, R>>, std::uniform_int_distribution<std::common_type_t<L, R>>, std::uniform_real_distribution<std::common_type_t<L, R>>>;

    constexpr auto to_upper = [](const unsigned char c){return static_cast<char>(std::toupper(c));};
    constexpr auto to_lower = [](const unsigned char c){return static_cast<char>(std::tolower(c));};
    constexpr auto is_whitespace = [](const unsigned char c){return static_cast<bool>(std::isspace(c));};
}

//--------------------DEFINITIONS------------------------

template<typename L, typename R>
std::common_type_t<L, R> ES::easy::random(const L low, const R high) {
    auto dist = low <= high ? Secret::uniform_dist<L,R>(low, high) : Secret::uniform_dist<L,R>(high, low);
    return dist(Secret::get_quick_engine());
}

template<typename L>
L ES::easy::random(const L High) {
    return random(L{0}, High);
}

template<std::floating_point F = float, std::size_t Bits = std::numeric_limits<F>::digits>
F ES::easy::random() {
    return std::generate_canonical<F, Bits>(Secret::get_quick_engine());
}

template<typename L, typename R>
auto ES::easy::random_seeded_callable(std::uint32_t seed, L min, R max) {
    return [
                eng = Secret::quick_engine(seed),
                dist = Secret::uniform_dist<L,R> (min, max)
        ] mutable {
        return dist(eng);
    };
}

template<typename L>
auto ES::easy::random_seeded_callable(std::uint32_t seed, L max) {
    return random_seeded_callable(seed, L{0}, max);
}

template<std::floating_point F = float, std::size_t Bits = std::numeric_limits<F>::digits>
auto ES::easy::random_seeded_callable(std::uint32_t seed) {
    return [
                eng = Secret::quick_engine(seed)
        ] mutable {
        return std::generate_canonical<F, Bits>(eng);
    };
}

template<typename T>
bool ES::easy::coin_flip(T const odds_of_heads) {
    std::bernoulli_distribution dist(odds_of_heads);
    return dist(Secret::get_quick_engine());
}

template<std::ranges::range R>
constexpr auto ES::easy::sum_range(R &&arr) {
    return std::reduce(std::ranges::begin(arr), std::ranges::end(arr));
}

template<std::ranges::range R>
constexpr auto ES::easy::min_max(R &r) {
    if (std::ranges::empty(r)) throw std::out_of_range("ES::easy::min_max(), range must not be empty!");
    const auto retval = std::ranges::minmax_element(r);
    return std::pair{*retval.min, *retval.max};
}


void ES::easy::snap_stacktrace(std::ostream &where_to_print, const std::stacktrace& trace) {
    where_to_print << std::to_string(trace);
}

template<std::ranges::range R>
void ES::easy::shuffle(R && arr) {
    std::shuffle(std::ranges::begin(arr), std::ranges::end(arr), Secret::get_quick_engine());
}

template<std::ranges::range R>
decltype(auto) ES::easy::raffle(R &r) {
    if constexpr (std::ranges::random_access_range<R>){
        if (std::ranges::empty(r)) throw std::out_of_range("ES::easy::raffle(), we can't throw a raffle with an empty range!");
        const auto size = std::ranges::distance(r);
        return r[ES::easy::random(size - decltype(size){1})];
    } else {
        std::ranges::range_value_t<R> chosen{};
        std::size_t count = 0;
        for (auto&& element : r) {
            ++count;
            if (ES::easy::random(count - 1uz) == 0) chosen = std::forward<decltype(element)>(element);
        }
        return chosen;
    }
}

template<std::invocable func, typename... Args>
double ES::easy::time_it(func&& f, Args&&... args) {
    auto begin = std::chrono::steady_clock::now();
    (void) f(std::forward<Args>(args)...);
    auto end = std::chrono::steady_clock::now();
    return std::chrono::duration<double>{end-begin}.count();
}

template<std::ranges::range R>
constexpr auto ES::easy::capitalize_range(R &&r) {
    std::remove_cvref_t<R> retval(std::forward<R>(r));
    std::ranges::transform(retval, std::ranges::begin(retval), Secret::to_upper);
    return retval;
}

template<std::ranges::range R>
constexpr R& ES::easy::capitalize_range_in_place(R &r) {
    std::ranges::transform(r, std::ranges::begin(r), Secret::to_upper);
    return r;
}

template<std::ranges::range R>
constexpr auto ES::easy::lowercase_range(R &&r) {
    std::remove_cvref_t<R> retval(std::forward<R>(r));
    std::ranges::transform(retval, std::ranges::begin(retval), Secret::to_lower);
    return retval;
}

template<std::ranges::range R>
constexpr R & ES::easy::lowercase_range_in_place(R &r) {
    std::ranges::transform(r, std::ranges::begin(r), Secret::to_lower);
    return r;
}

template<std::ranges::range R>
constexpr auto ES::easy::trim_whitespace(R &&r) {
    std::remove_cvref_t<R> retval(std::forward<R>(r));
    auto front_end = std::ranges::find_if_not(retval, Secret::is_whitespace);
    retval.erase(std::ranges::begin(retval), front_end);
    auto back_begin = std::ranges::find_last_if_not(retval, Secret::is_whitespace);
    if (not back_begin.empty()) retval.erase(std::ranges::next(back_begin.begin()), std::ranges::end(retval));
    return retval;
}

template<std::ranges::range R>
constexpr R & ES::easy::trim_whitespace_in_place(R & r) {
    return r = ES::easy::trim_whitespace(std::move(r));
}

