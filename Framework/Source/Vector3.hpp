#pragma once

#include <glm\vec3.hpp>

namespace Framework
{
	template<typename T = float>
	class Vector3
	{
	public:
		T x, y, z;

		Vector3<T>(T x = 0, T y = 0, T z = 0);
		const Vector3<T> operator-() const;
		Vector3<T>& operator+=(const Vector3& v);
		Vector3<T>& operator-=(const Vector3& v);
		Vector3<T>& operator*=(const Vector3& v);
		Vector3<T>& operator*=(const glm::quat& q);
		Vector3<T>& operator*=(float f);
		const Vector3<T> operator+(const Vector3& v) const;
		const Vector3<T> operator-(const Vector3& v) const;
		const Vector3<T> operator*(const Vector3& v) const;
		const Vector3<T> operator*(const glm::quat& q) const;
		const Vector3<T> operator*(float f) const;
		const float length() const;
		static const float dot(const Vector3& v1, const Vector3& v2);
		static const Vector3<T> cross(const Vector3& v1, const Vector3& v2);
	};
}

//#include "Vector3.h"

namespace Framework
{
	template<typename T>
	Vector3<T>::Vector3(T x, T y, T z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	template<typename T>
	const Vector3<T> Vector3<T>::operator-() const
	{
		return { -x, -y, -z };
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
		x *= f;
		y *= f;
		z *= f;
		return *this;
	}

	template<typename T>
	Vector3<T>& Vector3<T>::operator*=(const glm::quat& q)
	{
		// TODO: implement custom calculations for added precision?
		glm::vec3 v = glm::inverse(q) * glm::vec3(this->x, this->y, this->z);
		this->x = v.x;
		this->y = v.y;
		this->z = v.z;
		return *this;
	}

	template<typename T>
	const Vector3<T> Vector3<T>::operator+(const Vector3& v) const
	{
		return { x + v.x, y + v.y, z + v.z };
	}

	template<typename T>
	const Vector3<T> Vector3<T>::operator-(const Vector3& v) const
	{
		return { x - v.x, y - v.y, z - v.z };
	}

	template<typename T>
	const Vector3<T> Vector3<T>::operator*(const Vector3& v) const
	{
		return { x * v.x, y * v.y, z * v.z };
	}

	template<typename T>
	const Vector3<T> Vector3<T>::operator*(const glm::quat& q) const
	{
		// TODO: implement custom calculations for added precision?
		glm::vec3 v = glm::inverse(q) * glm::vec3(this->x, this->y, this->z);
		return { v.x, v.y, v.z };
	}

	template<typename T>
	const Vector3<T> Vector3<T>::operator*(float f) const
	{
		return { x * f, y * f, z * f };
	}

	template<typename T>
	const float Vector3<T>::length() const
	{
		return sqrtf((float)x * x + (float)y * y + (float)z * z);
	}

	template<typename T>
	static const float Vector3<T>::dot(const Vector3& v1, const Vector3& v2)
	{
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}

	template<typename T>
	static const Vector3<T> Vector3<T>::cross(const Vector3& v1, const Vector3& v2)
	{
		return {
			v1.y * v2.z - v1.z * v2.y,
			v1.z * v2.x - v1.x * v2.z,
			v1.x * v2.y - v1.y * v2.x,
		};
	}
}
