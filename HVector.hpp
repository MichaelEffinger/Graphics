#ifndef HVECTOR
#define HVECTOR

#include <type_traits>
#include <stdexcept>

template <typename T> requires std::is_arithmetic_v<T>
class Vector2;

template <typename T> requires std::is_arithmetic_v<T>
class Vector3;


template <typename T> requires std::is_arithmetic_v<T>
class HVector {
private:
	T array[4];
public:
	T& x = array[0];
	T& y = array[1];
	T& z = array[2];
	T& w = array[3];

	HVector(T xPos, T yPos, T zPos, T wPos);

	HVector<T> operator+(const HVector<T>& other) const;

	HVector<T> operator-(const HVector<T>& other) const;


	HVector<T> operator*(const T scalar) const;

	HVector<T> hadamardProduct(const HVector<T>& other) const;

	HVector<T> operator/(const T scalar) const;
	
	HVector<T> hadamardDivide(const HVector<T>& other) const;

	HVector<T> crossProduct(const HVector<T>& other) const;

	T dotProduct(const HVector<T>& other) const;

	T& operator[](size_t index);

	Vector2<T> toVector2();
	Vector3<T> toVector3();








};


#include "HVector.inl"

#endif