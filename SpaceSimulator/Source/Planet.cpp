#include "Planet.h"

#include <Framework.hpp>

namespace Game
{
#ifdef USE_REALISTIC_SCALE
	// A little over 0.06 millimeters per unit.
	// Allows for planets (and the orbits of their moons) with a radius of over 1800 times
	// the distance between the Earth and the Sun (almost 700,000 times the distance
	// between the Moon and the Earth) when using 64-bit integers. Enough precision
	// and size to fit any planetary satellite orbit. 
	const float Planet::SCALE = 1.0f / (1 << 14);

	// Neso, a small moon of Neptune, which has the highest distance
	// from its planet of any natural satellites in our Solar System
	// can be almost 50 million kilometers from Neptune.
	// Let's make its orbit fit in twice for some wiggle room.
	const float Planet::MAX_RADIUS = 1000 / SCALE * 50000000 * 2;
#else
	const float Planet::SCALE = 1.0f / (1 << 7) * 64;
	const float Planet::MAX_RADIUS = (int_least64_t)1 << 62;
#endif
	const glm::vec4 Planet::COLOR = { 1, 1, 0, 1 };

	Planet::Planet(CoordinateSystem* parent, float radius)
	{
		this->parent_ = parent;
		this->radius_ = radius;
		this->name_ = "Planet";
	}

	float Planet::getScale() const
	{
		return SCALE;
	}

	const glm::vec4& Planet::getColor() const
	{
		return COLOR;
	}

	void Planet::create()
	{
		//addMoons_();
	}

	void Planet::addMoons_()
	{
		float maxRadius = Moon::MAX_RADIUS * (Moon::SCALE / SCALE);

		int numberOfMoons = Random::randInt(1, 4);

		for (int i = 0; i < numberOfMoons; i++) {
			float r = Random::randFloat();
			float moonRadius = maxRadius * (0.25f + 0.75f * r * r);

			auto moon = std::make_shared<Moon>(this, moonRadius);

			glm::vec2 v = glm::diskRand(0.5f * radius_ * parent_->getScale() / getScale());
			moon->transform.setPosition({ (Coordinate)v.x, 0, (Coordinate)v.y });

			r = Random::randFloat();
			moon->transform.rotate(45 * r * r, glm::sphericalRand(1.0f));

			children_.push_back(moon);
		}
	}
}
