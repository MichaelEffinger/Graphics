#pragma once

#include <type_traits>
#include <cassert>
#include <cmath>
#include <algorithm>
#include <cstring>
#include "ContainerN.hpp"
#include "ArithmeticOpsMixin.hpp"
#include "VectorN.hpp"

namespace ES{



    // I think I am going to go with column major... too popular in graphics to ignore
    template<typename T, std::size_t N, std::size_t M = N>
    class Matrix : public ArithmeticOpsMixin<Matrix<T,N,M>, T, N*M>, public ContainerN<Matrix<T,N,M>,T,N*M>{
       
        using ContainerN<Matrix,T,N*M>::data_;


        public: 

        using ContainerN<Matrix,T,N*M>::zip_in_place;
        using ContainerN<Matrix,T,N*M>::zip;
        using ContainerN<Matrix,T,N*M>::zip_reduce;
        using ContainerN<Matrix,T,N*M>::begin;
        using ContainerN<Matrix,T,N*M>::end;
        using ContainerN<Matrix,T,N*M>::cend;
        using ContainerN<Matrix,T,N*M>::data;
        using ContainerN<Matrix,T,N*M>::cbegin;
        using ContainerN<Matrix,T,N*M>::operator[];
        using ContainerN<Matrix,T,N*M>::ContainerN;
        using ArithmeticOpsMixin<Matrix,T,N*M>::operator*;


        static constexpr void can_scalar_multiply(){return;}
        static constexpr void can_scalar_divide(){return;}
        static constexpr void can_component_add(){return;}
        static constexpr void can_component_subtract(){return;}
        static constexpr void can_lerp(){return;}
        static constexpr void can_negate(){return;}
        static constexpr void can_clamp(){return;}


        template<class... Args>
        constexpr Matrix(Args... columns) requires(sizeof...(Args) == M && (std::is_same_v<Args, VectorN<T,N>> && ...)){
            std::size_t col = 0;
            ((std::memcpy(&data_[col * N], &columns[0], sizeof(T) * N), col++), ...);
        }



        constexpr auto&& operator()(this auto&& self, std::size_t row, std::size_t column) noexcept{
            return std::forward_like<decltype(self)>(self[(column*N+row)]);
        }

        [[nodiscard]] constexpr Matrix swap_rows(std::size_t first, std::size_t second) const noexcept {
            Matrix temp((*this));
            for(std::size_t i=0; i<M;i++){
                std::swap(temp(first,i), temp(second, i));
            }
            return temp;            
        }


        constexpr Matrix& swap_rows_in_place(std::size_t first, std::size_t second) noexcept{
            for(std::size_t i=0; i<M; i++){
                std::swap((*this)(first,i),(*this)(second,i));
            }
            return (*this);
        }

        [[nodiscard]] constexpr Matrix scale_row(std::size_t row, T scale)const noexcept{
            Matrix temp((*this));
            for (std::size_t i =0; i<M; i++){
                temp(row,i) *= scale;
            }
            return temp;
        }

        constexpr Matrix& scale_row_in_place(std::size_t row, T scale) noexcept {
            for (std::size_t i =0; i<M; i++){
                (*this)(row,i) *= scale;
            }
            return (*this);
        }

        [[nodiscard]] constexpr Matrix add_scaled_row(std::size_t source, T scale, std::size_t destination) const noexcept{
            Matrix temp((*this));
            for(std::size_t i =0; i<M; i++){
                temp(destination,i) += temp(source,i)*scale;
            }
            return temp;
        }
        
        constexpr Matrix& add_scaled_row_in_place(std::size_t source, T scale, std::size_t destination) noexcept{
            for(std::size_t i = 0; i<M; i++){
                (*this)(destination,i) += (*this)(source,i)*scale;
            }
            return (*this);
        }            
        
        [[nodiscard]] constexpr T determinant() const noexcept requires (N==1 && M==1){
            return data_[0];
        }
        [[nodiscard]] constexpr T determinant() const noexcept requires (N==2 && M==2){
            return (*this)[0]*(*this)[3] - (*this)[1]*(*this)[2];
        }
        //specialization for the 3x3, its just the cofactor expansion urnolled
        [[nodiscard]] constexpr T determinant() const noexcept requires (N==3 && M==3){
            return (*this)[0]*(*this)[4]*(*this)[8] + (*this)[3]*(*this)[7]*(*this)[2] + (*this)[6]*(*this)[1]*(*this)[5] - (*this)[6]*(*this)[4]*(*this)[2] - (*this)[3]*(*this)[1]*(*this)[8] -(*this)[0]*(*this)[7]*(*this)[5];
        }

