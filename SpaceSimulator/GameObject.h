#pragma once

#include "Transform.h"

namespace Game
{
	class GameObject
	{
	public:
		GameObject * parent = nullptr;
		Transform transform;

		//void moveX(float distance);
		//void moveY(float distance);
		//void moveZ(float distance);
		//void move(const glm::vec3& vector);
	};
}
