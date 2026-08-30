module;

#include <random>
#include <numeric>
#include <ranges>
#include <iostream>
#include <stacktrace>
#include <chrono>

export module ES_easy;

#define NDCR [[nodiscard]] constexpr
#define NDAO [[nodiscard]] auto
#define NDCRAO [[nodiscard]] constexpr auto


/**
 *@namespace ES::easy Your one-stop-shop for hard things that require thought turned one line!
 */
export namespace ES::easy {
    ///////////////////
    ///Easy random!
    //////////////////
    template<typename L, typename  R>
    [[nodiscard]] std::common_type_t<L,R> random(L low, R high);

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

    ///////////////////
    ///Easy ranges!
    //////////////////
    template<std::ranges::range R>
    NDCRAO sum_range(R &&);

    template<std::ranges::range R>
    void shuffle(R&&);

    ///////////////////
    ///Easy debug!
    //////////////////
    void snap_stacktrace(std::ostream &where_to_print = std::cerr, const std::stacktrace& trace = std::stacktrace::current());

    /**
     * Easy timer that takes a function and returns how many seconds it took.
     * @tparam func The callable to be tested.
     * @tparam Args The (optional) args to send to the callable.
     * @return The time, in seconds, it took for this function to complete.
     */
    template<std::invocable func, typename... Args>
    [[nodiscard]] double time_it(func&&, Args&&...);
}

namespace ES::easy::Secret {
    using quick_engine = std::minstd_rand;
    std::minstd_rand &get_quick_engine() {
        thread_local quick_engine eng{std::random_device()()};
        return eng;
    }

    template<typename L, typename R>
    using uniform_dist = std::conditional_t<std::is_integral_v<std::common_type_t<L, R>>, std::uniform_int_distribution<std::common_type_t<L, R>>, std::uniform_real_distribution<std::common_type_t<L, R>>>;
}

//DEFINITIONS

template<typename L, typename R>
std::common_type_t<L, R> ES::easy::random(const L low, const R high) {
    Secret::uniform_dist<L,R> dist(low, high);
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


void ES::easy::snap_stacktrace(std::ostream &where_to_print, const std::stacktrace& trace) {
    where_to_print << std::to_string(trace);
}

template<std::ranges::range R>
void ES::easy::shuffle(R && arr) {
    std::shuffle(std::ranges::begin(arr), std::ranges::end(arr), Secret::get_quick_engine());
}

template<std::invocable func, typename... Args>
double ES::easy::time_it(func&& f, Args&&... args) {
    auto begin = std::chrono::steady_clock::now();
    (void) f(std::forward<Args>(args)...);
    auto end = std::chrono::steady_clock::now();
    return std::chrono::duration<double>{end-begin}.count();
}

