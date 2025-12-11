#pragma once
#include <type_traits>
#include <cassert>
#include <cmath>
#include <algorithm>
#include <functional>
#include <string>
#include "ContainerN.hpp"
#include <cstdint>



namespace ES{

    template <typename T> struct max_color;

    template <> struct max_color<uint8_t> {
        static constexpr uint8_t value = 255;
    };

    template <> struct max_color<float>{
        static constexpr float value = 1.0f;
    };

    template <> struct max_color<uint16_t>{
         static constexpr uint16_t value = 65025;
    };

    template<class Child, typename T, std::size_t N>
    class ColorOpsMixin{

    protected:
        constexpr Child& derived() {return static_cast<Child&>(*this);}
        constexpr const Child& derived() const {return static_cast<const Child&>(*this);}
        
    public:

        [[nodiscard]] constexpr Child operator+(Child rhs) const noexcept{
            return derived().zip(rhs,std::plus{});
        }

        constexpr Child& operator+=(Child rhs) noexcept{
            return derived().zip_in_place(rhs,std::plus{});
        }

        [[nodiscard]] constexpr Child operator-(Child rhs)const noexcept{
            return derived().zip(rhs,std::minus{});
        }

        constexpr Child & operator-=(Child rhs) noexcept{
            return derived().zip_in_place(rhs,std::minus{});
        }
        
        [[nodiscard]] constexpr Child operator*(Child rhs) const noexcept{
            return derived().zip(rhs,std::multiplies{});
        }

        constexpr Child& operator*=(Child rhs) noexcept{
            return derived().zip_in_place(rhs,std::multiplies{});
        }

        [[nodiscard]] constexpr Child operator/(Child rhs)const noexcept{
            return derived().zip(rhs, [](T a, T b) { assert(b !=0 && "Divide by zero in component division"); return (b != 0) ? (a / b) : T{0}; });
        }

        constexpr Child& operator/=(Child rhs)noexcept{
            return derived().zip_in_place(rhs, [](T a, T b) { assert(b !=0 && "Divide by zero in component division"); return (b != 0) ? (a / b) : T{0}; });
        }

        [[nodiscard]] constexpr Child operator*(T scalar) const noexcept{
            Child temp_col;
            std::transform(derived().cbegin(),derived().cend(),temp_col.begin(),[scalar](T in) {return in*scalar;});
            return temp_col;
        }
        
        [[nodiscard]] constexpr friend Child operator*(T scalar, Child lhs) noexcept{
            Child temp_col;
            std::transform(lhs.cbegin(),lhs.cend(),temp_col.begin(),[scalar](T in) {return in*scalar;} );
            return temp_col;
        }

        constexpr Child& operator*=(T scalar)noexcept{
            std::transform(derived().begin(),derived().end(),derived().begin(),[scalar](T in){return in * scalar;});
            return derived();
        }

        [[nodiscard]] constexpr Child operator/(T scalar) const noexcept{
            Child tempCol;
            std::transform(derived().cbegin(), derived().cend(),tempCol.begin(),[scalar](T in) {assert(scalar !=0 && "Divide by zero in operator/"); return (scalar != 0) ? (in / scalar) : T{0}; });
            return tempCol;
        }

        constexpr Child& operator/=(T scalar)noexcept{
            std::transform(derived().begin(),derived().end(),derived().begin(),[scalar](T in) {assert(scalar !=0 && "Divide by zero in operator/"); return (scalar != 0) ? (in / scalar) : T{0}; });
            return derived();
        }

        [[nodiscard]] constexpr Child lerp(Child rhs, float t) const noexcept{
            return derived().zip(rhs,[t](T a, T b) {return a+(b-a)*t;});
        }

        constexpr Child& lerp_in_place(Child rhs, float t) noexcept{
            return derived().zip_in_place(rhs,[t](T a,T b) {return a+(b-a)*t;});
        }

