#include "Star.h"

#include <Framework.hpp>

namespace Game
{
	namespace
	{
#ifdef USE_REALISTIC_SCALE
		const float SCALE = 1.0f / (1 << 6); // About 1.6 centimers per unit.
		// Allows for stars (and their planetary systems) with a radius of over 480,000 AU
		// or about 7.6 light-years when using 64-bit integers.
		// One AU is the distance from the Earth to the Sun,
		// which is 150 million kilometers, or 8 light-minutes.
		// The largest estimate of the size of the Oort cloud,
		// considered the ultimate edge of our Solar System, is 200,000 AU.
#else
		const float SCALE = (int_least64_t)1 << 42;
#endif
	}

	Star::Star(CoordinateSystem* parent, float radius)
	{
		this->parent = parent;
		this->radius = radius;
		this->name = "Star";
		this->scale = SCALE;

		addPlanets_();
	}

	void Star::addPlanets_()
	{
#ifdef USE_REALISTIC_SCALE
		// Neso, a small moon of Neptune, which has the highest distance
		// from its planet of any natural satellites in our Solar System
		// can be almost 50 million kilometers from Neptune.
		// Let's make its orbit fit in twice for some wiggle room.
		float maxRadius = 1000 / SCALE * 100000000;
#else
		float maxRadius = (int_least64_t)1 << 55;
#endif

		float r = (float)rand() / RAND_MAX;
		int numberOfPlanets = (int)(10 * r * r);

		for (int i = 0; i < numberOfPlanets; i++) {
			float r = (float)rand() / RAND_MAX;
			float planetRadius = maxRadius * (0.25 + 0.75 * r * r);

			children.push_back(std::make_unique<Planet>(this, planetRadius));

			Planet* planet = (Planet*)children.back().get();

			glm::vec2 v = glm::diskRand(radius * 100);
			planet->transform.setPosition(Vector3(v.x, 0, v.y));
			r = (float)rand() / RAND_MAX;
			planet->transform.rotate(45 * r * r, glm::sphericalRand(1));
		}
	}
}
