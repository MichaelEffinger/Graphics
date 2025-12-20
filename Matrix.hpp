#include <type_traits>
#include <cassert>
#include <cmath>
#include <algorithm>
#include <functional>
#include <string>
#include "ContainerN.hpp"
#include <cstdint>
#include "ArithmeticOpsMixin.hpp"
#include "VectorN.hpp"

namespace ES{



    // I think I am going to go with column major... too popular in graphics to ignore
    template<typename T, std::size_t N, std::size_t M = N>
    class Matrix : public ArithmeticOpsMixin<Matrix<T,N,M>, T, N*M>, public ContainerN<Matrix<T,N,M>,T,N*M>{
       
        using ContainerN<Matrix,T,N>::data_;
        public: 

        using ContainerN<Matrix,T,N>::zip_in_place;
        using ContainerN<Matrix,T,N>::zip;
        using ContainerN<Matrix,T,N>::zip_reduce;
        using ContainerN<Matrix,T,N>::begin;
        using ContainerN<Matrix,T,N>::end;
        using ContainerN<Matrix,T,N>::cend;
        using ContainerN<Matrix,T,N>::data;
        using ContainerN<Matrix,T,N>::cbegin;
        using ContainerN<Matrix,T,N>::operator[];
        using ContainerN<Matrix,T,N>::ContainerN;


        static constexpr void can_scalar_multiply(){return;}
        static constexpr void can_scalar_divide(){return;}
        static constexpr void can_component_add(){return;}
        static constexpr void can_component_subtract(){return;}
        static constexpr void can_lerp(){return;}
        static constexpr void can_negate(){return;}
        static constexpr void can_clamp(){return;}



        constexpr auto&& operator()(this auto&& self, std::size_t row, std::size_t column) noexcept{
            return std::forward_like<decltype(self)>(self[(column*N+row)]);
        }

        constexpr Matrix swap_rows(std::size_t first, std::size_t second) const noexcept {
            Matrix temp(*this);
            for(std::size_t i=0; i<M;i++){
                std::swap(temp[i*N+ first], temp[i*N + second]);
            }
            return temp;            
        }

        constexpr Matrix& swap_rows_in_place(std::size_t first, std::size_t second) noexcept{
            for(std::size_t i=0; i<M; i++){
                std::swap(operator[](i*N+first),operator[](i*N+second));
            }
            return *this;
        }

        constexpr Matrix scale_row(std::size_t row, T scale)const noexcept{
            Matrix temp(*this);
            for (std::size_t i =0; i<M; i++){
                temp[i*N+row] *= scale;
            }
            return temp;
        }

        constexpr Matrix& scale_row_in_place(std::size_t row, T scale) noexcept {
            for (std::size_t i =0; i<M; i++){
                operator[](i*N+row) *= scale;
            }
            return *this;
        }

        constexpr Matrix add_scaled_row(std::size_t source, T scale, std::size_t destination) const noexcept{
            Matrix temp(*this);
            for(std::size_t i =0; i<M; i++){
                temp[i*N+destination] +=temp[i*N+source]*scale; 
            }
            return temp;
        }

        constexpr Matrix& add_scaled_row_in_place(std::size_t source, T scale, std::size_t destination) noexcept{
            for(std::size_t i = 0; i<M; i++){
                operator[](i*N+destination) += operator[](i*N+source)*scale;
            }
            return *this;
        }            
        
        constexpr T determinant() const noexcept requires (N==2 && M==2){
            return operator[](0)*operator[](3) - operator[](1)*operator[](2);
        }

        constexpr T determinant() const noexcept requires (N==3 && M==3){
            return operator[](0)*operator[](4)*operator[](8) + operator[](3)*operator[](7)*operator[](2) + operator[](6)*operator[](1)*operator[](5) - operator[](6)*operator[](4)*operator[](2) - operator[](3)*operator[](1)*operator[](8) -operator[](0)*operator[](7)*operator[](5);
        }

