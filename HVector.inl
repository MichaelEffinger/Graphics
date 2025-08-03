#ifndef HVECTOR_INL
#define	HVECTOR_INL

#include "HVector.hpp"

template <typename T> requires std::is_arithmetic_v<T>
HVector<T>::HVector(T xPos, T yPos, T zPos, T wPos) {
	x = xPos;
	y = yPos;
	z = zPos;
	w = wPos;
}

template <typename T> requires std::is_arithmetic_v<T>
HVector<T> HVector<T>::operator+(const HVector<T>& other) const {

	return HVector<T>(x + other.x, y + other.y, z + other.z, w + other.w);
}

template <typename T> requires std::is_arithmetic_v<T>
HVector<T> HVector<T>::operator-(const HVector<T>& other) const {
	return HVector<T>(x - other.x, y - other.y, z - other.z, w - other.w);
}

template <typename T> requires std::is_arithmetic_v<T>
HVector<T> HVector<T>::operator*(const T scalar) const {
	return HVector<T>(x * scalar, y * scalar, z * scalar, w * scalar);
}

template <typename T> requires std::is_arithmetic_v<T>
HVector<T> HVector<T>::hadamardProduct(const HVector<T>& other) const {
	return HVector<T>(x * other.x, y * other.y, z * other.z, w * other.w);
}

template <typename T> requires std::is_arithmetic_v<T>
HVector<T> HVector<T>::operator/(const T scalar) const {
	
	if (scalar == 0) {
		T TMax = std::numeric_limits<T>::max();
		return HVector<T>(TMax, TMax,TMax,TMax);
	}
	else {
		return HVector<T>(x / scalar, y / scalar, z / scalar, w / scalar);
	}
}

template <typename T> requires std::is_arithmetic_v<T>
HVector<T> HVector<T>::hadamardDivide(const HVector<T>& other) const {

	T newX;
	T newY;
	T newZ;
	T newW;

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
	if (other.z == 0) {
		newZ = std::numeric_limits<T>::max();
	}
	else {
		newZ = z / other.z;
	}
	if (other.w == 0) {
		newW = std::numeric_limits<T>::max();
	}
	else {
		newW = w / other.w;
	}

	return HVector(newX, newY, newZ, newW);

}

template <typename T> requires std::is_arithmetic_v<T>
Vector2<T> HVector<T>::toVector2() {
	return Vector2<T>(x, y);
}

template <typename T> requires std::is_arithmetic_v<T>
Vector3<T> HVector<T>::toVector3() {
	return Vector3<T>(x, y, z);
}


#endif 