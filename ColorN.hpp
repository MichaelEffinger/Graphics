#pragma once
#include <type_traits>
#include <cassert>
#include <cmath>
#include <algorithm>
#include <functional>
#include <string>
#include "ContainerN.hpp"
#include <cstdint>
#include "ArithmeticOpsMixin.hpp"


namespace ES{

    template <typename T> struct max_color;

    template <> struct max_color<int16_t> {
        static constexpr int16_t value = 255;
    };

    template <> struct max_color<float>{
        static constexpr float value = 1.0f;
    };




    template<class Child, typename T, std::size_t N>
    class ColorOpsMixin{

    protected:
        constexpr Child& derived() {return static_cast<Child&>(*this);}
        constexpr const Child& derived() const {return static_cast<const Child&>(*this);}
        
    public:

        [[nodiscard]] constexpr Child adjust_brightness(float factor) noexcept requires requires { Child::is_alpha(); } {
            Child temp_col;
            std::transform(derived().begin(),derived().end()-1,temp_col.begin(),[factor](float in){return in * factor;});
            temp_col.tail() = derived().tail();
            return temp_col;
        }

        constexpr Child adjust_brightness_in_place(float factor) noexcept requires requires { Child::is_alpha(); } {
            std::transform(derived().begin(),derived().end()-1,derived().begin(),[factor](float in){return in * factor;});
            return derived();
        }
        
        [[nodiscard]] constexpr Child adjust_brightness(float factor) noexcept requires requires { Child::is_standard(); }{
            Child temp_col;
            std::transform(derived().cbegin(),derived().cend(),temp_col.begin(),[factor](float in){return in * factor;});
            return temp_col;
        }

        constexpr Child adjust_brightness_in_place(float factor) noexcept requires requires { Child::is_standard(); }{
            std::transform(derived().begin(),derived().end(),derived().begin(),[factor](float in){return in * factor;});
            return derived();
        }


        [[nodiscard]] constexpr Child invert() const noexcept requires requires { Child::is_standard(); } {
            Child temp_col;
            std::transform(derived().cbegin(),derived().cend(),temp_col.begin(),[](float in){return  max_color<T>::value-in;});
            return temp_col;
        }

        constexpr Child invert_in_place() noexcept requires requires { Child::is_standard(); }{
            std::transform(derived().begin(),derived().end(),derived().begin(),[](float in){return  max_color<T>::value-in;});
            return derived();
        }

        [[nodiscard]] constexpr Child invert() const noexcept requires requires { Child::is_alpha(); }{
            Child temp_col;
            std::transform(derived().cbegin(),derived().cend()-1,temp_col.begin(),[](float in){return  max_color<T>::value-in;});
            temp_col.tail() = derived().tail();
            return temp_col;
        }

        constexpr Child invert_in_place() noexcept requires requires { Child::is_alpha(); }{
            std::transform(derived().begin(),derived().end()-1,derived().begin(),[](float in){return  max_color<T>::value-in;});
            return derived();
        }

        [[nodiscard]] constexpr Child adjust_saturation(float s) const noexcept requires requires { Child::is_standard(); } {
            T L = derived().luminance()* max_color<T>::value;
            Child L_color;
            std::fill(L_color.begin(),L_color.end(),L);
            return derived().lerp(L_color,1-s);
        }

        constexpr Child adjust_saturation_in_place(float s) noexcept requires requires { Child::is_standard(); }{
            T L = derived().luminance() * max_color<T>::value;
            Child L_color;
            std::fill(L_color.begin(),L_color.end(),L);
            return derived().lerp_in_place(L_color, 1-s);
        }

        [[nodiscard]] constexpr Child adjust_saturation(float s) const noexcept requires requires { Child::is_alpha(); } {
            T L = derived().luminance()* max_color<T>::value;
            Child L_color;
            std::fill(L_color.begin(),L_color.end()-1,L);
            L_color.tail() = derived().tail();
            return lerp(L_color,1-s);
        }

        constexpr Child adjust_saturation_in_place(float s) noexcept requires requires { Child::is_alpha(); }{
            T L = derived().luminance() * max_color<T>::value;
            Child L_color;
            std::fill(L_color.begin(),L_color.end()-1,L);
            L_color.tail() = derived().tail();
            return lerp_in_place(L_color, 1-s);
        }


    };
    
    
    class RGB : public ContainerN<RGB,float,3>, public ColorOpsMixin<RGB,float,3>, public ArithmeticOpsMixin<RGB,float,3> {
        
    public:

        constexpr static void is_standard(){return;};
        constexpr static void can_component_add(){return;};
        constexpr static void can_component_subtract(){return;};
        constexpr static void can_component_multiply(){return;};
        constexpr static void can_component_divide(){return;};
        constexpr static void can_scalar_divide(){return;};
        constexpr static void can_scalar_multiply(){return;};
        constexpr static void can_clamp(){return;};
        constexpr static void can_lerp(){return;};


