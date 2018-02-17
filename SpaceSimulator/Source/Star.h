#pragma once

#include "Planet.h"

namespace Game
{
	class Star : public CoordinateSystem
	{
	public:
		static const float SCALE;
		static const float MAXIMUM_RADIUS;
		static const glm::vec4 COLOR;

		Star(CoordinateSystem* parent, float radius);
		glm::vec4 getColor() const;
		const std::vector<std::unique_ptr<CoordinateSystem>>& getChildren() const;

	private:
		static int counter_;
		std::vector<std::unique_ptr<CoordinateSystem>> planets_;

		void addPlanets_();
	};
}