        [[nodiscard]] constexpr Child clamp(T lower, T upper) const noexcept{
            Child temp_col;
            std::transform(derived().cbegin(),derived().cend(), temp_col.begin(),[lower,upper](T in){return std::clamp(in, lower, upper);});
            return temp_col;
        }

        constexpr Child& clamp_in_place(T lower, T upper)noexcept{
            std::transform(derived().begin(),derived().end(),derived().begin(),[lower,upper](T in){return std::clamp(in, lower, upper);});
            return derived();
        }

        [[nodiscard]] constexpr Child adjust_brightness(T factor) noexcept requires requires { Child::is_alpha(); } {
            Child temp_col;
            std::transform(derived().begin(),derived().end()-1,temp_col.begin(),[factor](T in){return in * factor;});
            temp_col.tail() = derived().tail();
            return temp_col;
        }

        constexpr Child adjust_brightness_in_place(T factor) noexcept requires requires { Child::is_alpha(); } {
            std::transform(derived().begin(),derived().end()-1,derived().begin(),[factor](T in){return in * factor;});
            return derived();
        }
        
        [[nodiscard]] constexpr Child adjust_brightness(T factor) noexcept requires requires { Child::is_standard(); }{
            Child temp_col;
            std::transform(derived().cbegin(),derived().cend(),temp_col.begin(),[factor](T in){return in * factor;});
            return temp_col;
        }

        constexpr Child adjust_brightness_in_place(T factor) noexcept requires requires { Child::is_standard(); }{
            std::transform(derived().begin(),derived().end(),derived().begin(),[factor](T in){return in * factor;});
            return derived();
        }


        [[nodiscard]] constexpr Child invert() const noexcept requires requires { Child::is_standard(); } {
            Child temp_col;
            std::transform(derived().cbegin(),derived().cend(),temp_col.begin(),[](T in){return  max_color<T>::value-in;});
            return temp_col;
        }

        constexpr Child invert_in_place() noexcept requires requires { Child::is_standard(); }{
            std::transform(derived().begin(),derived().end(),derived().begin(),[](T in){return  max_color<T>::value-in;});
            return derived();
        }

        [[nodiscard]] constexpr Child invert() const noexcept requires requires { Child::is_alpha(); }{
            Child temp_col;
            std::transform(derived().cbegin(),derived().cend()-1,temp_col.begin(),[](T in){return  max_color<T>::value-in;});
            temp_col.tail() = derived().tail();
            return temp_col;
        }

        constexpr Child invert_in_place() noexcept requires requires { Child::is_alpha(); }{
            std::transform(derived().begin(),derived().end()-1,derived().begin(),[](T in){return  max_color<T>::value-in;});
            return derived();
        }

        [[nodiscard]] constexpr Child adjust_saturation(float s) const noexcept requires requires { Child::is_standard(); } {
            T L = derived().luminance()* max_color<T>::value;
            Child L_color;
            std::fill(L_color.begin(),L_color.end(),L);
            return lerp(L_color,1-s);
        }

