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
	const int Universe::MAX_GALAXIES_IN_A_ROW_ = 4;
#else
	// About 1.9 light-years per unit.
	// Gives a universe with a radius of almost 9 million million million light-years
	// when using 64 bit integers. One light-year is 9,460,730,472,580,800 meters.
	const float Universe::SCALE = (int_least64_t)1 << 53;
	const int Universe::MAX_GALAXIES_IN_A_ROW_ = 16;
#endif
	const glm::vec4 Universe::COLOR = { 1, 1, 1, 1 };
	const float Universe::PERIOD_ = 100 * Galaxy::MAX_RADIUS * Galaxy::SCALE / Universe::SCALE;

	Universe::Universe()
	{
		radius_ = 1;
		name_ = "Universe";

		children_ = std::vector<std::shared_ptr<CoordinateSystem>>
			(MAX_GALAXIES_IN_A_ROW_ * MAX_GALAXIES_IN_A_ROW_ * MAX_GALAXIES_IN_A_ROW_);
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

	void Universe::create(const Camera& camera)
	{
		Vector3 positionInUniverse = camera.getHierarchy()[0].position;

		updateCameraPosition_ = Vector3(
			(Coordinate)floor(positionInUniverse.x / PERIOD_),
			(Coordinate)floor(positionInUniverse.y / PERIOD_),
			(Coordinate)floor(positionInUniverse.z / PERIOD_)
		);

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

		updateIndex_.x = (updateIndex_.x + offset.x + MAX_GALAXIES_IN_A_ROW_) % MAX_GALAXIES_IN_A_ROW_;
		updateIndex_.y = (updateIndex_.y + offset.y + MAX_GALAXIES_IN_A_ROW_) % MAX_GALAXIES_IN_A_ROW_;
		updateIndex_.z = (updateIndex_.z + offset.z + MAX_GALAXIES_IN_A_ROW_) % MAX_GALAXIES_IN_A_ROW_;

		bool updateAll = offset == Vector3(0) ||
			abs(offset.x) >= MAX_GALAXIES_IN_A_ROW_ ||
			abs(offset.y) >= MAX_GALAXIES_IN_A_ROW_ ||
			abs(offset.z) >= MAX_GALAXIES_IN_A_ROW_;

		int startX = (updateIndex_.x + MAX_GALAXIES_IN_A_ROW_ - (offset.x > 0 ? offset.x : 0)) % MAX_GALAXIES_IN_A_ROW_;
		int startY = (updateIndex_.y + MAX_GALAXIES_IN_A_ROW_ - (offset.y > 0 ? offset.y : 0)) % MAX_GALAXIES_IN_A_ROW_;
		int startZ = (updateIndex_.z + MAX_GALAXIES_IN_A_ROW_ - (offset.z > 0 ? offset.z : 0)) % MAX_GALAXIES_IN_A_ROW_;

		int endX = (startX + abs(offset.x)) % MAX_GALAXIES_IN_A_ROW_;
		int endY = (startY + abs(offset.y)) % MAX_GALAXIES_IN_A_ROW_;
		int endZ = (startZ + abs(offset.z)) % MAX_GALAXIES_IN_A_ROW_;

		bool wrapX = startX > endX;
		bool wrapY = startY > endY;
		bool wrapZ = startZ > endZ;

		int minX = std::min(startX, endX);
		int minY = std::min(startY, endY);
		int minZ = std::min(startZ, endZ);

		int maxX = std::max(startX, endX);
		int maxY = std::max(startY, endY);
		int maxZ = std::max(startZ, endZ);

		for (int z = 0; z < MAX_GALAXIES_IN_A_ROW_; z++) {
			for (int y = 0; y < MAX_GALAXIES_IN_A_ROW_; y++) {
				for (int x = 0; x < MAX_GALAXIES_IN_A_ROW_; x++) {
					if (updateAll ||
						offset.x != 0 && wrapX == (x < minX || x >= maxX) ||
						offset.y != 0 && wrapY == (y < minY || y >= maxY) ||
						offset.z != 0 && wrapZ == (z < minZ || z >= maxZ)) {

						Vector3 p(
							(x + MAX_GALAXIES_IN_A_ROW_ - updateIndex_.x) % MAX_GALAXIES_IN_A_ROW_,
							(y + MAX_GALAXIES_IN_A_ROW_ - updateIndex_.y) % MAX_GALAXIES_IN_A_ROW_,
							(z + MAX_GALAXIES_IN_A_ROW_ - updateIndex_.z) % MAX_GALAXIES_IN_A_ROW_
						);

						Random::setRandSeed(Random::u32FromByteArray(&(updateCameraPosition_ + p), 24));

						float r = Random::randFloat();
						float maxRadius = Galaxy::MAX_RADIUS * Galaxy::SCALE / SCALE;
						float galaxyRadius = maxRadius *(0.25f + 0.75f * r * r);

						auto galaxy = std::make_shared<Galaxy>(this, galaxyRadius);

						glm::vec3 v = {
							updateCameraPosition_.toVec3() +
							glm::vec3(1 - 0.5f * MAX_GALAXIES_IN_A_ROW_) +
							glm::vec3(
								Random::randFloat(-0.375, 0.375),
								Random::randFloat(-0.375, 0.375),
								Random::randFloat(-0.375, 0.375)
							)
						};

						galaxy->transform.setPosition(p * PERIOD_ + v * PERIOD_);

						r = Random::randFloat();
						galaxy->transform.rotate(360 * r, glm::sphericalRand(1.0f));

						galaxy->create();

						children_[x + (y + z * MAX_GALAXIES_IN_A_ROW_) * MAX_GALAXIES_IN_A_ROW_] = galaxy;
					}
				}
			}
		}
	}
}
