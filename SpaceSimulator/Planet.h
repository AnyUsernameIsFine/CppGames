#pragma once

#include "Moon.h"

namespace Game
{
	class Planet : public CoordinateSystem
	{
	public:
		Planet(CoordinateSystem* parent, float radius);

	private:
		void addMoons_();
	};
}
