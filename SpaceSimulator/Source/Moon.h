#pragma once

#include "CoordinateSystem.h"

#include <System\Random.h>
#include <3D\Meshes\Cube.h>
#include <3D\Meshes\Octahedron.h>
#include <3D\Meshes\Dodecahedron.h>
#include <3D\Meshes\Icosahedron.h>

#include <glm\gtc\random.hpp>

namespace SpaceSimulator
{
	class Moon : public CoordinateSystem
	{
	public:
		static const float SCALE;
		static const float MAX_RADIUS;
		static const glm::vec4 COLOR;

		Moon(CoordinateSystem* parent, float radius);
		float getScale() const;
		const glm::vec4& getColor() const;
		float getCameraNearPlane() const;
	};
}
