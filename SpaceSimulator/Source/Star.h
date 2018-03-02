#pragma once

#include "Planet.h"

namespace SpaceSimulator
{
	class Star : public CoordinateSystem
	{
	public:
		static const float SCALE;
		static const float MAX_RADIUS;
		static const glm::vec4 COLOR;

		Star(CoordinateSystem* parent, float radius);
		float getScale() const;
		const glm::vec4& getColor() const;
		float getCameraNearPlane() const;
		void create();

	private:
		void addPlanets();
	};
}
