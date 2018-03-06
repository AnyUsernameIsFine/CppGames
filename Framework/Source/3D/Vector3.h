#pragma once

#include <glm\vec3.hpp>

#include "System\Globals.h"

namespace Framework
{
	template<typename T>
	class Vector3Type;

	using Vector3 = Vector3Type<float>;

	template<typename T>
	class Vector3Type
	{
	public:
		Vector3Type<T>(T t = 0);
		Vector3Type<T>(T x, T y, T z);
		Vector3Type<T> operator+() const;
		Vector3Type<T> operator-() const;
		Vector3Type<T>& operator+=(const Vector3Type& v);
		Vector3Type<T>& operator-=(const Vector3Type& v);
		Vector3Type<T>& operator*=(const glm::quat& q);
		Vector3Type<T>& operator*=(float f);
		Vector3Type<T>& operator*=(double d);
		Vector3Type<T>& operator/=(float f);
		Vector3Type<T>& operator/=(double d);
		Vector3Type<T> operator+(const Vector3Type& v) const;
		Vector3Type<T> operator-(const Vector3Type& v) const;
		Vector3Type<T> operator*(const glm::quat& q) const;
		Vector3Type<T> operator*(float f) const;
		Vector3Type<T> operator*(double d) const;
		Vector3Type<T> operator/(float f) const;
		Vector3Type<T> operator/(double d) const;
		bool operator==(const Vector3Type& v) const;
		bool operator!=(const Vector3Type& v) const;
		float dot(const Vector3Type& v) const;
		Vector3Type<T> cross(const Vector3Type& v) const;
		float lengthSquared() const;
		float length() const;
		float distanceSquared(const Vector3Type& v) const;
		float distance(const Vector3Type& v) const;
		glm::vec3 toVec3() const;

		static Vector3Type<T> fromVec3(const glm::vec3& v);

		T x, y, z;
	};

	template<typename T>
	Vector3Type<T>::Vector3Type(T t)
	{
		x = t;
		y = t;
		z = t;
	}

	template<typename T>
	Vector3Type<T>::Vector3Type(T x, T y, T z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	template<typename T>
	Vector3Type<T> Vector3Type<T>::operator+() const
	{
		return { x, y, z };
	}

	template<typename T>
	Vector3Type<T> Vector3Type<T>::operator-() const
	{
		return { -x, -y, -z };
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
		glm::vec3 v = glm::inverse(q) * glm::vec3(x, y, z);
		x = static_cast<T>(v.x);
		y = static_cast<T>(v.y);
		z = static_cast<T>(v.z);
		return *this;
	}

	template<typename T>
	Vector3Type<T> Vector3Type<T>::operator+(const Vector3Type& v) const
	{
		return {
			x + v.x,
			y + v.y,
			z + v.z
		};
	}

	template<typename T>
	Vector3Type<T> Vector3Type<T>::operator-(const Vector3Type& v) const
	{
		return {
			x - v.x,
			y - v.y,
			z - v.z
		};
	}

	template<typename T>
	Vector3Type<T> Vector3Type<T>::operator*(const glm::quat& q) const
	{
		// TODO: implement custom calculations for added precision?
		glm::vec3 v = glm::inverse(q) * glm::vec3(x, y, z);
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
	float Vector3Type<T>::lengthSquared() const
	{
		float x = static_cast<float>(this->x);
		float y = static_cast<float>(this->y);
		float z = static_cast<float>(this->z);
		return x * x + y * y + z * z;
	}

	template<typename T>
	float Vector3Type<T>::length() const
	{
		return sqrt(lengthSquared());
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

	template<typename T>
	glm::vec3 Vector3Type<T>::toVec3() const
	{
		return { x, y, z };
	}

	template<typename T>
	Vector3Type<T> Vector3Type<T>::fromVec3(const glm::vec3& v)
	{
		return {
			static_cast<T>(v.x),
			static_cast<T>(v.y),
			static_cast<T>(v.z),
		};
	}
}
