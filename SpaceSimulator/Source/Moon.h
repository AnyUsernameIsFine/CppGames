#pragma once

#include "CoordinateSystem.h"

#include <glm\gtc\random.hpp>

namespace Game
{
	class Moon : public CoordinateSystem
	{
	public:
		static const float SCALE;
		static const float MAXIMUM_RADIUS;
		static const glm::vec4 COLOR;

		Moon(CoordinateSystem* parent, float radius);
		glm::vec4 getColor() const;
		const std::vector<std::unique_ptr<CoordinateSystem>>& getChildren() const;

	private:
		static int counter_;
	};
}
