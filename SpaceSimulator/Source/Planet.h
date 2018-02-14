#pragma once

#include "Moon.h"

namespace Game
{
	class Planet : public CoordinateSystem
	{
	public:
		Planet(CoordinateSystem* parent, float radius);

	private:
		static int counter_;

		void addMoons_();
	};
}
