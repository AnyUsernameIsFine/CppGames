#pragma once

#define GLM_FORCE_INLINE

#include <GLEngine.h>

namespace SpaceSimulator
{
	using namespace GLEngine;

	using Coordinate = Int64;
	using Vector3 = Vector3Type<Coordinate>;
	using Transform = TransformType<Coordinate>;
	using GameObject = GameObjectType<Coordinate>;
}
