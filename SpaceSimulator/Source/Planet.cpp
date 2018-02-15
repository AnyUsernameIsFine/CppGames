#include "Planet.h"

#include <Framework.hpp>

namespace Game
{
	namespace
	{
#ifdef USE_REALISTIC_SCALE
		const float SCALE = 1.0f / (1 << 14); // A little over 0.06 millimeters per unit.
		// Allows for planets (and the orbits of their moons) with a radius of over 1800 times
		// the distance between the Earth and the Sun (almost 700,000 times the distance
		// between the Moon and the Earth) when using 64-bit integers. Enough precision
		// and size to fit any planetary satellite orbit. 
#else
		const float SCALE = 1.0f / (1 << 7);
#endif
	}

	int Planet::counter_ = 1;

	Planet::Planet(CoordinateSystem* parent, float radius)
	{
		this->parent = parent;
		this->radius = radius;
		this->name = "Planet #" + std::to_string(counter_++);
		this->scale = SCALE;

		addMoons_();
	}

	void Planet::addMoons_()
	{
#ifdef USE_REALISTIC_SCALE
		// Ganymede, the largest moon in our Solar System, which is in
		// orbit around Jupiter has a radius of over 2634 kilometers.
		// Let's make that fit in about twice for some wiggle room.
		float maxRadius = 1000 / SCALE * 5000;
#else
		float maxRadius = (int_least64_t)1 << 55;
#endif

		float r = (float)rand() / RAND_MAX;
		int numberOfMoons = (int)(5 * r * r);

		for (int i = 0; i < numberOfMoons; i++) {
			float r = (float)rand() / RAND_MAX;
			float moonRadius = maxRadius * (0.25f + 0.75f * r * r);

			children.push_back(std::make_unique<Moon>(this, moonRadius));

			Moon* moon = (Moon*)children.back().get();

			glm::vec2 v = glm::diskRand(0.5f * radius * ((CoordinateSystem*)parent)->scale / scale);
			moon->transform.setPosition({ (Coordinate)v.x, 0, (Coordinate)v.y });
			r = (float)rand() / RAND_MAX;
			moon->transform.rotate(45 * r * r, glm::sphericalRand(1.0f));
		}
	}
}
