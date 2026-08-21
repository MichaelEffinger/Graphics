#pragma once
#include <concepts>

namespace ES::concepts{

    template<typename Op, typename T>
    concept FoldExpr = requires(Op op, T accum, T l, T r){
        { op(accum, l, r) } -> std::convertible_to<T>;
    };
        
    template<typename C>
    concept Iterable = requires(C c) {
        c.begin();
        c.end();
        c.cbegin();
        c.cend();
    };

    template<typename T>
    concept CheapToCopy = std::is_trivially_copy_constructible_v<T> && std::is_trivially_destructible_v<T> && sizeof(T) <= 2*sizeof(void*);

}