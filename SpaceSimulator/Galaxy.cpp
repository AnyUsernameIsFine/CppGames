#include "Galaxy.h"

#include <Framework.hpp>

namespace Game
{
	namespace
	{
#ifdef USE_REALISTIC_SCALE
		const float SCALE = (int_least32_t)1 << 15; // 32,768 meters per unit.
		// Allows for galaxies with a radius of up to nearly 16 million light-years
		// when using 64-bit integers.
#else
		const float SCALE = (int_least64_t)1 << 49;
#endif
	}

	Galaxy::Galaxy(CoordinateSystem* parent, float radius)
	{
		this->parent = parent;
		this->radius = radius;
		this->name = "Galaxy";
		this->scale = SCALE;

		addStars_();
	}

	void Galaxy::addStars_()
	{
#ifdef USE_REALISTIC_SCALE
		// The Oort cloud (the outer edge of our Solar System)
		// has a radius as much as 200,000 AU. One AU is 149,597,870,700 meters.
		// Let's make that fit in twice for some wiggle room.
		float maxRadius = 149597870700 / SCALE * 400000;
#else
		float maxRadius = (int_least64_t)1 << 55;
#endif

		int numberOfStars = 10 + rand() % 11;
		float r = (float)rand() / RAND_MAX;
		float roundness = r * r * r;

		for (int i = 0; i < numberOfStars; i++) {
			float r = (float)rand() / RAND_MAX;
			float starRadius = maxRadius * (0.25 + 0.75 * r * r);

			children.push_back(std::make_unique<Star>(this, starRadius));

			Star* star = (Star*)children.back().get();

			glm::vec3 v = glm::ballRand(radius * 100);
			star->transform.setPosition(Vector3(v.x, v.y * roundness, v.z));
			r = (float)rand() / RAND_MAX;
			star->transform.rotate(360 * r, glm::sphericalRand(1));
		}
	}
}
