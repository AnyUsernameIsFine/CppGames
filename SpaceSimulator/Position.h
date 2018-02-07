#pragma once

#include <glm\vec3.hpp>

typedef int_least64_t T;

namespace Game
{
	class Position
	{
	public:
		T x, y, z;

		Position(T x = 0, T y = 0, T z = 0);
		Position(const glm::vec3& vec3);
		Position& operator+=(const glm::vec3& vec3);
		const Position operator-(const Position& position) const;
	};
}
