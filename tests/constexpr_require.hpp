#pragma once

#define CONSTEXPR_REQUIRE(...)                                               \
    do {                                                                     \
        if (!(__VA_ARGS__)) {                                                \
            throw "CONSTEXPR_REQUIRE failed: " #__VA_ARGS__;                 \
        }                                                                    \
    } while (0)


