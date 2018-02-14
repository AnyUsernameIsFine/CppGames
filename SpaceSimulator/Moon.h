#pragma once

#include "CoordinateSystem.h"

#include <glm\gtc\random.hpp>

namespace Game
{
	class Moon : public CoordinateSystem
	{
	public:
		Moon(CoordinateSystem* parent, float radius);
	};
}
