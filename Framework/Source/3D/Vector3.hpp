#pragma once

#include <glm\vec3.hpp>

namespace Framework
{
	template<typename T = float>
	class Vector3
	{
	public:
		T x, y, z;

		Vector3<T>(T t = 0);
		Vector3<T>(T x, T y, T z);
		Vector3<T>(const glm::vec3& v);
		Vector3<T> operator-() const;
		Vector3<T>& operator+=(const Vector3& v);
		Vector3<T>& operator-=(const Vector3& v);
		Vector3<T>& operator*=(const Vector3& v);
		Vector3<T>& operator*=(const glm::quat& q);
		Vector3<T>& operator*=(float f);
		Vector3<T>& operator*=(double d);
		Vector3<T> operator+(const Vector3& v) const;
		Vector3<T> operator-(const Vector3& v) const;
		Vector3<T> operator*(const Vector3& v) const;
		Vector3<T> operator*(const glm::quat& q) const;
		Vector3<T> operator*(float f) const;
		Vector3<T> operator*(double d) const;
		glm::vec3 toVec3() const;
		float length() const;
		float dot(const Vector3& v) const;
		Vector3<T> cross(const Vector3& v) const;
	};
}

//#include "Vector3.h"

namespace Framework
{
	template<typename T>
	Vector3<T>::Vector3(T t)
	{
		this->x = t;
		this->y = t;
		this->z = t;
	}

	template<typename T>
	Vector3<T>::Vector3(T x, T y, T z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	template<typename T>
	Vector3<T>::Vector3(const glm::vec3& v)
	{
		this->x = static_cast<T>(v.x);
		this->y = static_cast<T>(v.y);
		this->z = static_cast<T>(v.z);
	}

	template<typename T>
	Vector3<T> Vector3<T>::operator-() const
	{
		return {
			-x,
			-y,
			-z,
		};
	}

	template<typename T>
	Vector3<T>& Vector3<T>::operator+=(const Vector3& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	template<typename T>
	Vector3<T>& Vector3<T>::operator-=(const Vector3& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

	template<typename T>
	Vector3<T>& Vector3<T>::operator*=(const Vector3& v)
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
		return *this;
	}

	template<typename T>
	Vector3<T>& Vector3<T>::operator*=(float f)
	{
		x = static_cast<T>(x * f);
		y = static_cast<T>(y * f);
		z = static_cast<T>(z * f);
		return *this;
	}

	template<typename T>
	Vector3<T>& Vector3<T>::operator*=(double d)
	{
		x = static_cast<T>(x * d);
		y = static_cast<T>(y * d);
		z = static_cast<T>(z * d);
		return *this;
	}

	template<typename T>
	Vector3<T>& Vector3<T>::operator*=(const glm::quat& q)
	{
		// TODO: implement custom calculations for added precision?
		glm::vec3 v = glm::inverse(q) * glm::vec3(this->x, this->y, this->z);
		this->x = static_cast<T>(v.x);
		this->y = static_cast<T>(v.y);
		this->z = static_cast<T>(v.z);
		return *this;
	}

	template<typename T>
	Vector3<T> Vector3<T>::operator+(const Vector3& v) const
	{
		return {
			x + v.x,
			y + v.y,
			z + v.z,
		};
	}

	template<typename T>
	Vector3<T> Vector3<T>::operator-(const Vector3& v) const
	{
		return {
			x - v.x,
			y - v.y,
			z - v.z,
		};
	}

	template<typename T>
	Vector3<T> Vector3<T>::operator*(const Vector3& v) const
	{
		return {
			x * v.x,
			y * v.y,
			z * v.z,
		};
	}

	template<typename T>
	Vector3<T> Vector3<T>::operator*(const glm::quat& q) const
	{
		// TODO: implement custom calculations for added precision?
		glm::vec3 v = glm::inverse(q) * glm::vec3(this->x, this->y, this->z);
		return {
			static_cast<T>(v.x),
			static_cast<T>(v.y),
			static_cast<T>(v.z),
		};
	}

	template<typename T>
	Vector3<T> Vector3<T>::operator*(float f) const
	{
		return {
			static_cast<T>(x * f),
			static_cast<T>(y * f),
			static_cast<T>(z * f),
		};
	}

	template<typename T>
	Vector3<T> Vector3<T>::operator*(double d) const
	{
		return {
			static_cast<T>(x * d),
			static_cast<T>(y * d),
			static_cast<T>(z * d),
		};
	}

	template<typename T>
	glm::vec3 Vector3<T>::toVec3() const
	{
		return {
			this->x,
			this->y,
			this->z,
		};
	}

	template<typename T>
	float Vector3<T>::length() const
	{
		// TODO: something about loss of precision?
		return sqrt((float)x * (float)x + (float)y * (float)y + (float)z * (float)z);
	}

	template<typename T>
	float Vector3<T>::dot(const Vector3& v) const
	{
		// TODO: something about loss of precision?
		return this->x * v.x + this->y * v.y + this->z * v.z;
	}

	template<typename T>
	Vector3<T> Vector3<T>::cross(const Vector3& v) const
	{
		return {
			this->y * v.z - this->z * v.y,
			this->z * v.x - this->x * v.z,
			this->x * v.y - this->y * v.x,
		};
	}
}
