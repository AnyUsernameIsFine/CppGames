#pragma once

#include <Framework.h>

#define GLM_FORCE_INLINE

namespace SpaceSimulator
{
	using namespace Framework;

	typedef int64 Coordinate;
	typedef Vector3Type<Coordinate> Vector3;
	typedef TransformType<Coordinate> Transform;
	typedef GameObjectType<Coordinate> GameObject;
}
