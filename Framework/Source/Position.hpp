#pragma once

#include <glm\vec3.hpp>

typedef int_least64_t T;

namespace Game
{
	template<typename T>
	class Position
	{
	public:
		T x, y, z;

		Position<T>(T x = 0, T y = 0, T z = 0);
		Position<T>(const glm::vec3& vec3);
		Position<T>& operator+=(const glm::vec3& vec3);
		const Position<T> operator-(const Position<T>& position) const;
	};
}

//#include "Position.h"

namespace Game
{
	template<typename T>
	Position<T>::Position(T x, T y, T z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	template<typename T>
	Position<T>::Position(const glm::vec3& vec3)
	{
		x = (T)vec3.x;
		y = (T)vec3.y;
		z = (T)vec3.z;
	}

	template<typename T>
	Position<T>& Position<T>::operator+=(const glm::vec3& vec3)
	{
		x += (T)vec3.x;
		y += (T)vec3.y;
		z += (T)vec3.z;
		return *this;
	}

	template<typename T>
	const Position<T> Position<T>::operator-(const Position& position) const
	{
		return Position<T>(x - position.x, y - position.y, z - position.z);
	}
}