        using ContainerN<RGB,float,3>::zip_in_place;
        using ContainerN<RGB,float,3>::zip;
        using ContainerN<RGB,float,3>::zip_reduce;
        using ContainerN<RGB,float,3>::begin;
        using ContainerN<RGB,float,3>::end;
        using ContainerN<RGB,float,3>::cend;
        using ContainerN<RGB,float,3>::cbegin;
        using ContainerN<RGB,float,3>::data_;
        using ContainerN<RGB,float,3>::ContainerN;
        using ArithmeticOpsMixin<RGB, float,3>::lerp;
        using ArithmeticOpsMixin<RGB, float,3>::lerp_in_place;



        [[nodiscard]] constexpr auto&& R(this auto&& self) noexcept {
            return std::forward_like<decltype(self)>(self[0]);
        }
        [[nodiscard]] constexpr auto&& G(this auto&& self) noexcept {
            return std::forward_like<decltype(self)>(self[1]);
        }
        [[nodiscard]] constexpr auto&& B(this auto&& self) noexcept{
            return std::forward_like<decltype(self)>(self[2]);
        }

        static constexpr RGB from_srgb(float r, float g, float b) noexcept {
            float R = (r <= 0.04045f) ? r/12.92f : std::pow((r+0.055f)/1.055f, 2.4f);
            float G = (g <= 0.04045f) ? g/12.92f : std::pow((g+0.055f)/1.055f, 2.4f);
            float B = (b <= 0.04045f) ? b/12.92f : std::pow((b+0.055f)/1.055f, 2.4f);

            return RGB(R, G, B);
        } 

        static constexpr RGB from_srgb(int r, int g, int b) noexcept {
            return RGB::from_srgb(r/255.0f,g/255.0f,b/255.0f);
        }

        static constexpr RGB from_hexRGB(const uint32_t hex) noexcept{
            float sR = ((hex >> 16) & 0xFF) / 255.0f;
            float sG = ((hex >> 8)  & 0xFF) / 255.0f;
            float sB = (hex & 0xFF) / 255.0f;

            float R = (sR <= 0.04045f) ? sR/12.92f : std::pow((sR+0.055f)/1.055f, 2.4f);
            float G = (sG <= 0.04045f) ? sG/12.92f : std::pow((sG+0.055f)/1.055f, 2.4f);
            float B = (sB <= 0.04045f) ? sB/12.92f : std::pow((sB+0.055f)/1.055f, 2.4f);
            return RGB(R,G,B);
        }  

        static constexpr RGB from_hexBGR(const uint32_t hex) noexcept{
            float sB = ((hex >> 16) & 0xFF) / 255.0f;
            float sG = ((hex >> 8)  & 0xFF) / 255.0f;
            float sR = (hex & 0xFF) / 255.0f;

            float R = (sR <= 0.04045f) ? sR/12.92f : std::pow((sR+0.055f)/1.055f, 2.4f);
            float G = (sG <= 0.04045f) ? sG/12.92f : std::pow((sG+0.055f)/1.055f, 2.4f);
            float B = (sB <= 0.04045f) ? sB/12.92f : std::pow((sB+0.055f)/1.055f, 2.4f);
            return RGB(R,G,B);
        }

        static constexpr RGB from_linear(float r, float g, float b) noexcept{
            assert(r >= 0.0f && r <= 1.0f); 
            assert(g >= 0.0f && g <= 1.0f);
            assert(b >= 0.0f && b <= 1.0f);
            return RGB(r,g,b);
        }

        static constexpr RGB from_linear(int r, int g, int b)noexcept {
            assert(r >= 0 && r <= 255);
            assert(g >= 0 && g <= 255);
            assert(b >= 0 && b <= 255);
            return RGB(r/255.0f,g/255.0f, b/255.0f);
        }


        [[nodiscard]] constexpr float luminance() const noexcept{
            return (0.2126*R())+(0.7152*G())+(0.0722*B());
        }

        [[nodiscard]] constexpr std::array<float,3> to_srgb() const noexcept{
            std::array<float, 3> SRGB;
            SRGB[0] = (R()<=0.0031308f) ? 12.92f*R() : 1.055f * std::pow(R(), 1.0f / 2.4f) - 0.055f;
            SRGB[1] = (G()<=0.0031308f) ? 12.92f*G() : 1.055f * std::pow(G(), 1.0f / 2.4f) - 0.055f;
            SRGB[2] = (B()<=0.0031308f) ? 12.92f*B() : 1.055f * std::pow(B(), 1.0f / 2.4f) - 0.055f;
            return SRGB;
        }

    };

    class RGB_Int : public ContainerN<RGB_Int,int16_t,3>, public ColorOpsMixin<RGB_Int,int16_t,3>, public ArithmeticOpsMixin<RGB_Int, int16_t, 3>{
      
       
        public:
       
