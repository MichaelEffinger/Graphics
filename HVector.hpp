#pragma once
#include <type_traits>
#include <stdexcept>

#include "Vector2.hpp"
#include "Vector3.hpp"


template <typename T> requires std::is_arithmetic_v<T>
class Vector2;

template <typename T> requires std::is_arithmetic_v<T>
class Vector3;
#include <stdexcept>

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

	HVector(const HVector<T>& vecToCopy);

	HVector(HVector<T>&& source) noexcept;

	HVector<T>& operator=(HVector<T>&& other) noexcept;

	HVector<T>& operator=(const HVector<T>& other);

	const bool operator==(const HVector<T>& other) const;

	HVector<T> operator+(const HVector<T>& other) const;

	HVector<T> operator-(const HVector<T>& other) const;

	HVector<T> operator*(const T scalar) const;
	
	HVector<T> operator/(const T scalar) const;
	
	HVector<T> hadamardProduct(const HVector<T>& other) const;
	
	HVector<T> hadamardDivide(const HVector<T>& other) const;

	HVector<T> crossProduct(const HVector<T>& other) const;
	
	T dotProduct(const HVector<T>& other) const;
	
	const T& operator[](size_t index) const;
	
	T& operator[](size_t index);
	
	explicit operator Vector2<T>() const;
	
	explicit operator Vector3<T>() const;
	
	void homogenize();

	HVector<T> homogenized() const;
	

};


#include "HVector.inl"