        constexpr Child adjust_saturation_in_place(float s) noexcept requires requires { Child::is_standard(); }{
            T L = derived().luminance() * max_color<T>::value;
            Child L_color;
            std::fill(L_color.begin(),L_color.end(),L);
            return lerp_in_place(L_color, 1-s);
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
    
    
    class RGB : public ContainerN<RGB,float,3>, public ColorOpsMixin<RGB,float,3> {
        constexpr static void is_standard(){
            return;
        }

    public:
        using ContainerN<RGB,float,3>::zip_in_place;
        using ContainerN<RGB,float,3>::zip;
        using ContainerN<RGB,float,3>::zip_reduce;
        using ContainerN<RGB,float,3>::begin;
        using ContainerN<RGB,float,3>::end;
        using ContainerN<RGB,float,3>::cend;
        using ContainerN<RGB,float,3>::cbegin;
        using ContainerN<RGB,float,3>::data_;
        using ContainerN<RGB,float,3>::ContainerN;



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

        static constexpr RGB from_hex(const uint32_t hex) noexcept{
            float R = ((hex >> 16) & 0xFF) / 255.0f;
            float G = ((hex >> 8)  & 0xFF) / 255.0f;
            float B = (hex & 0xFF) / 255.0f;
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

    class RGB_Int : public ContainerN<RGB_Int,uint8_t,3>, public ColorOpsMixin<RGB_Int,uint8_t,3>{
      
        constexpr static void is_standard(){
            return;
        }

        public:
        using ContainerN<RGB_Int,uint8_t,3>::zip_in_place;
        using ContainerN<RGB_Int,uint8_t,3>::zip;
        using ContainerN<RGB_Int,uint8_t,3>::zip_reduce;
        using ContainerN<RGB_Int,uint8_t,3>::begin;
        using ContainerN<RGB_Int,uint8_t,3>::end;
        using ContainerN<RGB_Int,uint8_t,3>::cend;
        using ContainerN<RGB_Int,uint8_t,3>::cbegin;
        using ContainerN<RGB_Int,uint8_t,3>::data_;
        using ContainerN<RGB_Int,uint8_t,3>::ContainerN;


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
                static_cast<uint8_t>(std::round(R*255)),
                static_cast<uint8_t>(std::round(G*255)),
                static_cast<uint8_t>(std::round(B*255))
            );
        } 

        static constexpr RGB_Int from_srgb(int r, int g, int b) noexcept {
            return RGB_Int::from_srgb(r/255.0f,g/255.0f,b/255.0f);
        }

        static constexpr RGB_Int from_hex(const uint32_t hex) noexcept{
            int R = ((hex >> 16) & 0xFF);
            int G = ((hex >> 8)  & 0xFF);
            int B = (hex & 0xFF);
            return RGB_Int(R,G,B);
        }  

        static constexpr RGB_Int from_linear(int r, int g, int b) noexcept{
            assert(r >= 0 && r <= 255); 
            assert(g >= 0 && g <= 255);
            assert(b >= 0 && b <= 255);
            return RGB_Int(r,g,b);
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

    class RGBA : public ContainerN<RGBA,float, 4>, public ColorOpsMixin<RGBA,float,4>{
        
        constexpr static void is_alpha(){
            return;
        }
        
        public:
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

        static constexpr RGBA from_hex(const uint32_t hex) noexcept{
            
            float R = ((hex >> 24) & 0xFF) / 255.0f;
            float G = ((hex >> 16)  & 0xFF) / 255.0f;
            float B = ((hex >> 8) & 0xFF) / 255.0f;
            float A = (hex & 0xFF) / 255.0f;
            return RGBA(R,G,B,A);
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
            return (0.2126*R())+(0.7152*G())+(0.0722*B());
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

    class RGBA_Int : public ContainerN<RGBA_Int,uint16_t, 4>, public ColorOpsMixin<RGBA_Int,uint16_t, 4>{
        
        constexpr static void is_alpha(){
            return;
        }
        
        public:
        using ContainerN<RGBA_Int,uint16_t,4>::zip_in_place;
        using ContainerN<RGBA_Int,uint16_t,4>::zip;
        using ContainerN<RGBA_Int,uint16_t,4>::zip_reduce;
        using ContainerN<RGBA_Int,uint16_t,4>::begin;
        using ContainerN<RGBA_Int,uint16_t,4>::end;
        using ContainerN<RGBA_Int,uint16_t,4>::cend;
        using ContainerN<RGBA_Int,uint16_t,4>::cbegin;
        using ContainerN<RGBA_Int,uint16_t,4>::data_;
        using ContainerN<RGBA_Int,uint16_t,4>::ContainerN;


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
            float R = (r <= 0.04045f) ? r/12.92f : std::pow((r+0.055f)/1.055f, 2.4f) *a;
            float G = (g <= 0.04045f) ? g/12.92f : std::pow((g+0.055f)/1.055f, 2.4f)*a;
            float B = (b <= 0.04045f) ? b/12.92f : std::pow((b+0.055f)/1.055f, 2.4f)*a;

            return RGBA_Int(
                static_cast<uint8_t>(std::round(R*255)),
                static_cast<uint8_t>(std::round(G*255)),
                static_cast<uint8_t>(std::round(B*255)),
                static_cast<uint8_t>(std::round(a*255))
            );
        } 

        static constexpr RGBA_Int from_srgba(int r, int g, int b,int a) noexcept {
            return RGBA_Int::from_srgba(r/255.0f,g/255.0f,b/255.0f, a/255.0f);
        }


        static constexpr RGBA_Int from_hex(const uint32_t hex) noexcept{
            
            uint8_t R = ((hex >> 24) & 0xFF);
            uint8_t G = ((hex >> 16)  & 0xFF);
            uint8_t B = ((hex >> 8) & 0xFF);
            uint8_t A = (hex & 0xFF);
            return RGBA_Int(R,G,B,A);
        }  

        static constexpr RGBA_Int from_straight_linear(int r, int g, int b, int a) noexcept{
            int R = r * a;
            int G = g * a;
            int B = b * a;
            assert(r >= 0 && r <= 255);
            assert(g >= 0 && g <= 255);
            assert(b >= 0 && b <= 255);
            assert(a >= 0 && a <= 255);
            return RGBA_Int(R, G, B, a);
        }

        static constexpr RGBA_Int from_straight_linear(float r, float g, float b, float a) noexcept{
            int R = static_cast<uint8_t>(std::round(r*255));
            int G = static_cast<uint8_t>(std::round(g*255));
            int B = static_cast<uint8_t>(std::round(b*255));
            int A = static_cast<uint8_t>(std::round(a*255));
            return RGBA_Int::from_straight_linear(R,G ,B ,A);
        }

        static constexpr RGBA_Int from_premultiplied_linear(float r, float g, float b, float a)noexcept{
            assert(r >= 0.0f && r <= 1.0f); 
            assert(g >= 0.0f && g <= 1.0f);
            assert(b >= 0.0f && b <= 1.0f);
            assert(a >= 0.0f && a <= 1.0f);
            return RGBA_Int(r,g,b,a);
        }

        static constexpr RGBA_Int from_premultiplied_linear(int r, int g, int b, int a)noexcept{
            return RGBA_Int::from_premultiplied_linear(r/255.0f,g/255.0f,b/255.0f,a/255.0f);
        }

        [[nodiscard]] constexpr float luminance() const noexcept{
            return (0.2126*R())+(0.7152*G())+(0.0722*B());
        }


        [[nodiscard]] constexpr std::array<float,4> to_srgba() const noexcept{
            std::array<float, 4> SRGBA;

            if(A() == 0){
                return {0,0,0,0};
            }
            
            float r = R()/static_cast<float>(A());
            float g =G()/static_cast<float>(A());
            float b = B()/static_cast<float>(A());

            SRGBA[0] = (r<=0.0031308f) ? 12.92f*r : 1.055f * std::pow(r, 1.0f / 2.4f) - 0.055f;
            SRGBA[1] = (g<=0.0031308f) ? 12.92f*g : 1.055f * std::pow(g, 1.0f / 2.4f) - 0.055f;
            SRGBA[2] = (b<=0.0031308f) ? 12.92f*b : 1.055f * std::pow(b, 1.0f / 2.4f) - 0.055f;
            SRGBA[3] = A();
            return SRGBA;
        }
        
        [[nodiscard]] constexpr std::array<uint8_t,4> to_straight_linear() const noexcept{
            if(A() == 0){
                return {0,0,0,0};
            }
            return {static_cast<uint8_t>(R()/A()),static_cast<uint8_t>(G()/A()),static_cast<uint8_t>(B()/A()),static_cast<uint8_t>(A())};
        }

    };

    class LA : public ContainerN<LA, float, 2>, public ColorOpsMixin<LA,float,2>{
        
        constexpr static void is_alpha(){
            return;
        }
        
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
}