        constexpr static void is_standard(){
            return;
        }
        constexpr static void can_component_add(){return;};
        constexpr static void can_component_subtract(){return;};
        constexpr static void can_scalar_divide(){return;};
        constexpr static void can_scalar_multiply(){return;};
        constexpr static void can_clamp(){return;};
        constexpr static void can_lerp(){return;};

        using ContainerN<RGB_Int,int16_t,3>::zip_in_place;
        using ContainerN<RGB_Int,int16_t,3>::zip;
        using ContainerN<RGB_Int,int16_t,3>::zip_reduce;
        using ContainerN<RGB_Int,int16_t,3>::begin;
        using ContainerN<RGB_Int,int16_t,3>::end;
        using ContainerN<RGB_Int,int16_t,3>::cend;
        using ContainerN<RGB_Int,int16_t,3>::cbegin;
        using ContainerN<RGB_Int,int16_t,3>::data_;
        using ContainerN<RGB_Int,int16_t,3>::ContainerN;


        [[nodiscard]] constexpr auto&& R(this auto&& self) {
            return std::forward_like<decltype(self)>(self[0]);
        }
        [[nodiscard]] constexpr auto&& G(this auto&& self) noexcept {
            return std::forward_like<decltype(self)>(self[1]);
        }
        [[nodiscard]] constexpr auto&& B(this auto&& self) noexcept{
            return std::forward_like<decltype(self)>(self[2]);
        }

        static constexpr RGB_Int from_srgb(float r, float g, float b) noexcept {
            float R = (r <= 0.04045f) ? r/12.92f : std::pow((r+0.055f)/1.055f, 2.4f);
            float G = (g <= 0.04045f) ? g/12.92f : std::pow((g+0.055f)/1.055f, 2.4f);
            float B = (b <= 0.04045f) ? b/12.92f : std::pow((b+0.055f)/1.055f, 2.4f);

            return RGB_Int(
                static_cast<int16_t>(std::round(R*255)),
                static_cast<int16_t>(std::round(G*255)),
                static_cast<int16_t>(std::round(B*255))
            );
        } 

        static constexpr RGB_Int from_srgb(int r, int g, int b) noexcept {
            return RGB_Int::from_srgb(r/255.0f,g/255.0f,b/255.0f);
        }

        static constexpr RGB_Int from_hexRGB(const uint32_t hex) noexcept{
            float sR = ((hex >> 16) & 0xFF) / 255.0f;
            float sG = ((hex >> 8)  & 0xFF) / 255.0f;
            float sB = (hex & 0xFF) / 255.0f;

            float R = (sR <= 0.04045f) ? sR/12.92f : std::pow((sR+0.055f)/1.055f, 2.4f);
            float G = (sG <= 0.04045f) ? sG/12.92f : std::pow((sG+0.055f)/1.055f, 2.4f);
            float B = (sB <= 0.04045f) ? sB/12.92f : std::pow((sB+0.055f)/1.055f, 2.4f);
            return RGB_Int::from_linear(R,G,B);
        }  

        static constexpr RGB_Int from_hexBGR(const uint32_t hex) noexcept{
            float sB = ((hex >> 16) & 0xFF) / 255.0f;
            float sG = ((hex >> 8)  & 0xFF) / 255.0f;
            float sR = (hex & 0xFF) / 255.0f;

            float R = (sR <= 0.04045f) ? sR/12.92f : std::pow((sR+0.055f)/1.055f, 2.4f);
            float G = (sG <= 0.04045f) ? sG/12.92f : std::pow((sG+0.055f)/1.055f, 2.4f);
            float B = (sB <= 0.04045f) ? sB/12.92f : std::pow((sB+0.055f)/1.055f, 2.4f);
            return RGB_Int::from_linear(R,G,B);
        }


        static constexpr RGB_Int from_linear(int r, int g, int b) noexcept{
            assert(r >= 0 && r <= 255); 
            assert(g >= 0 && g <= 255);
            assert(b >= 0 && b <= 255);
            return RGB_Int(r,g,b);
        }


        static constexpr RGB_Int from_linear(float r, float g, float b)noexcept{
            int R = std::round(r*255);
            int G = std::round(g*255);
            int B = std::round(b*255);
            return RGB_Int(R,G,B);
        }

        [[nodiscard]] constexpr float luminance() const noexcept{
            float r = R() /255.0f;
            float g = G() /255.0f;
            float b = B() /255.0f;
            return (0.2126*r)+(0.7152*g)+(0.0722*b);
        }

        [[nodiscard]] constexpr std::array<float,3> to_srgb() const noexcept{
            std::array<float, 3> SRGB;
            float r = R()/255.0f;
            float g = G()/255.0f;
            float b = B()/255.0f;

            SRGB[0] = (r<=0.0031308f) ? 12.92f*r : 1.055f * std::pow(r, 1.0f / 2.4f) - 0.055f;
            SRGB[1] = (g<=0.0031308f) ? 12.92f*g : 1.055f * std::pow(g, 1.0f / 2.4f) - 0.055f;
            SRGB[2] = (b<=0.0031308f) ? 12.92f*b : 1.055f * std::pow(b, 1.0f / 2.4f) - 0.055f;
            return SRGB;
        }
    };

