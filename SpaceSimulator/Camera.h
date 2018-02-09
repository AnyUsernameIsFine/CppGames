#pragma once

#include <Framework.hpp>

#include "Transform.h"

namespace Game
{
	using namespace Framework;

	class CoordinateSystem;

	class Camera : public Framework::Camera<Coordinate>
	{
	public:
		CoordinateSystem* coordinateSystem;
	};
}
