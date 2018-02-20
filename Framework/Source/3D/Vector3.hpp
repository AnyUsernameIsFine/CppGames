#pragma once

#define GLM_FORCE_INLINE 
#include <glm\vec3.hpp>

namespace Framework
{
	template<typename T>
	class Vector3Type
	{
	public:
		T x, y, z;

		Vector3Type<T>(T t = 0);
		Vector3Type<T>(T x, T y, T z);
		Vector3Type<T>(const glm::vec3& v);
		Vector3Type<T> operator-() const;
		Vector3Type<T>& operator+=(const Vector3Type& v);
		Vector3Type<T>& operator-=(const Vector3Type& v);
		Vector3Type<T>& operator*=(const Vector3Type& v);
		Vector3Type<T>& operator*=(const glm::quat& q);
		Vector3Type<T>& operator*=(float f);
		Vector3Type<T>& operator*=(double d);
		Vector3Type<T>& operator/=(float f);
		Vector3Type<T>& operator/=(double d);
		Vector3Type<T> operator+(const Vector3Type& v) const;
		Vector3Type<T> operator-(const Vector3Type& v) const;
		Vector3Type<T> operator*(const Vector3Type& v) const;
		Vector3Type<T> operator*(const glm::quat& q) const;
		Vector3Type<T> operator*(float f) const;
		Vector3Type<T> operator*(double d) const;
		Vector3Type<T> operator/(float f) const;
		Vector3Type<T> operator/(double d) const;
		bool operator==(const Vector3Type& v) const;
		bool operator!=(const Vector3Type& v) const;
		glm::vec3 toVec3() const;
		float lengthSquared() const;
		float length() const;
		float dot(const Vector3Type& v) const;
		Vector3Type<T> cross(const Vector3Type& v) const;
		float distanceSquared(const Vector3Type& v) const;
		float distance(const Vector3Type& v) const;
	};

	typedef Vector3Type<float> Vector3;
}

//#include "Vector3.h"

namespace Framework
{
	template<typename T>
	Vector3Type<T>::Vector3Type(T t)
	{
		this->x = t;
		this->y = t;
		this->z = t;
	}

	template<typename T>
	Vector3Type<T>::Vector3Type(T x, T y, T z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	template<typename T>
	Vector3Type<T>::Vector3Type(const glm::vec3& v)
	{
		this->x = static_cast<T>(v.x);
		this->y = static_cast<T>(v.y);
		this->z = static_cast<T>(v.z);
	}

	template<typename T>
	Vector3Type<T> Vector3Type<T>::operator-() const
	{
		return {
			-x,
			-y,
			-z,
		};
	}

	template<typename T>
	Vector3Type<T>& Vector3Type<T>::operator+=(const Vector3Type& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	template<typename T>
	Vector3Type<T>& Vector3Type<T>::operator-=(const Vector3Type& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

	template<typename T>
	Vector3Type<T>& Vector3Type<T>::operator*=(const Vector3Type& v)
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
		return *this;
	}

	template<typename T>
	Vector3Type<T>& Vector3Type<T>::operator*=(float f)
	{
		x = static_cast<T>(x * f);
		y = static_cast<T>(y * f);
		z = static_cast<T>(z * f);
		return *this;
	}

	template<typename T>
	Vector3Type<T>& Vector3Type<T>::operator*=(double d)
	{
		x = static_cast<T>(x * d);
		y = static_cast<T>(y * d);
		z = static_cast<T>(z * d);
		return *this;
	}

	template<typename T>
	Vector3Type<T>& Vector3Type<T>::operator/=(float f)
	{
		f = 1 / f;
		x = static_cast<T>(x * f);
		y = static_cast<T>(y * f);
		z = static_cast<T>(z * f);
		return *this;
	}

	template<typename T>
	Vector3Type<T>& Vector3Type<T>::operator/=(double d)
	{
		d = 1 / d;
		x = static_cast<T>(x * d);
		y = static_cast<T>(y * d);
		z = static_cast<T>(z * d);
		return *this;
	}

	template<typename T>
	Vector3Type<T>& Vector3Type<T>::operator*=(const glm::quat& q)
	{
		// TODO: implement custom calculations for added precision?
		glm::vec3 v = glm::inverse(q) * glm::vec3(this->x, this->y, this->z);
		this->x = static_cast<T>(v.x);
		this->y = static_cast<T>(v.y);
		this->z = static_cast<T>(v.z);
		return *this;
	}

	template<typename T>
	Vector3Type<T> Vector3Type<T>::operator+(const Vector3Type& v) const
	{
		return {
			x + v.x,
			y + v.y,
			z + v.z,
		};
	}

	template<typename T>
	Vector3Type<T> Vector3Type<T>::operator-(const Vector3Type& v) const
	{
		return {
			x - v.x,
			y - v.y,
			z - v.z,
		};
	}

	template<typename T>
	Vector3Type<T> Vector3Type<T>::operator*(const Vector3Type& v) const
	{
		return {
			x * v.x,
			y * v.y,
			z * v.z,
		};
	}

	template<typename T>
	Vector3Type<T> Vector3Type<T>::operator*(const glm::quat& q) const
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
	Vector3Type<T> Vector3Type<T>::operator*(float f) const
	{
		return {
			static_cast<T>(x * f),
			static_cast<T>(y * f),
			static_cast<T>(z * f),
		};
	}

	template<typename T>
	Vector3Type<T> Vector3Type<T>::operator*(double d) const
	{
		return {
			static_cast<T>(x * d),
			static_cast<T>(y * d),
			static_cast<T>(z * d),
		};
	}

	template<typename T>
	Vector3Type<T> Vector3Type<T>::operator/(float f) const
	{
		f = 1 / f;
		return {
			static_cast<T>(x * f),
			static_cast<T>(y * f),
			static_cast<T>(z * f),
		};
	}

	template<typename T>
	Vector3Type<T> Vector3Type<T>::operator/(double d) const
	{
		d = 1 / d;
		return {
			static_cast<T>(x * d),
			static_cast<T>(y * d),
			static_cast<T>(z * d),
		};
	}

	template<typename T>
	bool Vector3Type<T>::operator==(const Vector3Type& v) const
	{
		return x == v.x && y == v.x && z == v.z;
	}

	template<typename T>
	bool Vector3Type<T>::operator!=(const Vector3Type& v) const
	{
		return x != v.x || y != v.y || z != v.z;
	}

	template<typename T>
	glm::vec3 Vector3Type<T>::toVec3() const
	{
		return {
			this->x,
			this->y,
			this->z,
		};
	}

	template<typename T>
	float Vector3Type<T>::lengthSquared() const
	{
		return (float)x * (float)x + (float)y * (float)y + (float)z * (float)z;
	}

	template<typename T>
	float Vector3Type<T>::length() const
	{
		return sqrt(lengthSquared());
	}

	template<typename T>
	float Vector3Type<T>::dot(const Vector3Type& v) const
	{
		return x * v.x + y * v.y + z * v.z;
	}

	template<typename T>
	Vector3Type<T> Vector3Type<T>::cross(const Vector3Type& v) const
	{
		return {
			y * v.z - z * v.y,
			z * v.x - x * v.z,
			x * v.y - y * v.x,
		};
	}

	template<typename T>
	float Vector3Type<T>::distanceSquared(const Vector3Type& v) const
	{
		return (*this - v).lengthSquared();
	}

	template<typename T>
	float Vector3Type<T>::distance(const Vector3Type& v) const
	{
		return sqrt(distanceSquared(v));
	}
}
