#include "Universe.h"

#include <Framework.h>

namespace SpaceSimulator
{
#ifdef UNIVERSE_SCALE
#	if UNIVERSE_SCALE == 0
	const float Universe::SCALE = (int64)1 << 40;
#	elif UNIVERSE_SCALE == 1
	const float Universe::SCALE = (int64)1 << 59;
#	endif
	const int Universe::GALAXIES_PER_SIDE = 4;
#else
	// About 1.9 light-years per unit.
	// Gives a universe with a radius of almost 9 million million million light-years
	// when using 64 bit integers. One light-year is 9,460,730,472,580,800 meters.
	const float Universe::SCALE = (int64)1 << 53;
	const int Universe::GALAXIES_PER_SIDE = 16;
#endif
	const glm::vec4 Universe::COLOR = { 1, 1, 1, 1 };
	const float Universe::PERIOD = 100 * Galaxy::MAX_RADIUS * Galaxy::SCALE / Universe::SCALE;

	Universe::Universe()
	{
		radius = 1;
		name = "Universe";

		children = std::vector<std::shared_ptr<CoordinateSystem>>
			(GALAXIES_PER_SIDE * GALAXIES_PER_SIDE * GALAXIES_PER_SIDE);
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

		updateCameraPosition = Vector3(
			(Coordinate)floor(positionInUniverse.x / PERIOD),
			(Coordinate)floor(positionInUniverse.y / PERIOD),
			(Coordinate)floor(positionInUniverse.z / PERIOD)
		);

		updateIndex = glm::vec3(0);

		addGalaxies();
	}

	void Universe::update(const Camera& camera)
	{
		Vector3 positionInUniverse = camera.getHierarchy()[0].position;

		Vector3 offset = Vector3(
			(Coordinate)floor(positionInUniverse.x / PERIOD),
			(Coordinate)floor(positionInUniverse.y / PERIOD),
			(Coordinate)floor(positionInUniverse.z / PERIOD)
		) - updateCameraPosition;

		if (offset != Vector3(0)) {
			addGalaxies(offset);
		}
	}

	void Universe::draw(const Camera& camera)
	{
		auto hierarchy = camera.getHierarchy();

		std::vector<std::vector<std::vector<DrawConfiguration>>> toDrawList(hierarchy.size(), std::vector<std::vector<DrawConfiguration>>(3));

		drawWithChildren(toDrawList, hierarchy);

		myDraw(toDrawList, camera);
	}

	void Universe::addGalaxies(const Vector3& offset)
	{
		updateCameraPosition += offset;

		updateIndex.x = (updateIndex.x + offset.x + GALAXIES_PER_SIDE) % GALAXIES_PER_SIDE;
		updateIndex.y = (updateIndex.y + offset.y + GALAXIES_PER_SIDE) % GALAXIES_PER_SIDE;
		updateIndex.z = (updateIndex.z + offset.z + GALAXIES_PER_SIDE) % GALAXIES_PER_SIDE;

		bool updateAll = offset == Vector3(0) ||
			abs(offset.x) >= GALAXIES_PER_SIDE ||
			abs(offset.y) >= GALAXIES_PER_SIDE ||
			abs(offset.z) >= GALAXIES_PER_SIDE;

		int startX = (updateIndex.x + GALAXIES_PER_SIDE - (offset.x > 0 ? offset.x : 0)) % GALAXIES_PER_SIDE;
		int startY = (updateIndex.y + GALAXIES_PER_SIDE - (offset.y > 0 ? offset.y : 0)) % GALAXIES_PER_SIDE;
		int startZ = (updateIndex.z + GALAXIES_PER_SIDE - (offset.z > 0 ? offset.z : 0)) % GALAXIES_PER_SIDE;

		int endX = (startX + abs(offset.x)) % GALAXIES_PER_SIDE;
		int endY = (startY + abs(offset.y)) % GALAXIES_PER_SIDE;
		int endZ = (startZ + abs(offset.z)) % GALAXIES_PER_SIDE;

		bool wrapX = startX > endX;
		bool wrapY = startY > endY;
		bool wrapZ = startZ > endZ;

		int minX = std::min(startX, endX);
		int minY = std::min(startY, endY);
		int minZ = std::min(startZ, endZ);

		int maxX = std::max(startX, endX);
		int maxY = std::max(startY, endY);
		int maxZ = std::max(startZ, endZ);

		for (int z = 0; z < GALAXIES_PER_SIDE; z++) {
			for (int y = 0; y < GALAXIES_PER_SIDE; y++) {
				for (int x = 0; x < GALAXIES_PER_SIDE; x++) {
					if (updateAll ||
						offset.x != 0 && wrapX == (x < minX || x >= maxX) ||
						offset.y != 0 && wrapY == (y < minY || y >= maxY) ||
						offset.z != 0 && wrapZ == (z < minZ || z >= maxZ)) {

						Vector3 p(
							(x + GALAXIES_PER_SIDE - updateIndex.x) % GALAXIES_PER_SIDE,
							(y + GALAXIES_PER_SIDE - updateIndex.y) % GALAXIES_PER_SIDE,
							(z + GALAXIES_PER_SIDE - updateIndex.z) % GALAXIES_PER_SIDE
						);

						Random::setRandSeed(Random::uInt32FromByteArray(&(updateCameraPosition + p), 24));

						float r = Random::randFloat();
						float maxRadius = Galaxy::MAX_RADIUS * Galaxy::SCALE / SCALE;
						float galaxyRadius = maxRadius *(0.5f + 0.5f * r * r);

						auto galaxy = std::make_shared<Galaxy>(this, galaxyRadius);

						glm::vec3 v = {
							updateCameraPosition.toVec3() +
							glm::vec3(1 - 0.5f * GALAXIES_PER_SIDE) +
							glm::vec3(
								Random::randFloat(-0.4f, 0.4f),
								Random::randFloat(-0.4f, 0.4f),
								Random::randFloat(-0.4f, 0.4f)
							)
						};

						galaxy->transform.setPosition(p * PERIOD + Vector3::fromVec3(v * PERIOD));

						r = Random::randFloat();
						galaxy->transform.rotate(360 * r, glm::sphericalRand(1.0f));

						galaxy->create();

						children[x + (y + z * GALAXIES_PER_SIDE) * GALAXIES_PER_SIDE] = galaxy;
					}
				}
			}
		}
	}
}