    class RGBA : public ContainerN<RGBA,float, 4>, public ColorOpsMixin<RGBA,float,4>, public ArithmeticOpsMixin<RGBA, float, 4>{
        

        
        public:
        constexpr static void is_alpha(){return;}
        constexpr static void can_component_add(){return;};
        constexpr static void can_component_subtract(){return;};
        constexpr static void can_component_multiply(){return;};
        constexpr static void can_component_divide(){return;};
        constexpr static void can_scalar_divide(){return;};
        constexpr static void can_scalar_multiply(){return;};
        constexpr static void can_clamp(){return;};
        constexpr static void can_lerp(){return;};
        
        using ContainerN<RGBA,float,4>::zip_in_place;
        using ContainerN<RGBA,float,4>::zip;
        using ContainerN<RGBA,float,4>::zip_reduce;
        using ContainerN<RGBA,float,4>::begin;
        using ContainerN<RGBA,float,4>::end;
        using ContainerN<RGBA,float,4>::cend;
        using ContainerN<RGBA,float,4>::cbegin;
        using ContainerN<RGBA,float,4>::data_;
        using ContainerN<RGBA,float,4>::ContainerN;



        [[nodiscard]] constexpr auto&& R(this auto&& self) {
            return std::forward_like<decltype(self)>(self[0]);
        }
        [[nodiscard]] constexpr auto&& G(this auto&& self) noexcept {
            return std::forward_like<decltype(self)>(self[1]);
        }
        [[nodiscard]] constexpr auto&& B(this auto&& self) noexcept{
            return std::forward_like<decltype(self)>(self[2]);
        }
        [[nodiscard]] constexpr auto&& A(this auto&& self) noexcept{
            return std::forward_like<decltype(self)>(self[3]);
        }

        static constexpr RGBA from_srgba(float r, float g, float b, float a) noexcept {
            float R = (r <= 0.04045f) ? r/12.92f : std::pow((r+0.055f)/1.055f, 2.4f);
            float G = (g <= 0.04045f) ? g/12.92f : std::pow((g+0.055f)/1.055f, 2.4f);
            float B = (b <= 0.04045f) ? b/12.92f : std::pow((b+0.055f)/1.055f, 2.4f);

            return RGBA(R*a,G*a,B*a,a);
        } 

        static constexpr RGBA from_srgba(int r, int g, int b,int a) noexcept {
            return RGBA::from_srgba(r/255.0f,g/255.0f,b/255.0f, a/255.0f);
        }

        static constexpr RGBA from_hexRGBA(const uint32_t hex) noexcept{

            float sR = ((hex >> 24) & 0xFF) / 255.0f;
            float sG = ((hex >> 16)  & 0xFF) / 255.0f;
            float sB = ((hex >> 8) & 0xFF) / 255.0f;
            float A = (hex & 0xFF) / 255.0f;

            float R = (sR <= 0.04045f) ? sR/12.92f : std::pow((sR+0.055f)/1.055f, 2.4f);
            float G = (sG <= 0.04045f) ? sG/12.92f : std::pow((sG+0.055f)/1.055f, 2.4f);
            float B = (sB <= 0.04045f) ? sB/12.92f : std::pow((sB+0.055f)/1.055f, 2.4f);

            return RGBA(R*A,G*A,B*A,A);
        }
        


        static constexpr RGBA from_hexBGRA(const uint32_t hex) noexcept{

            float sB = ((hex >> 24) & 0xFF) / 255.0f;
            float sG = ((hex >> 16)  & 0xFF) / 255.0f;
            float sR = ((hex >> 8) & 0xFF) / 255.0f;
            float A = (hex & 0xFF) / 255.0f;

            float R = (sR <= 0.04045f) ? sR/12.92f : std::pow((sR+0.055f)/1.055f, 2.4f);
            float G = (sG <= 0.04045f) ? sG/12.92f : std::pow((sG+0.055f)/1.055f, 2.4f);
            float B = (sB <= 0.04045f) ? sB/12.92f : std::pow((sB+0.055f)/1.055f, 2.4f);

            return RGBA(R*A,G*A,B*A,A);
        }
        



        static constexpr RGBA from_straight_linear(float r, float g, float b, float a) noexcept{
            float R = r * a;
            float G = g * a;
            float B = b * a;
            assert(r >= 0.0f && r <= 1.0f);
            assert(g >= 0.0f && g <= 1.0f);
            assert(b >= 0.0f && b <= 1.0f);
            assert(a >= 0.0f && a <= 1.0f);
            return RGBA(R, G, B, a);
        }
        static constexpr RGBA from_straight_linear(int r, int g, int b, int a) noexcept{
            return RGBA::from_straight_linear(r/255.0f,g/255.0f ,b/255.0f ,a/255.0f);
        }
        static constexpr RGBA from_premultiplied_linear(float r, float g, float b, float a)noexcept{
            assert(r >= 0.0f && r <= 1.0f); 
            assert(g >= 0.0f && g <= 1.0f);
            assert(b >= 0.0f && b <= 1.0f);
            assert(a >= 0.0f && a <= 1.0f);
            return RGBA(r,g,b,a);
        }

