#pragma once


#include <stdexcept>
#include <type_traits>
#include <utility>

#include "Vector3.hpp"
#include "HVector.hpp"

template <typename T> requires std::is_arithmetic_v<T>
class Vector3;

template <typename T> requires std::is_arithmetic_v<T>
class HVector;


template <typename T> requires std::is_arithmetic_v<T>
class Vector2 {
private:
	T array[2];
public:
	T& x = array[0];
	T& y = array[1];

	Vector2(T xPos, T yPos);

	Vector2(const Vector2<T>& vecToCopy);

	Vector2(Vector2<T>&& source) noexcept;

	Vector2<T>& operator=(Vector2<T>&& other) noexcept;
	
	Vector2<T>& operator=(const Vector2<T>& other);

	const bool operator==(const Vector2<T>& other) const;

	Vector2<T> operator+(const Vector2<T>& other) const;

	Vector2<T> operator-(const Vector2<T>& other) const;

	Vector2<T> operator*(const T scalar) const;

	Vector2<T> operator/(const T scalar) const;

	Vector2<T> hadamardProduct(const Vector2<T>& other) const;

	Vector2<T> hadamardDivide(const Vector2<T>& other) const;
	
	T crossProduct(const Vector2<T>& other) const;
	
	T dotProduct(const Vector2<T>& other)const;

	const T& operator[](size_t index) const;

	T& operator[](size_t index);

	operator Vector3<T>() const;

	Vector3<T> toVector3(const T z={}) const;

	operator HVector<T>() const;

	HVector<T> toHVector(const T z = {}, const T w = {}) const;



};

#include "Vector2.inl"
