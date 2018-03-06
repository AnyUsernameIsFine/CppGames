#pragma once

#define GLM_FORCE_INLINE

#include <Framework.h>

namespace SpaceSimulator
{
	using namespace Framework;

	using Coordinate = Int64;
	using Vector3 = Vector3Type<Coordinate>;
	using Transform = TransformType<Coordinate>;
	using GameObject = GameObjectType<Coordinate>;
}
