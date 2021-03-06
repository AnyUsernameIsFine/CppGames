#include "Galaxy.h"

#include "Universe.h"

namespace SpaceSimulator
{
#ifdef UNIVERSE_SCALE
#	if UNIVERSE_SCALE == 0
	const float Galaxy::SCALE = 1 << 12;
#	elif UNIVERSE_SCALE == 1
	const float Galaxy::SCALE = 1 << 21;
#	endif
	const float Galaxy::MAX_RADIUS = Int64{ 1 } << 62;
#else
	// 32,768 meters per unit.
	// Allows for galaxies with a radius of up to nearly 16 million light-years
	// when using 64-bit integers.
	const float Galaxy::SCALE = 1 << 15;

	// Abell 2029, the largest known galaxy (cluster)
	// has a maximum radius of about 4 million light-years.
	// Let's make that fit in twice for some wiggle room.
	const float Galaxy::MAX_RADIUS = 9460730472580800 / SCALE * 4000000 * 2;
#endif
	const glm::vec4 Galaxy::COLOR = { 0.2, 0, 1, 0.5 };

	Galaxy::Galaxy(CoordinateSystem* parent, float radius)
	{
		this->parent = parent;
		this->radius = radius;
		name = "Galaxy #" + std::to_string(static_cast<UInt>(Random::randInt()));
		setMesh<CubeMesh>();
	}

	float Galaxy::getScale() const
	{
		return SCALE;
	}

	const glm::vec4& Galaxy::getColor() const
	{
		return COLOR;
	}

	float Galaxy::getCameraNearPlane() const
	{
		return Star::MAX_RADIUS * Star::SCALE / Universe::SCALE;
	}

	void Galaxy::create()
	{
#ifdef UNIVERSE_SCALE
		addStars();
#endif
	}

	void Galaxy::addStars()
	{
		float maxRadius = Star::MAX_RADIUS * (Star::SCALE / SCALE);

		int numStars = Random::randInt(10, 15);
		float r = Random::randFloat();
		float roundness = r * r * r;

		for (int i = 0; i < numStars; i++) {
			float r = Random::randFloat();
			float starRadius = maxRadius * (0.5f + 0.5f * r * r);

			auto star = std::make_shared<Star>(this, starRadius);

			glm::vec3 v = glm::ballRand(0.8f * radius * parent->getScale() / getScale());
			star->transform().setPosition({ static_cast<Coordinate>(v.x), static_cast<Coordinate>(v.y * roundness), static_cast<Coordinate>(v.z) });

			r = Random::randFloat();
			star->transform().rotate(360 * r, glm::sphericalRand(1.0f));

			star->create();

			children.push_back(star);
		}
	}
}
