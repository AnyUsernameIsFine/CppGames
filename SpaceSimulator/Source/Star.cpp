#include "Star.h"

#include <Framework.hpp>

namespace Game
{
#ifdef USE_REALISTIC_SCALE
	// About 1.6 centimers per unit.
	// Allows for stars (and their planetary systems) with a radius of over 480,000 AU
	// or about 7.6 light-years when using 64-bit integers.
	// One AU is the distance from the Earth to the Sun,
	// which is 150 million kilometers, or 8 light-minutes.
	// The largest estimate of the size of the Oort cloud,
	// considered the ultimate edge of our Solar System, is 200,000 AU.
	const float Star::SCALE = 1.0f / (1 << 6);

	// The Oort cloud (the outer edge of our Solar System)
	// has a radius as much as 200,000 AU. One AU is 149,597,870,700 meters.
	// Let's make that fit in twice for some wiggle room.
	const float Star::MAX_RADIUS = 149597870700 / SCALE * 200000 * 2;
#else
	const float Star::SCALE = 1.0f * 8;
	const float Star::MAX_RADIUS = (int_least64_t)1 << 62;
#endif
	const glm::vec4 Star::COLOR = { 1, 0, 0, 0.5 };

	Star::Star(CoordinateSystem* parent, float radius)
	{
		this->parent_ = parent;
		this->radius_ = radius;
		name_ = "Star";
	}

	float Star::getScale() const
	{
		return SCALE;
	}

	const glm::vec4& Star::getColor() const
	{
		return COLOR;
	}

	void Star::create()
	{
		addPlanets_();
	}

	void Star::addPlanets_()
	{
		float maxRadius = Planet::MAX_RADIUS * (Planet::SCALE / SCALE);

		int numberOfPlanets = Random::randInt(1, 10);

		for (int i = 0; i < numberOfPlanets; i++) {
			float r = Random::randFloat();
			float planetRadius = maxRadius * (0.25f + 0.75f * r * r);

			auto planet = std::make_shared<Planet>(this, planetRadius);

			glm::vec2 v = glm::diskRand(0.8f * radius_ * parent_->getScale() / getScale());
			planet->transform.setPosition({ (Coordinate)v.x, 0, (Coordinate)v.y });

			r = Random::randFloat();
			planet->transform.rotate(45.0f * r * r, glm::sphericalRand(1.0f));

			planet->create();

			children_.push_back(planet);
		}
	}
}
