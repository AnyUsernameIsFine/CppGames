#pragma once

#include "CoordinateSystem.h"

#include <glm\gtc\random.hpp>

namespace Game
{
	class Moon : public CoordinateSystem
	{
	public:
		static int counter_;

		Moon(CoordinateSystem* parent, float radius);
	};
}
