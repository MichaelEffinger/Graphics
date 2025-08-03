#ifndef VECTOR2_INL
#define VECTOR2_INL
#include "Vector2.hpp"


#include <stdexcept>




template <typename T> requires std::is_arithmetic_v<T>
Vector2<T>::Vector2(T xPos, T yPos) : array{ xPos,yPos } {}

template <typename T> requires std::is_arithmetic_v<T>
Vector2<T>::Vector2(const Vector2<T>& vecToCopy) : Vector2<T>(vecToCopy.x,vecToCopy.y) {}


template <typename T> requires std::is_arithmetic_v<T>
Vector2<T>::Vector2(Vector2<T>&& other) noexcept : array{ std::move(other.x), std::move(other.y) } {}


template <typename T> requires std::is_arithmetic_v<T>
Vector2<T>& Vector2<T>::operator=(Vector2<T>&& other) noexcept {
	x = std::move(other.x);
	y = std::move(other.y);
	return *this;
}

template <typename T> requires std::is_arithmetic_v<T>
Vector2<T>& Vector2<T>::operator=(const Vector2<T>& other) {
	x = other.x;
	y = other.y;
	return *this;
}
template <typename T> requires std::is_arithmetic_v<T>
const bool Vector2<T>::operator==(const Vector2<T>& other)const{

	return x == other.x && y == other.y;

}

template <typename T> requires std::is_arithmetic_v<T>
Vector2<T> Vector2<T>::operator+(const Vector2<T>& other) const {
	
	return Vector2<T>(x+other.x, y+other.y);
}

template <typename T> requires std::is_arithmetic_v<T>
Vector2<T> Vector2<T>::operator-(const Vector2<T>& other) const {
	return Vector2<T>(x - other.x, y - other.y);
}

template <typename T> requires std::is_arithmetic_v<T>
Vector2<T> Vector2<T>::hadamardProduct(const Vector2<T>& other) const {
	return Vector2<T>(x * other.x, y * other.y);
}

template <typename T> requires std::is_arithmetic_v<T>
Vector2<T> Vector2<T>::operator*(const T scalar) const {
	return Vector2<T>(x * scalar, y * scalar);
}


template <typename T> requires std::is_arithmetic_v<T>
Vector2<T> Vector2<T>::operator/(const T scalar) const {
	if (scalar == 0) {
		T TMax = std::numeric_limits<T>::max();
		return Vector2<T>(TMax, TMax);
	}
	else {
		return Vector2<T>(x / scalar, y / scalar);
	}
}


template <typename T> requires std::is_arithmetic_v<T>
Vector2<T> Vector2<T>::hadamardDivide(const Vector2<T>& other) const {
	T newX;
	T newY;

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
	return Vector2<T>(newX,newY);
}

template <typename T> requires std::is_arithmetic_v<T>
T Vector2<T>::dotProduct(const Vector2<T>& other) const {

	return (x * other.x) + (y * other.y);

}

template <typename T> requires std::is_arithmetic_v<T>
T Vector2<T>::crossProduct(const Vector2<T>& other) const {
	return (x * other.y) - (y * other.x);
}

template <typename T> requires std::is_arithmetic_v<T>
const T& Vector2<T>::operator[](size_t index) const{
	if (index < 2) {
		return array[index];
	}
	else {
		throw std::out_of_range("Index out of range");
	}
}

template <typename T> requires std::is_arithmetic_v<T>
T& Vector2<T>::operator[](size_t index) {
	if (index < 2) {
		return array[index];
	}
	else {
		throw std::out_of_range("Index out of range");
	}
}

template <typename T> requires std::is_arithmetic_v<T>
Vector2<T>::operator Vector3<T>() const {
	
	return Vector3<T>(x, y, 0);

}

template <typename T> requires std::is_arithmetic_v<T>
Vector3<T> Vector2<T>::toVector3(const T z) const {
	return Vector3<T>(x, y, z);
}


template <typename T> requires std::is_arithmetic_v<T>
Vector2<T>::operator HVector<T>() const {
	return HVector<T>(x, y, 0, 1);
}

template <typename T> requires std::is_arithmetic_v<T>
HVector<T> Vector2<T>::toHVector(T z, T w) const {
	return HVector<T>(x, y, z, w);
}



#endif