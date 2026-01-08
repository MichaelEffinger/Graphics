#pragma once
#include "Angle.hpp"
#include "ContainerN.hpp"
#include "ArithmeticOpsMixin.hpp"

namespace ES{
    template <ES::radian_or_degree Unit, typename T>
    class EulerAngles : public ContainerN<EulerAngles<Unit,T>,Angle<Unit,T>,3>, public ArithmeticOpsMixin<EulerAngles<Unit,T>, Angle<Unit,T>,3>{
        
        public:
        using ContainerN<EulerAngles,Angle<Unit,T>,3>::zip_in_place;
        using ContainerN<EulerAngles,Angle<Unit,T>,3>::zip;
        using ContainerN<EulerAngles,Angle<Unit,T>,3>::zip_reduce;
        using ContainerN<EulerAngles,Angle<Unit,T>,3>::begin;
        using ContainerN<EulerAngles,Angle<Unit,T>,3>::end;
        using ContainerN<EulerAngles,Angle<Unit,T>,3>::cend;
        using ContainerN<EulerAngles,Angle<Unit,T>,3>::data;
        using ContainerN<EulerAngles,Angle<Unit,T>,3>::cbegin;
        using ContainerN<EulerAngles,Angle<Unit,T>,3>::ContainerN;

        static constexpr void can_scalar_multiply(){return;}
        static constexpr void can_scalar_divide(){return;}
        static constexpr void can_component_add(){return;}
        static constexpr void can_component_subtract(){return;}
        //naive lerp
        static constexpr void can_lerp(){return;}
        static constexpr void can_clamp(){return;}
        static constexpr void can_negate(){return;}



        [[nodiscard]] constexpr auto&& yaw(this auto&& self) noexcept{
            return std::forward_like<decltype(self)>(self[0]);
        }

        [[nodiscard]] constexpr auto&& pitch(this auto&& self) noexcept{
            return std::forward_like<decltype(self)>(self[1]);
        }
        
        [[nodiscard]] constexpr auto&& roll(this auto&& self) noexcept{
            return std::forward_like<decltype(self)>(self[2]);
        }

        constexpr EulerAngles() noexcept{
            std::fill(begin(),end(), 0);
        }

        constexpr EulerAngles(Angle<Unit,T> y,Angle<Unit,T> p, Angle<Unit,T> r){
            yaw() = y;
            pitch() = p;
            roll() = r;
        }

        
        [[nodiscard]] constexpr EulerAngles normalize() const noexcept{
            EulerAngles temp(*this); 
            temp.yaw().wrap_to(-ES::math::pi<T>,ES::math::pi<T>);
            temp.pitch().wrap_to(-ES::math::pi<T>,ES::math::pi<T>);
            temp.roll().wrap_to(-ES::math::pi<T>,ES::math::pi<T>);
            return temp;
        }
        constexpr EulerAngles& normalize_in_place() noexcept{
            yaw().wrap_to_in_place(-ES::math::pi<T>,ES::math::pi<T>);
            pitch().wrap_to_in_place(-ES::math::pi<T>,ES::math::pi<T>);
            roll().wrap_to_in_place(-ES::math::pi<T>,ES::math::pi<T>);
            return *this;
        }

        [[nodiscard]] constexpr EulerAngles canonicalize() const noexcept{
            EulerAngles temp = normalize();
            temp.pitch().wrap_to_in_place(-ES::math::half_pi<T>,ES::math::half_pi<T>);
            return temp;
        }
        constexpr EulerAngles& canonicalize_in_place() noexcept{
            normalize_in_place();
            pitch().wrap_to_in_place(-ES::math::half_pi<T>,ES::math::half_pi<T>);
            return *this;
        }

        [[nodiscard]] constexpr T sin_yaw()  const noexcept{
            return std::sin(yaw().get());
        }
        [[nodiscard]] constexpr T cos_yaw()  const noexcept{
            return std::cos(yaw().get());
        }
        [[nodiscard]] constexpr T sin_pitch() const noexcept {
            return std::sin(pitch().get());
        }
        [[nodiscard]] constexpr T cos_pitch() const noexcept {
            return std::cos(pitch().get());
        }
        [[nodiscard]] constexpr T sin_roll() const noexcept{
            return std::sin(roll().get());
        }
        [[nodiscard]] constexpr T cos_roll() const noexcept{
            return std::cos(roll().get());
        }


    };


}