        //specialization for the 4x4, its just the cofactor expansion urnolled
        [[nodiscard]] constexpr T determinant() const noexcept requires (N==4 && M==4){
            T m0 = (*this)[0]*( (*this)[5]* (*this)[10]* (*this)[15] +  (*this)[6]* (*this)[11]* (*this)[13] +  (*this)[7]* (*this)[9]* (*this)[14] -  (*this)[7]* (*this)[10]* (*this)[13] -  (*this)[6]* (*this)[9]* (*this)[15] -  (*this)[5]* (*this)[11]* (*this)[14]);
            T m1 =  (*this)[1]*( (*this)[4]* (*this)[10]* (*this)[15] +  (*this)[6]* (*this)[11]* (*this)[12] +  (*this)[7]* (*this)[8]* (*this)[14] -  (*this)[7]* (*this)[10]* (*this)[12] -  (*this)[6]* (*this)[8]* (*this)[15] -  (*this)[4]* (*this)[11]* (*this)[14]);
            T m2 =  (*this)[2]*( (*this)[4]* (*this)[9]* (*this)[15] +  (*this)[5]* (*this)[11]* (*this)[12] +  (*this)[7]* (*this)[8]* (*this)[13] -  (*this)[7]* (*this)[9]* (*this)[12] -  (*this)[5]* (*this)[8]* (*this)[15] -  (*this)[4]* (*this)[11]* (*this)[13]);
            T m3 =  (*this)[3]*( (*this)[4]* (*this)[9]* (*this)[14] +  (*this)[5]* (*this)[10]* (*this)[12] +  (*this)[6]* (*this)[8]* (*this)[13] -  (*this)[6]* (*this)[9]* (*this)[12] -  (*this)[5]* (*this)[8]* (*this)[14] -  (*this)[4]* (*this)[10]* (*this)[13]);
          return m0 - m1 + m2 - m3;

        }
        
        [[nodiscard]] constexpr T determinant() const noexcept requires (N == M && N > 4 && std::is_floating_point_v<T>) {
            T accumulate = 1;
            Matrix temp((*this));

            for (std::size_t i = 0; i < N; i++) {
                std::size_t pivot = i;
                T max_value = std::abs(temp(i,i));
                for (std::size_t j = i + 1; j < N; j++) {
                    T value = std::abs(temp(j,i));
                    if (value > max_value) {
                        max_value = value;
                        pivot = j;
                    }
                }

                if (max_value == T{0}) {
                    return T{0};
                }

                if (pivot != i) {
                    temp.swap_rows_in_place(i, pivot);
                    accumulate *= -1;
                }

                for (std::size_t k = i + 1; k < N; k++) {
                    T scale = -temp(k,i) / temp(i,i);
                    temp.add_scaled_row_in_place(i, scale, k);
                }
            }

            return accumulate * temp.product_of_diagonals();
        }


        [[nodiscard]] constexpr T product_of_diagonals() const noexcept{
            T accumulate = 1;
            for(std::size_t i = 0; i<N; i++){
                accumulate *= (*this)(i,i);
            }
            return accumulate;
        }

        [[nodiscard]] constexpr VectorN<T,N> column(std::size_t column) const noexcept{
            assert(i < N);
            VectorN<T,N> temp;

            for(std::size_t i =0; i<N;i++){
                temp[i] = (*this)(i,column);
            }
            return temp;
        }


        [[nodiscard]] constexpr Matrix<T,M,N> transpose() const noexcept{
            Matrix<T,M,N> temp;

            for(std::size_t i=0; i<N; i++){
                for (std::size_t j=0;j<M;j++){
                    temp(j,i)=(*this)(i,j);
                } 
            }
            return temp;
        }

        constexpr Matrix<T,M,N>& transpose_in_place() noexcept requires(N==M){
 
            for(std::size_t i =0;i<N;i++){
                for(std::size_t j = i+1; j<N; j++){
                    std::swap((*this)(j,i),(*this)(i,j));
                }
            }
            return (*this);

        }

        [[nodiscard]] constexpr Matrix inverse() const noexcept requires (N==M) {
            Matrix temp((*this));
            return temp.inverse_in_place();
        };

        constexpr Matrix& inverse_in_place() noexcept requires (N == 2 && M == 2){

            const T det = determinant();
            assert(det != T{0});

            const T inv_det = T{1} / det;

            std::swap((*this)[0],(*this)[3]);

            (*this)[0] = (*this)[0]*inv_det;
            (*this)[1] *= -inv_det;
            (*this)[2] *= -inv_det;
            (*this)[3] = (*this)[3]*inv_det;
            return (*this);
        }

