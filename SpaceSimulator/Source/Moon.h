#pragma once

#include "CoordinateSystem.h"

#include <System\Random.hpp>
#include <3D\Meshes\Cube.hpp>
#include <3D\Meshes\Octahedron.hpp>
#include <3D\Meshes\Dodecahedron.hpp>
#include <3D\Meshes\Icosahedron.hpp>

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
