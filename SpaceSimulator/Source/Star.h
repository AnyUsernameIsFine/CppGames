#pragma once

#include "Planet.h"

namespace Game
{
	class Star : public CoordinateSystem
	{
	public:
		Star(CoordinateSystem* parent, float radius);
		glm::vec4 getColor() const { return { 1, 0, 0, 1 }; }

	private:
		static int counter_;

		void addPlanets_();
	};
}