        constexpr Matrix& inverse_in_place() noexcept requires ((N==3 && M==3) ||(N==4 && M==4) ){
            const T det = determinant();
            assert(det != T{0});

            const T inv_det = T{1} / det;

            Matrix adj = cofactor().transpose_in_place();

            (*this) = adj *inv_det;
            return (*this);

        }


        constexpr Matrix inverse_in_place() noexcept requires(N==M && N >4){
            Matrix temp((*this));

            Matrix inverse = Matrix::identity();

            for (std::size_t i = 0;i<N;i++){
                std::size_t pivot = i;
                T max_value = std::abs(temp(i,i));

                for(std::size_t j = i+1; j<N; j++){
                    T value = std::abs(temp(j,i));
                    if(value > max_value){
                        max_value = value;
                        pivot = j;
                    }
                }

                if(max_value == T{0}){
                    std::fill(inverse.begin(), inverse.end(), T{0});
                    return inverse;
                }

                if(pivot != i){
                    temp.swap_rows_in_place(i, pivot);
                    inverse.swap_rows_in_place(i, pivot);
                }

                for(std::size_t k = i+1; k<N;k++){
                    T scale = -temp(k,i)/temp(i,i);
                    temp.add_scaled_row_in_place(i,scale,k);
                    inverse.add_scaled_row_in_place(i,scale,k);
                }
            }

            for(int l= N-1;l>=0;l--){ 
                for (std::size_t m = 0; m < N; m++) {
                    temp(l,m) /= temp(l,l);
                    inverse(l,m) /=  temp(l,l);
                }


                for (int n = l - 1; n >= 0; n--) {
                    T scale = temp(n,l) / temp(l,l);
                    temp.add_scaled_row_in_place(l, -scale, n);   
                    inverse.add_scaled_row_in_place(l, -scale, n); 
                 }

            }
            return inverse;
        }

        [[nodiscard]] constexpr Matrix<T,M,N> pseudo_inverse() const noexcept requires(N>=M) {
            Matrix<T,M,N> At = transpose();
            Matrix<T,M> AtA = At * (*this);
            if(!AtA.is_invertible()){
              assert(false && "Matrix is not invertible");
              Matrix<T,M,N> A_plus;
              std::fill(A_plus.begin(), A_plus.end(),0);
              return A_plus;
            }
            Matrix<T,M> AtA_inv = AtA.inverse();
            Matrix<T,M,N> A_plus = AtA_inv * At;
            return A_plus;
        }

        [[nodiscard]] constexpr Matrix<T,M,N> pseudo_inverse() const noexcept requires(M>N) {
            Matrix<T,M,N> At = transpose();
            Matrix<T,N> AAt = (*this) * At;
            if(!AAt.is_invertible()){
              assert(false && "Matrix is not invertible");
              Matrix<T,M,N> A_plus;
              std::fill(A_plus.begin(), A_plus.end(),0);
              return A_plus;
            }
            Matrix<T,N> AAt_inv = AAt.inverse();
            Matrix<T,M,N> A_plus = At * AAt_inv;
            return A_plus;
        }
        
        [[nodiscard]] constexpr Matrix adjugate() const noexcept requires(N==M){
            Matrix<T,N> adjugate;
            
            for(std::size_t i = 0; i<N; i++){
                for(std::size_t j = 0; j<N; j++){
                    Matrix<T,N-1,N-1> m = minor(i,j);
                    const T negative = ((i+j)&1) ? T{-1} : T{1};
                    adjugate(j,i) =negative * m.determinant();
                }
            }
            return adjugate;
        }    

        [[nodiscard]] constexpr Matrix cofactor() const noexcept requires(N==M){
            Matrix<T,N> cof = adjugate();
            cof.transpose_in_place(); 
            return cof;
        }
        
        
        template <std::size_t O, std::size_t P>
        [[nodiscard]] constexpr Matrix<T,N,P> operator*(Matrix<T,O,P> rhs) const noexcept requires(O==M){

            Matrix<T,N,P> temp;
            for(std::size_t i =0; i<N; i++){
                for(std::size_t j = 0; j<P; j++){
                    T accumulate = T{0};
                    for(std::size_t k = 0; k<M;k++){
                        accumulate += (*this)(i,k)*rhs(k,j);
                    }
                    temp(i,j) = accumulate;
                }
            }
            return temp;
        }