        constexpr T determinant() const noexcept requires (N==4 && M==4){
            T m0 = operator[](0)*( operator[](5)* operator[](10)* operator[](15) +  operator[](6)* operator[](11)* operator[](13) +  operator[](7)* operator[](9)* operator[](14) -  operator[](7)* operator[](10)* operator[](13) -  operator[](6)* operator[](9)* operator[](15) -  operator[](5)* operator[](11)* operator[](14));
            T m1 =  operator[](1)*( operator[](4)* operator[](10)* operator[](15) +  operator[](6)* operator[](11)* operator[](12) +  operator[](7)* operator[](8)* operator[](14) -  operator[](7)* operator[](10)* operator[](12) -  operator[](6)* operator[](8)* operator[](15) -  operator[](4)* operator[](11)* operator[](14));
            T m2 =  operator[](2)*( operator[](4)* operator[](9)* operator[](15) +  operator[](5)* operator[](11)* operator[](12) +  operator[](7)* operator[](8)* operator[](13) -  operator[](7)* operator[](9)* operator[](12) -  operator[](5)* operator[](8)* operator[](15) -  operator[](4)* operator[](11)* operator[](13));
            T m3 =  operator[](3)*( operator[](4)* operator[](9)* operator[](14) +  operator[](5)* operator[](10)* operator[](12) +  operator[](6)* operator[](8)* operator[](13) -  operator[](6)* operator[](9)* operator[](12) -  operator[](5)* operator[](8)* operator[](14) -  operator[](4)* operator[](10)* operator[](13));
          return m0 - m1 + m2 - m3;

        }
        
        constexpr T determinant() const noexcept requires (N == M && N >4 && std::is_floating_point_v<T>) {
            T accumulate = 1;
            Matrix temp(*this);

            for (std::size_t i = 0;i<N;i++){
                
                std::size_t pivot = i;
                if(temp[i*N+i] == T{0}){
                    for(std::size_t j =i + 1; j<N; j++){
                        if (temp[i*N+j] != 0){
                            pivot = j;
                            break;
                        }  
                    }
                    if(pivot == i){
                        return T{0};
                    }
                    
                    temp.swap_rows_in_place(i,pivot);
                    accumulate *=-1;
                }
                for(std::size_t k = i+1; k<N;k++){
                    T scale = -temp[i*N+k]/temp[i*N+i];
                    temp.add_scaled_row_in_place(i,scale,k);
                }            
            }
            return accumulate * temp.product_of_diagonals();
        }


        constexpr T product_of_diagonals() const noexcept{
            T accumulate = 1;
            for(std::size_t i = 0; i<N; i++){
                accumulate *= operator[](i*N +i);
            }
            return accumulate;
        }


        constexpr VectorN<T,N> column(std::size_t column) const noexcept{
            VectorN<T,N> temp;

            for(std::size_t i =0; i<N;i++){
                temp[i] = operator[](column*N + i);
            }
            return temp;
        }


        constexpr Matrix transpose() const noexcept{
            Matrix<T,M,N>  temp;

            for(std::size_t i=0; i<N; i++){
                for (std::size_t j=0;j<M;j++){
                    temp[i*M+j]=operator[](j*N+i);
                } 
            }
            return temp;
        }
        constexpr Matrix& tranpose_in_place() noexcept requires(N==M){
 
            for(std::size_t i =0;i<N;i++){
                for(std::size_t j = i+1; j<N; j++){
                    std::swap(operator[](i*N+j),operator[](j*N+i));
                }
            }
            return *this;

        }

        constexpr Matrix inverse() const noexcept {
            Matrix temp(*this);
            return temp.inverse_in_place();
        };


        constexpr Matrix& inverse_in_place() requires (N == 2 && M == 2){

            const T det = determinant();
            assert(det != T{0});

            const T inv_det = T{1} / det;

            std::swap(operator[](0),operator[](3));

            operator[](0) = operator[](0)*inv_det;
            operator[](1) *= -inv_det;
            operator[](2) *= -inv_det;
            operator[](3) = operator[](3)*inv_det;
            return *this;
        }
        constexpr Matrix inverse_in_place() const noexcept requires (N==3 && M==3){
            const T det = determinant();
            assert(det != T{0});

            const T inv_det = T{1} / det;
            
        }
        constexpr Matrix inverse_in_place() const noexcept requires(N==4,M==4){

        }
        constexpr Matrix inverse_in_place() const noexcept{};




        template <std::size_t O, std::size_t P>
        constexpr Matrix<T,N,P> operator*(){
            return false;
        }
        constexpr Matrix rref() const noexcept{};
        constexpr Matrix reduce() const noexcept{};
        constexpr Matrix reduce_aggregate() const noexcept{};
        constexpr T trace() const noexcept{};
        constexpr Matrix adjugate() const noexcept{};
        constexpr auto Eigenvalues() const noexcept{};
        constexpr auto orthonormalize() const noexcept{}; 
        constexpr auto minor(auto row,auto col) const noexcept{};
        constexpr auto map(auto &&func) const noexcept{};


        constexpr auto is_symmetric() const noexcept{};
        constexpr auto is_orthogonal() const noexcept{};
        constexpr auto is_invertible() const noexcept{};



        auto get() noexcept {
            return data_;
        }

         auto get_row_major() {
            return data_;
        }

        auto get_col_major() {
            return data_;
        }


    };



}