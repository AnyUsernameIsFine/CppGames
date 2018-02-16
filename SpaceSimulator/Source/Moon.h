#pragma once

#include "CoordinateSystem.h"

#include <glm\gtc\random.hpp>

namespace Game
{
	class Moon : public CoordinateSystem
	{
	public:
		Moon(CoordinateSystem* parent, float radius);
		glm::vec4 getColor() const { return { 1, 0, 1, 1 }; }

	private:
		static int counter_;
	};
}