        static constexpr RGBA from_premultiplied_linear(int r, int g, int b, int a)noexcept{
            return RGBA::from_premultiplied_linear(r/255.0f,g/255.0f,b/255.0f,a/255.0f);
        }

        [[nodiscard]] constexpr float luminance() const noexcept{
            if(A() ==0.0f){
                return 0.0f;;
            }
            return ((0.2126*R())+(0.7152*G())+(0.0722*B()))/A();
        }

        [[nodiscard]] constexpr std::array<float,4> to_srgba() const noexcept{
            std::array<float, 4> SRGBA;

            if(A() == 0){
                return {0,0,0,0};
            }
            
            float r = R()/A();
            float g =G()/A();
            float b = B()/A();

            SRGBA[0] = (r<=0.0031308f) ? 12.92f*r : 1.055f * std::pow(r, 1.0f / 2.4f) - 0.055f;
            SRGBA[1] = (g<=0.0031308f) ? 12.92f*g : 1.055f * std::pow(g, 1.0f / 2.4f) - 0.055f;
            SRGBA[2] = (b<=0.0031308f) ? 12.92f*b : 1.055f * std::pow(b, 1.0f / 2.4f) - 0.055f;
            SRGBA[3] = A();
            return SRGBA;
        }
        
        [[nodiscard]] constexpr std::array<float,4> to_straight_linear() const noexcept{
            if(A() == 0){
                return {0,0,0,0};
            }
            return {R()/A(),G()/A(),B()/A(),A()};
        }


    };

    class RGBA_Int : public ContainerN<RGBA_Int,int16_t, 4>, public ColorOpsMixin<RGBA_Int,int16_t, 4>, public ArithmeticOpsMixin<RGBA_Int, int16_t, 4>{
        
        public:
        
        constexpr static void is_alpha(){
            return;
        }


        constexpr static void can_component_add(){return;};
        constexpr static void can_component_subtract(){return;};
        constexpr static void can_scalar_divide(){return;};
        constexpr static void can_scalar_multiply(){return;};
        constexpr static void can_clamp(){return;};
        constexpr static void can_lerp(){return;};

        
        using ContainerN<RGBA_Int,int16_t,4>::zip_in_place;
        using ContainerN<RGBA_Int,int16_t,4>::zip;
        using ContainerN<RGBA_Int,int16_t,4>::zip_reduce;
        using ContainerN<RGBA_Int,int16_t,4>::begin;
        using ContainerN<RGBA_Int,int16_t,4>::end;
        using ContainerN<RGBA_Int,int16_t,4>::cend;
        using ContainerN<RGBA_Int,int16_t,4>::cbegin;
        using ContainerN<RGBA_Int,int16_t,4>::data_;
        using ContainerN<RGBA_Int,int16_t,4>::ContainerN;


        [[nodiscard]] constexpr auto&& R(this auto&& self) {
            return std::forward_like<decltype(self)>(self[0]);
        }
        [[nodiscard]] constexpr auto&& G(this auto&& self) noexcept {
            return std::forward_like<decltype(self)>(self[1]);
        }
        [[nodiscard]] constexpr auto&& B(this auto&& self) noexcept{
            return std::forward_like<decltype(self)>(self[2]);
        }
        [[nodiscard]] constexpr auto&& A(this auto&& self) noexcept{
            return std::forward_like<decltype(self)>(self[3]);
        }


        static constexpr RGBA_Int from_srgba(float r, float g, float b, float a) noexcept {
            float R = (r <= 0.04045f) ? r/12.92f : std::pow((r+0.055f)/1.055f, 2.4f);
            float G = (g <= 0.04045f) ? g/12.92f : std::pow((g+0.055f)/1.055f, 2.4f);
            float B = (b <= 0.04045f) ? b/12.92f : std::pow((b+0.055f)/1.055f, 2.4f);

            return RGBA_Int(
                static_cast<int16_t>(std::round(R*255)),
                static_cast<int16_t>(std::round(G*255)),
                static_cast<int16_t>(std::round(B*255)),
                static_cast<int16_t>(std::round(a*255))
            );
        } 

        static constexpr RGBA_Int from_srgba(int r, int g, int b,int a) noexcept {
            return RGBA_Int::from_srgba(r/255.0f,g/255.0f,b/255.0f, a/255.0f);
        }


