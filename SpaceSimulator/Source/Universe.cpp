#include "Universe.h"

#include <Framework.hpp>

#include <algorithm>

namespace Game
{
#ifdef UNIVERSE_SCALE
#	if UNIVERSE_SCALE == 0
	const float Universe::SCALE = (int_least64_t)1 << 40;
#	elif UNIVERSE_SCALE == 1
	const float Universe::SCALE = (int_least64_t)1 << 59;
#	endif
	const int Universe::GALAXIES_PER_SIDE_ = 4;
#else
	// About 1.9 light-years per unit.
	// Gives a universe with a radius of almost 9 million million million light-years
	// when using 64 bit integers. One light-year is 9,460,730,472,580,800 meters.
	const float Universe::SCALE = (int_least64_t)1 << 53;
	const int Universe::GALAXIES_PER_SIDE_ = 16;
#endif
	const glm::vec4 Universe::COLOR = { 1, 1, 1, 1 };
	const float Universe::PERIOD_ = 100 * Galaxy::MAX_RADIUS * Galaxy::SCALE / Universe::SCALE;

	Universe::Universe()
	{
		radius_ = 1;
		name_ = "Universe";

		children_ = std::vector<std::shared_ptr<CoordinateSystem>>
			(GALAXIES_PER_SIDE_ * GALAXIES_PER_SIDE_ * GALAXIES_PER_SIDE_);
	}

	float Universe::getScale() const
	{
		return SCALE;
	}

	const glm::vec4& Universe::getColor() const
	{
		return COLOR;
	}

	float Universe::getCameraNearPlane() const
	{
		return Galaxy::MAX_RADIUS * Galaxy::SCALE / SCALE;
	}

	void Universe::create()
	{
		Vector3 positionInUniverse(0);

		updateCameraPosition_ = Vector3(
			(Coordinate)floor(positionInUniverse.x / PERIOD_),
			(Coordinate)floor(positionInUniverse.y / PERIOD_),
			(Coordinate)floor(positionInUniverse.z / PERIOD_)
		);

		updateIndex_ = glm::vec3(0);

		addGalaxies_();
	}

	void Universe::update(const Camera& camera)
	{
		Vector3 positionInUniverse = camera.getHierarchy()[0].position;

		Vector3 offset = Vector3(
			(Coordinate)floor(positionInUniverse.x / PERIOD_),
			(Coordinate)floor(positionInUniverse.y / PERIOD_),
			(Coordinate)floor(positionInUniverse.z / PERIOD_)
		) - updateCameraPosition_;

		if (offset != Vector3(0)) {
			addGalaxies_(offset);
		}
	}

	void Universe::draw(const Camera& camera, float totalSeconds)
	{
		auto hierarchy = camera.getHierarchy();

		std::vector<std::vector<std::vector<DrawConfiguration>>> toDrawList(hierarchy.size(), std::vector<std::vector<DrawConfiguration>>(3));

		drawWithChildren_(toDrawList, hierarchy);

		draw_(toDrawList, camera, totalSeconds);
	}

	void Universe::addGalaxies_(const Vector3& offset)
	{
		updateCameraPosition_ += offset;

		updateIndex_.x = (updateIndex_.x + offset.x + GALAXIES_PER_SIDE_) % GALAXIES_PER_SIDE_;
		updateIndex_.y = (updateIndex_.y + offset.y + GALAXIES_PER_SIDE_) % GALAXIES_PER_SIDE_;
		updateIndex_.z = (updateIndex_.z + offset.z + GALAXIES_PER_SIDE_) % GALAXIES_PER_SIDE_;

		bool updateAll = offset == Vector3(0) ||
			abs(offset.x) >= GALAXIES_PER_SIDE_ ||
			abs(offset.y) >= GALAXIES_PER_SIDE_ ||
			abs(offset.z) >= GALAXIES_PER_SIDE_;

		int startX = (updateIndex_.x + GALAXIES_PER_SIDE_ - (offset.x > 0 ? offset.x : 0)) % GALAXIES_PER_SIDE_;
		int startY = (updateIndex_.y + GALAXIES_PER_SIDE_ - (offset.y > 0 ? offset.y : 0)) % GALAXIES_PER_SIDE_;
		int startZ = (updateIndex_.z + GALAXIES_PER_SIDE_ - (offset.z > 0 ? offset.z : 0)) % GALAXIES_PER_SIDE_;

		int endX = (startX + abs(offset.x)) % GALAXIES_PER_SIDE_;
		int endY = (startY + abs(offset.y)) % GALAXIES_PER_SIDE_;
		int endZ = (startZ + abs(offset.z)) % GALAXIES_PER_SIDE_;

		bool wrapX = startX > endX;
		bool wrapY = startY > endY;
		bool wrapZ = startZ > endZ;

		int minX = std::min(startX, endX);
		int minY = std::min(startY, endY);
		int minZ = std::min(startZ, endZ);

		int maxX = std::max(startX, endX);
		int maxY = std::max(startY, endY);
		int maxZ = std::max(startZ, endZ);

		for (int z = 0; z < GALAXIES_PER_SIDE_; z++) {
			for (int y = 0; y < GALAXIES_PER_SIDE_; y++) {
				for (int x = 0; x < GALAXIES_PER_SIDE_; x++) {
					if (updateAll ||
						offset.x != 0 && wrapX == (x < minX || x >= maxX) ||
						offset.y != 0 && wrapY == (y < minY || y >= maxY) ||
						offset.z != 0 && wrapZ == (z < minZ || z >= maxZ)) {

						Vector3 p(
							(x + GALAXIES_PER_SIDE_ - updateIndex_.x) % GALAXIES_PER_SIDE_,
							(y + GALAXIES_PER_SIDE_ - updateIndex_.y) % GALAXIES_PER_SIDE_,
							(z + GALAXIES_PER_SIDE_ - updateIndex_.z) % GALAXIES_PER_SIDE_
						);

						Random::setRandSeed(Random::ui32FromByteArray(&(updateCameraPosition_ + p), 24));

						float r = Random::randFloat();
						float maxRadius = Galaxy::MAX_RADIUS * Galaxy::SCALE / SCALE;
						float galaxyRadius = maxRadius *(0.5f + 0.5f * r * r);

						auto galaxy = std::make_shared<Galaxy>(this, galaxyRadius);

						glm::vec3 v = {
							updateCameraPosition_.toVec3() +
							glm::vec3(1 - 0.5f * GALAXIES_PER_SIDE_) +
							glm::vec3(
								Random::randFloat(-0.4f, 0.4f),
								Random::randFloat(-0.4f, 0.4f),
								Random::randFloat(-0.4f, 0.4f)
							)
						};

						galaxy->transform.setPosition(p * PERIOD_ + v * PERIOD_);

						r = Random::randFloat();
						galaxy->transform.rotate(360 * r, glm::sphericalRand(1.0f));

						galaxy->create();

						children_[x + (y + z * GALAXIES_PER_SIDE_) * GALAXIES_PER_SIDE_] = galaxy;
					}
				}
			}
		}
	}
}
