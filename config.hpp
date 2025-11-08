#pragma once
#include <cstdint>

namespace ES{
   using real = float;
   using angle = float;
   using Whole = uint32_t;
   using Index = std::size_t;



   
   
   template<typename T>
   struct NumericTraits;
   template<>
   struct NumericTraits<real> {
       static constexpr real epsilon = 1e-6f;
       static constexpr real minSafe = 1e-30f;  // e.g., for normalization
       static constexpr real maxSafe = 1e30f;   // optional
   };




}


