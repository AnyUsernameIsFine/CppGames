#include "Position.h"

namespace Game
{
	Position::Position(T x, T y, T z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	Position::Position(const glm::vec3& vec3)
	{
		x = vec3.x;
		y = vec3.y;
		z = vec3.z;
	}

	Position& Position::operator+=(const glm::vec3& vec3)
	{
		x += (T)vec3.x;
		y += (T)vec3.y;
		z += (T)vec3.z;
		return *this;
	}

	const Position Position::operator-(const Position& position) const
	{
		return Position(x - position.x, y - position.y, z - position.z);
	}
}
