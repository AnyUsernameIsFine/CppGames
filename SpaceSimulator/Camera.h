#pragma once

#include "Transform.h"

namespace Game
{
	class CoordinateSystem;

	class Camera : public Framework::Camera<Coordinate>
	{
	public:
		CoordinateSystem* coordinateSystem;

		void move(const glm::vec3& vector);
		void move(float x, float y, float z);
		void moveX(float distance);
		void moveY(float distance);
		void moveZ(float distance);
		void checkForCoordinateSystemSwap();
	};
}
