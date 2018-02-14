#pragma once

#include "Planet.h"

namespace Game
{
	class Star : public CoordinateSystem
	{
	public:
		Star(CoordinateSystem* parent, float radius);

	private:
		static int counter_;

		void addPlanets_();
	};
}
