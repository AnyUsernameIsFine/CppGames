#include "Moon.h"

#include <Framework.hpp>

namespace Game
{
#ifdef USE_REALISTIC_SCALE
	// A little over 0.06 millimeters per unit.
	const float Moon::SCALE = 1.0f / (1 << 14);

	// Ganymede, the largest moon in our Solar System, which is in
	// orbit around Jupiter has a radius of over 2634 kilometers.
	// Let's make that fit in about twice for some wiggle room.
	const float Moon::MAX_RADIUS = 1000 / SCALE * 2634 * 2;
#else
	const float Moon::SCALE = 1.0f / (1 << 14);
	const float Moon::MAX_RADIUS = (int_least64_t)1 << 62;
#endif
	const glm::vec4 Moon::COLOR = { 1, 0, 1, 1 };

	Moon::Moon(CoordinateSystem* parent, float radius)
	{
		this->parent_ = parent;
		this->radius_ = radius;
		this->name_ = "Moon";
	}

	float Moon::getScale() const
	{
		return SCALE;
	}

	const glm::vec4& Moon::getColor() const
	{
		return COLOR;
	}
}
