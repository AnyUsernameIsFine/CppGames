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
		const float SCALE = (int_least64_t)1 << 7;
#endif
	}

	int Galaxy::counter_ = 1;

	Galaxy::Galaxy(CoordinateSystem* parent, float radius)
	{
		this->parent = parent;
		this->radius = radius;
		this->name = "Galaxy #" + std::to_string(counter_++);
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
			float starRadius = maxRadius * (0.25f + 0.75f * r * r);

			children.push_back(std::make_unique<Star>(this, starRadius));

			Star* star = (Star*)children.back().get();

			glm::vec3 v = glm::ballRand(0.5f * radius * ((CoordinateSystem*)parent)->scale / scale);
			star->transform.setPosition({ (Coordinate)v.x, (Coordinate)(v.y * roundness), (Coordinate)v.z });
			r = (float)rand() / RAND_MAX;
			star->transform.rotate(360 * r, glm::sphericalRand(1.0f));
		}
	}
}
