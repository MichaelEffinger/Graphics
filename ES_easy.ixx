module;

#include <random>


export module ES_easy;


/**
 *@namespace ES::easy Your one-stop-shop for hard things that require thought turned one line!
 */
export namespace ES::easy {
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

}

namespace ES::easy::Secret {
    inline std::minstd_rand &get_quick_engine();

    using quick_engine = std::minstd_rand;

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

std::minstd_rand& ES::easy::Secret::get_quick_engine() {
    thread_local quick_engine eng{std::random_device()()};
    return eng;
}


