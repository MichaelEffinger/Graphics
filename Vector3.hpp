#pragma once

#include <type_traits>
#include "Vector2.hpp"

#include "HVector.hpp"




template <typename T> requires std::is_arithmetic_v<T>
class Vector2;

template <typename T> requires std::is_arithmetic_v<T>
class HVector;

template <typename T> requires std::is_arithmetic_v<T>
class Vector3 {
private:
	T array[3];
public:
	T& x = array[0];
	T& y = array[1];
	T& z = array[2];

	Vector3(T xPos, T yPos, T zPos);
	
	Vector3(const Vector3<T>& vecToCopy);

	Vector3(Vector3<T>&& source) noexcept;

	const bool operator==(const Vector3<T>& other) const;

	Vector3<T>& operator=(Vector3<T>&& other) noexcept;

	Vector3<T>& operator=(const Vector3<T>& other);

	Vector3<T> operator+(const Vector3<T>& other) const;

	Vector3<T> operator-(const Vector3<T>& other) const;

	Vector3<T> operator*(const T scalar) const;

	Vector3<T> operator/(const T scalar) const;

	Vector3<T> hadamardProduct(const Vector3<T>& other) const;

	Vector3<T> hadamardDivide(const Vector3<T>& other) const;

	Vector3<T> crossProduct(const Vector3<T>& other) const;

	T dotProduct(const Vector3<T>& other) const;

	const T& operator[](size_t index) const;

	T& operator[](size_t index);
	
	explicit operator Vector2<T>() const;
	
	operator HVector<T>() const;

	HVector<T> toHVector(const T w = {}) const;

};


#include "Vector3.inl"