        template<std::size_t O>
        [[nodiscard]] constexpr VectorN<T,N> operator*(VectorN<T,O> rhs) const noexcept requires(O==M){
            VectorN<T,N> temp;

            for(std::size_t i =0; i<N; i++){
                T accumulate = T{0};
                for(std::size_t j = 0; j<O;j++){
                    accumulate += (*this)(i,j)*rhs[j];
                }
                temp[i]= accumulate;
            }
            return temp;
        }

        [[nodiscard]] constexpr Matrix rref() const noexcept{
            Matrix temp((*this));
            std::size_t row = 0;
            std::size_t col = 0;
            while(row<N && col <M){
                std::size_t pivot = row;
                T max_value = std::abs(temp(row,col));

                for(std::size_t r = row +1; r<N;r++){
                    T value = std::abs(temp(r,col));
                    if(value>max_value){
                        max_value = value;
                        pivot = r;
                    }
                }

                if(max_value == T{0}){
                    col++;
                    continue;
                }

                if(pivot != row){
                    temp.swap_rows_in_place(row,pivot);
                }
                T pivot_value = temp(row,col);
                
                temp.scale_row_in_place(row,1/pivot_value);

                for(std::size_t r = 0; r<N; r++){
                    if(r==row){
                        continue;
                    } 
                    T scale = -temp(r,col);
                    if(scale != temp(r,col)){
                        temp.add_scaled_row_in_place(row, scale, r);
                    }

                }
                row++;
                col++;
            }
            return temp;
        }

        constexpr Matrix rref_in_place() noexcept{
            std::size_t row = 0;
            std::size_t col = 0;
            while(row<N && col <M){
                std::size_t pivot = row;
                T max_value = std::abs((*this)(row,col));

                for(std::size_t r = row +1; r<N;r++){
                    T value = std::abs((*this)(row,col));
                    if(value>max_value){
                        max_value = value;
                        pivot = r;
                    }
                }

                if(max_value == T{0}){
                    col++;
                    continue;
                }

                if(pivot != row){
                    (*this).swap_rows_in_place(row,pivot);
                }
                T pivot_value = (*this)(row,col);
                
                (*this).scale_row_in_place(row,1/pivot_value);

                for(std::size_t r = 0; r<N; r++){
                    if(r==row){
                        continue;
                    } 
                    T scale = -(*this)(r,col);
                    if(scale != (*this)(r,col)){
                        (*this).add_scaled_row_in_place(row, scale, r);
                    }

                }
                row++;
                col++;
            }
            return (*this);
        }

        [[nodiscard]] constexpr Matrix reduce() const noexcept{
            Matrix temp((*this));
            std::size_t row = 0;
            std::size_t col = 0;
            while(row<N && col <M){
                std::size_t pivot = row;
                T max_value = std::abs(temp(row,col));

                for(std::size_t r = row +1; r<N;r++){
                    T value = std::abs(temp(row,col));
                    if(value>max_value){
                        max_value = value;
                        pivot = r;
                    }
                }

                if(max_value == T{0}){
                    col++;
                    continue;
                }

                if(pivot != row){
                    temp.swap_rows_in_place(row,pivot);
                }
                T pivot_value = temp(row,col);
                temp.scale_row_in_place(row,1/pivot_value);

                row++;
                col++;
            }
            return temp;
        }

        constexpr Matrix reduce_in_place() noexcept{
            std::size_t row = 0;
            std::size_t col = 0;
            while(row<N && col <M){
                std::size_t pivot = row;
                T max_value = std::abs((*this)(row,col));

                for(std::size_t r = row +1; r<N;r++){
                    T value = std::abs((*this)(row,col));
                    if(value>max_value){
                        max_value = value;
                        pivot = r;
                    }
                }

                if(max_value == T{0}){
                    col++;
                    continue;
                }

                if(pivot != row){
                    (*this).swap_rows_in_place(row,pivot);
                }
                T pivot_value = (*this)(row,col);
                (*this).scale_row_in_place(row,1/pivot_value);

                row++;
                col++;
            }
            return (*this);
        }
        
        [[nodiscard]] constexpr T trace() const noexcept requires(N == M){
            T accumulate = 0;
            for(std::size_t i = 0; i<N; i++){
                accumulate += (*this)(i,i);
            }
            return accumulate; 
        }

