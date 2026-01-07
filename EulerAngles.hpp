#pragma once
#include "Angle.hpp"
#include "ContainerN.hpp"
#include "ArithmeticOpsMixin.hpp"

namespace ES{
    template <ES::radian_or_degree Unit, typename T>
    class EulerAngles : public ContainerN<EulerAngles<Unit,T>,T,3>, public ArithmeticOpsMixin<EulerAngles<Unit,T>, T, 3>{
        static constexpr void can_scalar_multiply(){return;}
        static constexpr void can_scalar_divide(){return;}
        static constexpr void can_component_add(){return;}
        static constexpr void can_component_subtract(){return;}
        static constexpr void can_clamp(){return;}
        

    };


}