#pragma once

#include "Star.h"

namespace Game
{
	class Galaxy : public CoordinateSystem
	{
	public:
		Galaxy(CoordinateSystem* parent, float radius);

	private:
		void addStars_();
	};
}
