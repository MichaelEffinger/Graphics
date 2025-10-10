#pragma once
#include "Vector3.hpp"


#include <stdexcept>





template <typename T> requires std::is_arithmetic_v<T>
Vector3<T>::Vector3(T xPos, T yPos, T zPos) : array{ xPos, yPos, zPos}{}

template <typename T> requires std::is_arithmetic_v<T>
Vector3<T>::Vector3(const Vector3<T> &vecToCopy) : Vector3<T>(vecToCopy.x,vecToCopy.y,vecToCopy.z){}

template <typename T> requires std::is_arithmetic_v<T>
Vector3<T>::Vector3(Vector3<T>&& other) noexcept : array {std::move(other.x),std::move(other.y), std::move(other.z)}{}

template <typename T> requires std::is_arithmetic_v<T>
Vector3<T>& Vector3<T>::operator=(Vector3<T>&& other) noexcept{

	x = std::move(other.x);
	y = std::move(other.y);
	z = std::move(other.z);
	return *this;

}

template <typename T> requires std::is_arithmetic_v<T>
const bool Vector3<T>::operator==(const Vector3<T>& other)const{
	return x == other.x && y == other.y && z == other.z;
}

template <typename T> requires std::is_arithmetic_v<T>
Vector3<T> Vector3<T>::operator+(const Vector3<T>& other) const {

	return Vector3<T>(x + other.x, y + other.y, z + other.z);
}

template <typename T> requires std::is_arithmetic_v<T>
Vector3<T> Vector3<T>::operator-(const Vector3<T>& other) const {
	return Vector3<T>(x - other.x, y - other.y, z - other.z);
}

template <typename T> requires std::is_arithmetic_v<T>
Vector3<T> Vector3<T>::hadamardProduct(const Vector3<T>& other) const {
	return Vector3<T>(x * other.x, y * other.y, z * other.z);
}

template <typename T> requires std::is_arithmetic_v<T>
Vector3<T> Vector3<T>::operator*(const T scalar) const {
	return Vector3<T>(x * scalar, y * scalar, z * scalar);
}

template <typename T> requires std::is_arithmetic_v<T>
Vector3<T> Vector3<T>::hadamardDivide(const Vector3<T>& other) const {

	T newX;
	T newY;
	T newZ;

	if (other.x == 0) {
		newX = std::numeric_limits<T>::max();
	}
	else {
		newX = x / other.x;
	}
	if (other.y == 0) {
		newY = std::numeric_limits<T>::max();
	}
	else {
		newY = y / other.y;
	}
	if (other.z == 0){
		newZ = std::numeric_limits<T>::max();
	}
	else {
		newZ = z / other.z;
	}

	return Vector3<T>(newX, newY, newZ);

}

template <typename T> requires std::is_arithmetic_v<T>
Vector3<T> Vector3<T>::operator/(const T scalar)const {
	if (scalar == 0) {
		T TMax = std::numeric_limits<T>::max();
		return Vector3<T>(TMax, TMax, TMax);
	}
	else {
		return Vector3<T>(x / scalar, y / scalar, z / scalar);
	}
}








template <typename T> requires std::is_arithmetic_v<T>
T Vector3<T>::dotProduct(const Vector3<T>& other) const {

	return (x * other.x) + (y * other.y) + (z*other.z);

}

template <typename T> requires std::is_arithmetic_v<T>
Vector3<T> Vector3<T>::crossProduct(const Vector3<T>& other) const {
	return Vector3((y * other.z) - (z * other.y), (z * other.x) - (x * other.z), (x * other.y) - (y * other.x));
}



template <typename T> requires std::is_arithmetic_v<T>
T& Vector3<T>::operator[](size_t index) {
	if (index < 3) {
		return array[index];
	}
	else {
		throw std::out_of_range("Index out of range");
	}
}
