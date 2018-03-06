#include "Planet.h"

#include "Star.h"

namespace SpaceSimulator
{
#ifdef UNIVERSE_SCALE
#	if UNIVERSE_SCALE == 0
	const float Planet::SCALE = 1 << 4;
#	elif UNIVERSE_SCALE == 1
	const float Planet::SCALE = 1 << 7;
#	endif
	const float Planet::MAX_RADIUS = Int64{ 1 } << 62;
#else
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
#endif
	const glm::vec4 Planet::COLOR = { 1, 1, 0, 0.5 };

	Planet::Planet(CoordinateSystem* parent, float radius)
	{
		this->parent = parent;
		this->radius = radius;
		name = "Planet #" + std::to_string(static_cast<UInt>(Random::randInt()));
		setMesh<DodecahedronMesh>();
	}

	float Planet::getScale() const
	{
		return SCALE;
	}

	const glm::vec4& Planet::getColor() const
	{
		return COLOR;
	}

	float Planet::getCameraNearPlane() const
	{
		return Moon::MAX_RADIUS * Moon::SCALE / Star::SCALE;
	}

	void Planet::create()
	{
		addMoons();
	}

	void Planet::addMoons()
	{
		float maxRadius = Moon::MAX_RADIUS * (Moon::SCALE / SCALE);

		int numMoons = Random::randInt(1, 4);

		for (int i = 0; i < numMoons; i++) {
			float r = Random::randFloat();
			float moonRadius = maxRadius * (0.5f + 0.5f * r * r);

			auto moon = std::make_shared<Moon>(this, moonRadius);

			glm::vec2 v = glm::diskRand(0.8f * radius * parent->getScale() / getScale());
			moon->transform().setPosition(Vector3::fromVec3({ v.x, v.y, 0 }));

			r = Random::randFloat();
			moon->transform().rotate(45 * r * r, glm::sphericalRand(1.0f));

			children.push_back(moon);
		}
	}
}