        [[nodiscard]] constexpr Matrix<T,N-1,N-1> minor(std::size_t row, std::size_t col) const noexcept {
            Matrix<T,N-1,N-1> m{};
            std::size_t small_col = 0;
            for (std::size_t k = 0; k < N; k++) {
                if (k == col) {
                continue;
            }
            std::size_t small_row = 0;
            for (std::size_t l = 0; l < N; l++) {
                if (row == l) {
                    continue;
                }
                m(small_row, small_col) = (*this)(l, k);

                small_row++;
            }
            small_col++;
            }
          return m;
        }

        [[nodiscard]] constexpr Matrix map(auto &&func) const noexcept{
            Matrix temp; 
            std::transform(cbegin(),cend(),temp.begin(), func);
            return temp;
        }
        constexpr Matrix& map_in_place(auto &&func) noexcept{
            std::transform(begin(),end(),begin(), func);
            return (*this);
        }
        
        [[nodiscard]] static constexpr Matrix identity() noexcept requires(N==M){
            Matrix temp;
            std::fill(temp.begin(),temp.end(),T{0});
            for(std::size_t i =0;i<N;i++){
                temp(i,i) = 1;
            }
            return temp;
        }
        
        [[nodiscard]] constexpr Matrix orthonormalize() const noexcept{

            std::array<VectorN<T,N>,M> temp_array;
            for(std::size_t i =0;i<M;i++){
                temp_array[i] = column(i);
            }
            temp_array[0].normalize_in_place();
            for (std::size_t i = 1; i < M; ++i) {
                for (std::size_t j = 0; j < i; ++j) {
                    T proj = temp_array[i].dot(temp_array[j]);
                    temp_array[i] -= temp_array[j] * proj;
                 }
                temp_array[i].normalize_in_place();
            }
            Matrix result;
            for (std::size_t c = 0; c < M; ++c){
                 for (std::size_t r = 0; r < N; ++r){
                    result(r, c) = temp_array[c][r];
                 }
            }
            return result;

        }

        [[nodiscard]] constexpr bool is_symmetric() const noexcept{
            if constexpr(N!=M){
                return false;
            }
            if (*this == transpose()){
                return true;
            }
            return false;
        }

        [[nodiscard]] constexpr bool is_orthogonal() const noexcept{
            if constexpr(N!=M){
                return false;
            }
            
            for(std::size_t i = 0; i<N;i++){
                for(std::size_t j = i;j<N; j++){
                    auto first = column(i);
                    auto second = column(j);
                    auto dot = first.dot(second);

                    if(i ==j){
                        if(!math::approx_equal(dot,T{1})){
                            return false;
                        }
                    }
                    else{
                        if(!math::approx_equal(dot,T{0})){
                            return false;
                        }
                    }
                }
                
            }
            return true;
        }

        [[nodiscard]] constexpr bool is_invertible() const noexcept{
            if constexpr(N!=M){
                return false;
            }
            if (determinant() == 0){
                return false;
            }
            return true;
        }


        [[nodiscard]] constexpr Matrix normalize() const noexcept {
            Matrix result = *this;
            result.normalize_in_place(); 
            return result;
        }

        constexpr Matrix& normalize_in_place() noexcept {
            for (std::size_t col = 0; col < M; ++col) {
                VectorN<T,N> vec;
                std::memcpy(&vec[0], &data_[col * N], sizeof(T) * N);
                vec.normalize_in_place();
                std::memcpy(&data_[col * N], &vec[0], sizeof(T) * N);
            }
            return *this;
        }

        [[nodiscard]] constexpr Matrix set_row(std::size_t row, const VectorN<T,M>& vec) const noexcept {
            Matrix result = *this;
            result.set_row_in_place(row, vec);
            return result;
        }

        constexpr Matrix& set_row_in_place(std::size_t row, const VectorN<T,M>& vec) noexcept {
            for (std::size_t col = 0; col < M; ++col) {
                data_[col * N + row] = vec[col];
            }
            return *this;
        }

        [[nodiscard]] constexpr Matrix set_column(std::size_t col, const VectorN<T,N>& vec) const noexcept {
            Matrix result = *this;
            result.set_column_in_place(col, vec);
            return result;
        }

        constexpr Matrix& set_column_in_place(std::size_t col, const VectorN<T,N>& vec) noexcept {
            std::memcpy(&data_[col * N], &vec[0], sizeof(T) * N);
            return *this;
        }


        [[nodiscard]] auto get() noexcept {
            return data_;
        }

        [[nodiscard]] auto get_row_major() {
            return transpose().data_;
        }

        [[nodiscard]] auto get_col_major() {
            return data_;
        }


    };



}