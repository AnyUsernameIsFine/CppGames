#include "Moon.h"

#include <Framework.hpp>

namespace Game
{
	namespace
	{
#ifdef USE_REALISTIC_SCALE
		const float SCALE = 1.0f / (1 << 14); // A little over 0.06 millimeters per unit.
#else
		const float SCALE = (int_least64_t)1 << 28;
#endif
	}

	Moon::Moon(CoordinateSystem* parent, float radius)
	{
		this->parent = parent;
		this->radius = radius;
		this->name = "Moon";
		this->scale = SCALE;
	}
}
