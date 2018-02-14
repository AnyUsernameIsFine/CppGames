#pragma once

#include "Planet.h"

namespace Game
{
	class Star : public CoordinateSystem
	{
	public:
		Star(CoordinateSystem* parent, float radius);

	private:
		void addPlanets_();
	};
}