        static constexpr RGBA_Int from_hexRGBA(const uint32_t hex) noexcept{

            float sR = ((hex >> 24) & 0xFF) / 255.0f;
            float sG = ((hex >> 16)  & 0xFF) / 255.0f;
            float sB = ((hex >> 8) & 0xFF) / 255.0f;
            float A = (hex & 0xFF) / 255.0f;

            float R = (sR <= 0.04045f) ? sR/12.92f : std::pow((sR+0.055f)/1.055f, 2.4f);
            float G = (sG <= 0.04045f) ? sG/12.92f : std::pow((sG+0.055f)/1.055f, 2.4f);
            float B = (sB <= 0.04045f) ? sB/12.92f : std::pow((sB+0.055f)/1.055f, 2.4f);

            return RGBA_Int(
                static_cast<int16_t>(std::round(R*255)),
                static_cast<int16_t>(std::round(G*255)),
                static_cast<int16_t>(std::round(B*255)),
                static_cast<int16_t>(std::round(A*255))
            );
        }  

        static constexpr RGBA_Int from_hexBGRA(const uint32_t hex) noexcept{

            float sB = ((hex >> 24) & 0xFF) / 255.0f;
            float sG = ((hex >> 16)  & 0xFF) / 255.0f;
            float sR = ((hex >> 8) & 0xFF) / 255.0f;
            float A = (hex & 0xFF) / 255.0f;

            float R = (sR <= 0.04045f) ? sR/12.92f : std::pow((sR+0.055f)/1.055f, 2.4f);
            float G = (sG <= 0.04045f) ? sG/12.92f : std::pow((sG+0.055f)/1.055f, 2.4f);
            float B = (sB <= 0.04045f) ? sB/12.92f : std::pow((sB+0.055f)/1.055f, 2.4f);

            return RGBA_Int(
                static_cast<int16_t>(std::round(R*255)),
                static_cast<int16_t>(std::round(G*255)),
                static_cast<int16_t>(std::round(B*255)),
                static_cast<int16_t>(std::round(A*255))
            );
        }  


        static constexpr RGBA_Int from_straight_linear(int r, int g, int b, int a) noexcept{
            return RGBA_Int(r,g,b,a);
        }

        static constexpr RGBA_Int from_straight_linear(float r, float g, float b, float a) noexcept{
            return from_straight_linear(static_cast<int>(std::round(r*255)),static_cast<int>(std::round(g*255)),static_cast<int>(std::round(b*255)),static_cast<int>(std::round(a*255)));
        }

        static constexpr RGBA_Int from_premultiplied_linear(float r, float g, float b, float a)noexcept{
            int R = std::round(r*255);
            int G = std::round(g*255);
            int B = std::round(b*255);
            int A = std::round(a*255);
            return from_premultiplied_linear(R,G,B,A);
        }

        static constexpr RGBA_Int from_premultiplied_linear(int r, int g, int b, int a)noexcept{
            return RGBA_Int(std::round(r/a),std::round(g/a),std::round(b/a),a);
        }

        [[nodiscard]] constexpr float luminance() const noexcept{
            return ((0.2126*R())+(0.7152*G())+(0.0722*B()))/255.0f;
        }


        [[nodiscard]] constexpr std::array<float,4> to_srgba() const noexcept{
            std::array<float, 4> SRGBA;

            if(A() == 0){
                return {0,0,0,0};
            }

            float sR = R()/255.0f;
            float sG = G()/255.0f;
            float sB = B()/255.0f;
            float sA = A()/255.0f;

            SRGBA[0] = (sR<=0.0031308f) ? 12.92f*sR: 1.055f * std::pow(sR, 1.0f / 2.4f) - 0.055f;
            SRGBA[1] = (sG<=0.0031308f) ? 12.92f*sG : 1.055f * std::pow(sG, 1.0f / 2.4f) - 0.055f;
            SRGBA[2] = (sB<=0.0031308f) ? 12.92f*sB : 1.055f * std::pow(sB, 1.0f / 2.4f) - 0.055f;
            SRGBA[3] = sA;
            return SRGBA;
        }
        
        [[nodiscard]] constexpr std::array<int16_t,4> to_straight_linear() const noexcept{
            if(A() == 0){
                return {0,0,0,0};
            }

            return { R(),G(),B(),A() };
        }

    };

    class LA : public ContainerN<LA, float, 2>, public ColorOpsMixin<LA,float,2>{
        
        constexpr static void is_alpha(){return;};
        constexpr static void can_component_multiply(){return;};
        constexpr static void can_component_divide(){return;};
        constexpr static void can_scalar_multiply(){return;}
        constexpr static void can_scalar_divide(){return;};
        constexpr static void can_component_add(){return;};
        constexpr static void can_component_subtract(){return;};
        constexpr static void can_lerp(){return;};
        constexpr static void can_clamp(){return;};
        
        
        public:
        using ContainerN<LA,float,2>::zip_in_place;
        using ContainerN<LA,float,2>::zip;
        using ContainerN<LA,float,2>::zip_reduce;
        using ContainerN<LA,float,2>::begin;
        using ContainerN<LA,float,2>::end;
        using ContainerN<LA,float,2>::cend;
        using ContainerN<LA,float,2>::cbegin;
        using ContainerN<LA,float,2>::data_;
        using ContainerN<LA,float,2>::ContainerN;

