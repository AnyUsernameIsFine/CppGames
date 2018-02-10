#pragma once

#include <Framework.hpp>

namespace Game
{
	using namespace Framework;

		typedef float Coordinate;
		//typedef int_least64_t Coordinate;

	class Transform : public Framework::Transform<Coordinate>
	{
	public:
		//Transform(
		//	const Position<Coordinate>& position = { 0, 0, 0 }, 
		//	const glm::vec3& orientation = { 0, 0, 0 }, 
		//	const glm::vec3& scale = { 1, 1, 1 }
		//);
	};
}
