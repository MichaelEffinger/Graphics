//
// Created by Comedic Genuis on 10/22/2025.
//

#ifndef COMPUTERGRAPHICS_ESRANDOM_HPP
#define COMPUTERGRAPHICS_ESRANDOM_HPP

#include <random>

//SIGNATURES AND FRIENDS
namespace ES::random {
    template<typename L, typename  R>
    [[nodiscard]] std::common_type_t<L,R> easy(L low, R high);

    template<typename L>
    [[nodiscard]] L easy(L High);

    template<std::floating_point F, std::size_t Bits>
    [[nodiscard]] F easy();
}

namespace ES::random::Secret {
    inline std::minstd_rand &get_quick_engine();

    template<typename L, typename R>
    using uniform_dist = std::conditional_t<std::is_integral_v<std::common_type_t<L, R>>, std::uniform_int_distribution<std::common_type_t<L, R>>, std::uniform_real_distribution<std::common_type_t<L, R>>>;
}

//DEFINITIONS

template<typename L, typename R>
std::common_type_t<L, R> ES::random::easy(const L low, const R high) {
    Secret::uniform_dist<L,R> dist(low, high);
    return dist(Secret::get_quick_engine());
}

template<typename L>
L ES::random::easy(const L High) {
    return easy(L{0}, High);
}

template<std::floating_point F = float, std::size_t Bits = std::numeric_limits<F>::digits>
F ES::random::easy() {
    return std::generate_canonical<F, Bits>(Secret::get_quick_engine());
}


std::minstd_rand& ES::random::Secret::get_quick_engine() {
    thread_local std::minstd_rand eng{std::random_device()()};
    return eng;
}








#endif //COMPUTERGRAPHICS_ESRANDOM_HPP