        [[nodiscard]] constexpr auto&& L(this auto&& self) {
            return std::forward_like<decltype(self)>(self[0]);
        }
        [[nodiscard]] constexpr auto&& A(this auto&& self) noexcept {
            return std::forward_like<decltype(self)>(self[1]);
        }

        static constexpr LA from_straight_linear(float l,float a) noexcept{
            float L = l * a;
            assert(l >= 0.0f && l <= 1.0f);
            assert(a >= 0.0f && a <= 1.0f);
            return LA(L, a);
        }
        static constexpr LA from_straight_linear(int l, int a) noexcept{
            return LA::from_straight_linear(l/255.0f,a/255.0f);
        }

        static constexpr LA from_premultiplied_linear(float l, float a)noexcept{
            assert(l >= 0.0f && l <= 1.0f); 
            assert(a >= 0.0f && a <= 1.0f);
            return LA(l,a);
        }

        static constexpr LA from_premultiplied_linear(int l, int a)noexcept{
            return LA::from_premultiplied_linear(l/255.0f, a/255.0f);
        }

        [[nodiscard]] constexpr float luminance() const noexcept{
            return L();
        }

        [[nodiscard]] constexpr std::array<float,2> to_straight_linear() const noexcept{
            if(A() == 0){
                return {0,0};
            }
            return {L()/A(),A()};
        }
    };



    struct RGB8 {
        uint8_t R, G, B;
        constexpr RGB8(RGB rgb){
            float sR = (rgb.R()<=0.0031308f) ? 12.92f*rgb.R(): 1.055f * std::pow(rgb.R(), 1.0f / 2.4f) - 0.055f;
            float sG = (rgb.G()<=0.0031308f) ? 12.92f*rgb.G() : 1.055f * std::pow(rgb.G(), 1.0f / 2.4f) - 0.055f;
            float sB = (rgb.B()<=0.0031308f) ? 12.92f*rgb.B() : 1.055f * std::pow(rgb.B(), 1.0f / 2.4f) - 0.055f;
            R = static_cast<uint8_t>(std::clamp(std::round(sR*255),0.0f,255.0f));
            G = static_cast<uint8_t>(std::clamp(std::round(sG*255),0.0f,255.0f));
            B = static_cast<uint8_t>(std::clamp(std::round(sB*255),0.0f,255.0f));
        }

        constexpr RGB8(RGB_Int rgb){

            float r = rgb.R()/255.0f;
            float g = rgb.G()/255.0f;
            float b = rgb.B()/255.0f;

            float sR = (r<=0.0031308f) ? 12.92f*r: 1.055f * std::pow(r, 1.0f / 2.4f) - 0.055f;
            float sG = (g<=0.0031308f) ? 12.92f*g : 1.055f * std::pow(g, 1.0f / 2.4f) - 0.055f;
            float sB = (b<=0.0031308f) ? 12.92f*b : 1.055f * std::pow(b, 1.0f / 2.4f) - 0.055f;
            R = static_cast<uint8_t>(std::clamp(std::round(sR*255),0.0f,255.0f));
            G = static_cast<uint8_t>(std::clamp(std::round(sG*255),0.0f,255.0f));
            B = static_cast<uint8_t>(std::clamp(std::round(sB*255),0.0f,255.0f));
        }

        constexpr RGB8(RGBA rgba){

            if(rgba.A() == 0){
                R = 0;
                G= 0;
                B=0;
                return;
            }
            float r = rgba.R()/rgba.A();
            float g = rgba.G()/rgba.A();
            float b = rgba.B()/rgba.A();

            float sR = (r<=0.0031308f) ? 12.92f*r: 1.055f * std::pow(r, 1.0f / 2.4f) - 0.055f;
            float sG = (g<=0.0031308f) ? 12.92f*g : 1.055f * std::pow(g, 1.0f / 2.4f) - 0.055f;
            float sB = (b<=0.0031308f) ? 12.92f*b : 1.055f * std::pow(b, 1.0f / 2.4f) - 0.055f;
            R = static_cast<uint8_t>(std::clamp(std::round(sR*255),0.0f,255.0f));
            G = static_cast<uint8_t>(std::clamp(std::round(sG*255),0.0f,255.0f));
            B = static_cast<uint8_t>(std::clamp(std::round(sB*255),0.0f,255.0f));
        }
        constexpr RGB8(RGBA_Int rgba){

            float r = rgba.R()/255.0f;
            float g = rgba.G()/255.0f;
            float b = rgba.B()/255.0f;

            float sR = (r<=0.0031308f) ? 12.92f*r: 1.055f * std::pow(r, 1.0f / 2.4f) - 0.055f;
            float sG = (g<=0.0031308f) ? 12.92f*g : 1.055f * std::pow(g, 1.0f / 2.4f) - 0.055f;
            float sB = (b<=0.0031308f) ? 12.92f*b : 1.055f * std::pow(b, 1.0f / 2.4f) - 0.055f;
            R = static_cast<uint8_t>(std::clamp(std::round(sR*255),0.0f,255.0f));
            G = static_cast<uint8_t>(std::clamp(std::round(sG*255),0.0f,255.0f));
            B = static_cast<uint8_t>(std::clamp(std::round(sB*255),0.0f,255.0f));
        }

    };

