#include "Star.h"
#include "Galaxy.h"

#include <Framework.hpp>

namespace Game
{
#ifdef UNIVERSE_SCALE
#	if UNIVERSE_SCALE == 0
	const float Star::SCALE = (int64)1 << 8;
#	elif UNIVERSE_SCALE == 1
	const float Star::SCALE = (int64)1 << 14;
#	endif
	const float Star::MAX_RADIUS = (int64)1 << 62;
#else
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
#endif
	const glm::vec4 Star::COLOR = { 1, 0.2, 0, 0.5 };

	Star::Star(CoordinateSystem* parent, float radius)
	{
		this->parent = parent;
		this->radius = radius;
		name = "Star #" + std::to_string((uInt)Random::randInt());
		mesh = new OctahedronMesh();
	}

	float Star::getScale() const
	{
		return SCALE;
	}

	const glm::vec4& Star::getColor() const
	{
		return COLOR;
	}

	float Star::getCameraNearPlane() const
	{
		return Planet::MAX_RADIUS * Planet::SCALE / Galaxy::SCALE;
	}

	void Star::create()
	{
		addPlanets();
	}

	void Star::addPlanets()
	{
		float maxRadius = Planet::MAX_RADIUS * (Planet::SCALE / SCALE);

		int numberOfPlanets = Random::randInt(1, 10);

		for (int i = 0; i < numberOfPlanets; i++) {
			float r = Random::randFloat();
			float planetRadius = maxRadius *(0.5f + 0.5f * r * r);

			auto planet = std::make_shared<Planet>(this, planetRadius);

			glm::vec2 v = glm::diskRand(0.8f * radius * parent->getScale() / getScale());
			planet->transform.setPosition({ (Coordinate)v.x, 0, (Coordinate)v.y });

			r = Random::randFloat();
			planet->transform.rotate(45.0f * r * r, glm::sphericalRand(1.0f));

			planet->create();

			children.push_back(planet);
		}
	}
}
