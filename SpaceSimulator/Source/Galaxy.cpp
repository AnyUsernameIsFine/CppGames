#include "Galaxy.h"

#include <Framework.hpp>

namespace Game
{
#ifdef USE_REALISTIC_SCALE
	// 32,768 meters per unit.
	// Allows for galaxies with a radius of up to nearly 16 million light-years
	// when using 64-bit integers.
	const float Galaxy::SCALE = 1 << 15;

	// Abell 2029, the largest known galaxy (cluster)
	// has a maximum radius of about 4 million light-years.
	// Let's make that fit in twice for some wiggle room.
	const float Galaxy::MAXIMUM_RADIUS = 9460730472580800 / SCALE * 4000000 * 2;
#else
	const float Galaxy::SCALE = (int_least64_t)1 << 7;
	const float Galaxy::MAXIMUM_RADIUS = (int_least64_t)1 << 62;
#endif
	const glm::vec4 Galaxy::COLOR = { 0, 0, 1, 1 };
	int Galaxy::counter_ = 1;

	Galaxy::Galaxy(CoordinateSystem* parent, float radius)
	{
		this->parent_ = parent;
		this->radius_ = radius;
		this->name_ = "Galaxy #" + std::to_string(counter_++);

		//addStars_();
	}

	float Galaxy::getScale() const
	{
		return SCALE;
	}

	const glm::vec4& Galaxy::getColor() const
	{
		return COLOR;
	}

	void Galaxy::addStars_()
	{
		float maxRadius = Star::MAXIMUM_RADIUS * (Star::SCALE / SCALE);

		int numberOfStars = 10 + rand() % 11;
		float r = (float)rand() / RAND_MAX;
		float roundness = r * r * r;

		for (int i = 0; i < numberOfStars; i++) {
			float r = (float)rand() / RAND_MAX;
			float starRadius = maxRadius * (0.25f + 0.75f * r * r);

			children_.push_back(std::make_unique<Star>(this, starRadius));

			Star* star = (Star*)children_.back().get();

			glm::vec3 v = glm::ballRand(0.5f * radius_ * parent_->getScale() / getScale());
			star->transform.setPosition({ (Coordinate)v.x, (Coordinate)(v.y * roundness), (Coordinate)v.z });
			r = (float)rand() / RAND_MAX;
			star->transform.rotate(360 * r, glm::sphericalRand(1.0f));
		}
	}
}