    struct RGBA8{
        uint8_t R,G,B,A;

        constexpr RGBA8(RGB rgb,uint8_t a = 1){
            float sR = (rgb.R()<=0.0031308f) ? 12.92f*rgb.R(): 1.055f * std::pow(rgb.R(), 1.0f / 2.4f) - 0.055f;
            float sG = (rgb.G()<=0.0031308f) ? 12.92f*rgb.G() : 1.055f * std::pow(rgb.G(), 1.0f / 2.4f) - 0.055f;
            float sB = (rgb.B()<=0.0031308f) ? 12.92f*rgb.B() : 1.055f * std::pow(rgb.B(), 1.0f / 2.4f) - 0.055f;
            R = static_cast<uint8_t>(std::clamp(std::round(sR*255.0f),0.0f,255.0f));
            G = static_cast<uint8_t>(std::clamp(std::round(sG*255.0f),0.0f,255.0f));
            B = static_cast<uint8_t>(std::clamp(std::round(sB*255.0f),0.0f,255.0f));
            A = static_cast<uint8_t>(std::clamp(std::round(a*255.0f),0.0f,255.0f));
        }

        constexpr RGBA8(RGB_Int rgb,int a = 255){
            float r = rgb.R()/255.0f;
            float g = rgb.G()/255.0f;
            float b = rgb.B()/255.0f;

            float sR = (r<=0.0031308f) ? 12.92f*r: 1.055f * std::pow(r, 1.0f / 2.4f) - 0.055f;
            float sG = (g<=0.0031308f) ? 12.92f*g : 1.055f * std::pow(g, 1.0f / 2.4f) - 0.055f;
            float sB = (b<=0.0031308f) ? 12.92f*b : 1.055f * std::pow(b, 1.0f / 2.4f) - 0.055f;
            R = static_cast<uint8_t>(std::clamp(std::round(sR*255),0.0f,255.0f));
            G = static_cast<uint8_t>(std::clamp(std::round(sG*255),0.0f,255.0f));
            B = static_cast<uint8_t>(std::clamp(std::round(sB*255),0.0f,255.0f));
            A = static_cast<uint8_t>(a);

        }

        constexpr RGBA8(RGBA rgba){
            if(rgba.A() == 0){
                R = 0;
                G= 0;
                B=0;
                A = 0;
                return;
            }
            float r = rgba.R()/rgba.A();
            float g = rgba.G()/rgba.A();
            float b = rgba.B()/rgba.A();

            float sR = (r<=0.0031308f) ? 12.92f*r: 1.055f * std::pow(r, 1.0f / 2.4f) - 0.055f;
            float sG = (g<=0.0031308f) ? 12.92f*g : 1.055f * std::pow(g, 1.0f / 2.4f) - 0.055f;
            float sB = (b<=0.0031308f) ? 12.92f*b : 1.055f * std::pow(b, 1.0f / 2.4f) - 0.055f;
            R = static_cast<uint8_t>(std::clamp(std::round(sR*255),0.0f,255.0f));
            G = static_cast<uint8_t>(std::clamp(std::round(sG*255),0.0f,255.0f));
            B = static_cast<uint8_t>(std::clamp(std::round(sB*255),0.0f,255.0f));
            A = static_cast<uint8_t>(std::clamp(std::round(rgba.A()*255),0.0f,255.0f));

        }
        constexpr RGBA8(RGBA_Int rgba){
            float r = rgba.R()/255.0f;
            float g = rgba.G()/255.0f;
            float b = rgba.B()/255.0f;

            float sR = (r<=0.0031308f) ? 12.92f*r: 1.055f * std::pow(r, 1.0f / 2.4f) - 0.055f;
            float sG = (g<=0.0031308f) ? 12.92f*g : 1.055f * std::pow(g, 1.0f / 2.4f) - 0.055f;
            float sB = (b<=0.0031308f) ? 12.92f*b : 1.055f * std::pow(b, 1.0f / 2.4f) - 0.055f;
            R = static_cast<uint8_t>(std::clamp(std::round(sR*255),0.0f,255.0f));
            G = static_cast<uint8_t>(std::clamp(std::round(sG*255),0.0f,255.0f));
            B = static_cast<uint8_t>(std::clamp(std::round(sB*255),0.0f,255.0f));
            A = static_cast<uint8_t>(rgba.A());
        }


    };

}