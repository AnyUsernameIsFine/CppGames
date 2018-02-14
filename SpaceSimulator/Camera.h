#pragma once

#include "Transform.h"

namespace Game
{
	class CoordinateSystem;

	class Camera : public Framework::Camera<Coordinate>
	{
	public:
		CoordinateSystem* coordinateSystem;

		void checkForCoordinateSystemSwap();
	};
}
