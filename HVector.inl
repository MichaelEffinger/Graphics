#pragma once

#include "HVector.hpp"
#include "iostream"
#include <stdexcept>

template <typename T> requires std::is_arithmetic_v<T>
HVector<T>::HVector(T xPos, T yPos, T zPos, T wPos) : array {xPos,yPos,zPos,wPos} {}


template <typename T> requires std::is_arithmetic_v<T>
HVector<T>::HVector(const HVector<T>& vecToCopy): HVector<T>(vecToCopy.x,vecToCopy.y,vecToCopy.z, vecToCopy.w){}

template <typename T> requires std::is_arithmetic_v<T>
HVector<T>::HVector(HVector<T>&& other) noexcept : array{std::move(other.x),std::move(other.y), std::move(other.z), std::move(other.w)}{}


template <typename T> requires std::is_arithmetic_v<T>
HVector<T>& HVector<T>::operator=(HVector<T>&& other) noexcept{
	x = std::move(other.x);
	y = std::move(other.y);
	z = std::move(other.z);
	w = std::move(other.w);
	return * this;
}


template <typename T>requires std::is_arithmetic_v<T>
HVector<T>& HVector<T>::operator=(const HVector<T>& other){
	x = other.x;
	y = other.y;
	z = other.z;
	w = other.w;
	return *this;
}

template <typename T> requires std::is_arithmetic_v<T>
const bool HVector<T>::operator==(const HVector<T>& other)const{
	return x == other.x && y ==other.y && z == other.z && w == other.w;
}

template <typename T> requires std::is_arithmetic_v<T>
HVector<T> HVector<T>::operator+( const HVector<T>& other) const{

	if(w == 1 && other.w == 1){
		#ifdef DEBUG 
		std::cout << "adding two points not valid, finding midpoint instead \n";
		#endif
		return HVector<T>(x+ other.x, y+other.y,z+other.z,w+other.w).homogenized();
	}
	return HVector<T>(x+other.x,y+other.y,z+other.z,w);


}

template <typename T> requires std::is_arithmetic_v<T>
HVector<T> HVector<T>::operator-(const HVector<T>& other) const{

	if(w == 0 && other.w == 1){
		#ifdef 	DEBUG 
		std::cout << "subtracting a point from a direction makes no geometric sense, returning direction";
		#endif
		return *this;
	}
	return HVector<T>(x-other.x, y-other.y,z-other.z,w-other.w);
}

template <typename T> requires std::is_arithmetic_v<T>
HVector<T> HVector<T>::operator*(const T scalar) const{
	return HVector<T>(x*scalar,y*scalar,z*scalar,w*scalar).homogenized();
}

template <typename T> requires std::is_arithmetic_v<T>
HVector<T> HVector<T>::operator/(const T scalar) const{
	if(scalar == 0){
		#ifdef DEBUG 
		std::cout << "Attempted to divid by zero, returing zero vector";
		#endif
		return HVector<T>(0,0,0,w);
	}
	return HVector<T>(x/scalar,y/scalar,z/scalar,w/scalar).homogenized();
}

template <typename T> requires std::is_arithmetic_v<T>
HVector<T> HVector<T>::hadamardProduct(const HVector<T>& other) const{
	
	return HVector<T>(x*other.x,y*other.y,z*other.z,w*other.w);

}

template <typename T> requires std::is_arithmetic_v<T>
HVector<T> HVector<T>::hadamardDivide(const HVector<T>& other)const{
	

	HVector<T> myVector = HVector<T>(0,0,0,0);
	for(int i =0; i<4; i++){
		if(other.array[i] == 0){
			#ifdef DEBUG 
			std::cout << "Hadamard Divide by zero error";
			#endif
			myVector.array[i] = std::numeric_limits<T>::max();
		}
		else{
			myVector.array[i] = array[i]/other.array[i];
		}
	}
	return myVector;

}

template <typename T> requires std::is_arithmetic_v<T>
HVector<T> HVector<T>::crossProduct(const HVector<T>& other) const{

	#ifdef DEBUG
	if(w == 1 || other.w ==1){
		std::cout << "you are doing a cross product on a point be careful";
	}
	#endif
	return HVector((y * other.z) - (z * other.y), (z * other.x) - (x * other.z), (x * other.y) - (y * other.x),0);

}


template <typename T> requires std::is_arithmetic_v<T>
T HVector<T>::dotProduct(const HVector<T>& other) const{
	return (x * other.x) + (y * other.y) + (z*other.z);
} 

template <typename T> requires std::is_arithmetic_v<T>
T& HVector<T>::operator[](size_t index) {
    if (index < 4) {
        return array[index];
    }
    else {
        throw std::out_of_range("Index out of range");
    }
}
template <typename T> requires std::is_arithmetic_v<T>
const T& HVector<T>::operator[](size_t index) const {
    if (index < 4) {
        return array[index];
    }
    else {
        throw std::out_of_range("Index out of range");
    }
}


template <typename T> requires std::is_arithmetic_v<T>
HVector<T>::operator Vector3<T>() const{
	return Vector3<T>(x,y,z);
}

template <typename T> requires std::is_arithmetic_v<T>
HVector<T>::operator Vector2<T>() const{
	return Vector2<T>(x,y);
}



template <typename T> requires std::is_arithmetic_v<T>
void HVector<T>::homogenize(){
	x =  x/w;
	y = y/w;
	z = z/w;
	w = 1;
}

template <typename T>requires std::is_arithmetic_v<T>
HVector<T> HVector<T>::homogenized() const{
	return HVector<T>(x/w,y/w,z/w,1);
}
