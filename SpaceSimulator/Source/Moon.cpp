#include "Moon.h"

#include <Framework.hpp>

namespace Game
{
#ifdef UNIVERSE_SCALE
	const float Moon::SCALE = 1;
	const float Moon::MAX_RADIUS = (int_least64_t)1 << 62;
#else
	// A little over 0.06 millimeters per unit.
	const float Moon::SCALE = 1.0f / (1 << 14);

	// Ganymede, the largest moon in our Solar System, which is in
	// orbit around Jupiter has a radius of over 2634 kilometers.
	// Let's make that fit in about twice for some wiggle room.
	const float Moon::MAX_RADIUS = 1000 / SCALE * 2634 * 2;
#endif
	const glm::vec4 Moon::COLOR = { 0.5, 0, 1, 0.5 };

	Moon::Moon(CoordinateSystem* parent, float radius)
	{
		this->parent_ = parent;
		this->radius_ = radius;
		name_ = "Moon #" + std::to_string((unsigned int)Random::randInt());
		mesh = new IcosahedronMesh();
	}

	float Moon::getScale() const
	{
		return SCALE;
	}

	const glm::vec4& Moon::getColor() const
	{
		return COLOR;
	}

	float Moon::getCameraNearPlane() const
	{
		return 1;
	}
}
