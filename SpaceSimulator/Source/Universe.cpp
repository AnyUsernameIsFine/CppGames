#include "Universe.h"

#include <Framework.hpp>

#include <ctime>

namespace Game
{
	namespace
	{
		static const int MAX_GALAXIES_IN_A_ROW = 5;
		static const float PERIOD = 100 * Galaxy::MAXIMUM_RADIUS * Galaxy::SCALE / Universe::SCALE;
	}

#ifdef USE_REALISTIC_SCALE
	// About 1.9 light-years per unit.
	// Gives a universe with a radius of over 17.5 million million million light-years
	// when using 64 bit integers. One light-year is 9,460,730,472,580,800 meters.
	const float Universe::SCALE = (int_least64_t)1 << 54;
#else
	const float Universe::SCALE = (int_least64_t)1 << 20;
#endif
	const glm::vec4 Universe::COLOR = { 0, 1, 0, 1 };

	Universe::Universe()
	{
		this->name_ = "Universe";
		this->radius_ = 1;

		this->children_.reserve(MAX_GALAXIES_IN_A_ROW * MAX_GALAXIES_IN_A_ROW * MAX_GALAXIES_IN_A_ROW);
	}

	float Universe::getScale() const
	{
		return SCALE;
	}

	const glm::vec4& Universe::getColor() const
	{
		return COLOR;
	}

	void Universe::create(const Camera& camera)
	{
		Vector3 positionInUniverse = camera.getHierarchy().back().position.x;

		updateCameraPosition_ = Vector3(
			floor(positionInUniverse.x / PERIOD) * PERIOD,
			floor(positionInUniverse.y / PERIOD) * PERIOD,
			floor(positionInUniverse.z / PERIOD) * PERIOD
		);

		addGalaxies_(updateCameraPosition_);
	}

	void Universe::update(const Camera& camera)
	{
		Vector3 positionInUniverse = camera.getHierarchy().back().position;

		Vector3 steppedCameraPosition = Vector3(
			floor(positionInUniverse.x / PERIOD) * PERIOD,
			floor(positionInUniverse.y / PERIOD) * PERIOD,
			floor(positionInUniverse.z / PERIOD) * PERIOD
		);

		if (steppedCameraPosition != updateCameraPosition_) {
			if (steppedCameraPosition.x > updateCameraPosition_.x) {
				addGalaxiesX_(true);
				updateIndex_.x = (updateIndex_.x + 1) % MAX_GALAXIES_IN_A_ROW;
			}
			else if (steppedCameraPosition.x < updateCameraPosition_.x) {
				updateIndex_.x = (updateIndex_.x - 1 + MAX_GALAXIES_IN_A_ROW) % MAX_GALAXIES_IN_A_ROW;
				addGalaxiesX_(false);
			}

			if (steppedCameraPosition.y > updateCameraPosition_.y) {
				addGalaxiesY_(true);
				updateIndex_.y = (updateIndex_.y + 1) % MAX_GALAXIES_IN_A_ROW;
			}
			else if (steppedCameraPosition.y < updateCameraPosition_.y) {
				updateIndex_.y = (updateIndex_.y - 1 + MAX_GALAXIES_IN_A_ROW) % MAX_GALAXIES_IN_A_ROW;
				addGalaxiesY_(false);
			}

			if (steppedCameraPosition.z > updateCameraPosition_.z) {
				addGalaxiesZ_(true);
				updateIndex_.z = (updateIndex_.z + 1) % MAX_GALAXIES_IN_A_ROW;
			}
			else if (steppedCameraPosition.z < updateCameraPosition_.z) {
				updateIndex_.z = (updateIndex_.z - 1 + MAX_GALAXIES_IN_A_ROW) % MAX_GALAXIES_IN_A_ROW;
				addGalaxiesZ_(false);
			}

			updateCameraPosition_ = steppedCameraPosition;
		}
	}

	void Universe::addGalaxiesX_(bool positive)
	{
		int x = positive ? MAX_GALAXIES_IN_A_ROW : -1;

		float maxRadius = Galaxy::MAXIMUM_RADIUS * Galaxy::SCALE / SCALE;

		for (int z = 0; z < MAX_GALAXIES_IN_A_ROW; z++) {
			for (int y = 0; y < MAX_GALAXIES_IN_A_ROW; y++) {

				float r = (float)rand() / RAND_MAX;
				float galaxyRadius = maxRadius * (0.25f + 0.75f * r * r);

				int index = ((0 + updateIndex_.x) % MAX_GALAXIES_IN_A_ROW) + (((y + updateIndex_.y) % MAX_GALAXIES_IN_A_ROW) + ((z + updateIndex_.z) % MAX_GALAXIES_IN_A_ROW) * MAX_GALAXIES_IN_A_ROW) * MAX_GALAXIES_IN_A_ROW;
				children_.at(index).reset(new Galaxy(this, galaxyRadius));
				Galaxy* galaxy = (Galaxy*)children_.at(index).get();

				galaxy->transform.setPosition(updateCameraPosition_ + Vector3(
					(Coordinate)((x + 1 - MAX_GALAXIES_IN_A_ROW * 0.5f) * PERIOD) + 0.75 * (2 * (float)rand() / RAND_MAX - 1) * PERIOD,
					(Coordinate)((y + 1 - MAX_GALAXIES_IN_A_ROW * 0.5f) * PERIOD) + 0.75 * (2 * (float)rand() / RAND_MAX - 1) * PERIOD,
					(Coordinate)((z + 1 - MAX_GALAXIES_IN_A_ROW * 0.5f) * PERIOD) + 0.75 * (2 * (float)rand() / RAND_MAX - 1) * PERIOD
				));

				r = (float)rand() / RAND_MAX;
				galaxy->transform.rotate(360 * r, glm::sphericalRand(1.0f));
			}
		}
	}

	void Universe::addGalaxiesY_(bool positive)
	{
		int y = positive ? MAX_GALAXIES_IN_A_ROW : -1;

		float maxRadius = Galaxy::MAXIMUM_RADIUS * Galaxy::SCALE / SCALE;

		for (int z = 0; z < MAX_GALAXIES_IN_A_ROW; z++) {
			for (int x = 0; x < MAX_GALAXIES_IN_A_ROW; x++) {

				float r = (float)rand() / RAND_MAX;
				float galaxyRadius = maxRadius * (0.25f + 0.75f * r * r);

				int index = ((x + updateIndex_.x) % MAX_GALAXIES_IN_A_ROW) + (((0 + updateIndex_.y) % MAX_GALAXIES_IN_A_ROW) + ((z + updateIndex_.z) % MAX_GALAXIES_IN_A_ROW) * MAX_GALAXIES_IN_A_ROW) * MAX_GALAXIES_IN_A_ROW;
				children_.at(index).reset(new Galaxy(this, galaxyRadius));
				Galaxy* galaxy = (Galaxy*)children_.at(index).get();

				galaxy->transform.setPosition(updateCameraPosition_ + Vector3(
					(Coordinate)((x + 1 - MAX_GALAXIES_IN_A_ROW * 0.5f) * PERIOD) + 0.75 * (2 * (float)rand() / RAND_MAX - 1) * PERIOD,
					(Coordinate)((y + 1 - MAX_GALAXIES_IN_A_ROW * 0.5f) * PERIOD) + 0.75 * (2 * (float)rand() / RAND_MAX - 1) * PERIOD,
					(Coordinate)((z + 1 - MAX_GALAXIES_IN_A_ROW * 0.5f) * PERIOD) + 0.75 * (2 * (float)rand() / RAND_MAX - 1) * PERIOD
				));

				r = (float)rand() / RAND_MAX;
				galaxy->transform.rotate(360 * r, glm::sphericalRand(1.0f));
			}
		}
	}

	void Universe::addGalaxiesZ_(bool positive)
	{
		int z = positive ? MAX_GALAXIES_IN_A_ROW : -1;

		float maxRadius = Galaxy::MAXIMUM_RADIUS * Galaxy::SCALE / SCALE;

		for (int y = 0; y < MAX_GALAXIES_IN_A_ROW; y++) {
			for (int x = 0; x < MAX_GALAXIES_IN_A_ROW; x++) {

				float r = (float)rand() / RAND_MAX;
				float galaxyRadius = maxRadius * (0.25f + 0.75f * r * r);

				int index = ((x + updateIndex_.x) % MAX_GALAXIES_IN_A_ROW) + (((y + updateIndex_.y) % MAX_GALAXIES_IN_A_ROW) + ((0 + updateIndex_.z) % MAX_GALAXIES_IN_A_ROW) * MAX_GALAXIES_IN_A_ROW) * MAX_GALAXIES_IN_A_ROW;
				children_.at(index).reset(new Galaxy(this, galaxyRadius));
				Galaxy* galaxy = (Galaxy*)children_.at(index).get();

				galaxy->transform.setPosition(updateCameraPosition_ + Vector3(
					(Coordinate)((x + 1 - MAX_GALAXIES_IN_A_ROW * 0.5f) * PERIOD) + 0.75 * (2 * (float)rand() / RAND_MAX - 1) * PERIOD,
					(Coordinate)((y + 1 - MAX_GALAXIES_IN_A_ROW * 0.5f) * PERIOD) + 0.75 * (2 * (float)rand() / RAND_MAX - 1) * PERIOD,
					(Coordinate)((z + 1 - MAX_GALAXIES_IN_A_ROW * 0.5f) * PERIOD) + 0.75 * (2 * (float)rand() / RAND_MAX - 1) * PERIOD
				));

				r = (float)rand() / RAND_MAX;
				galaxy->transform.rotate(360 * r, glm::sphericalRand(1.0f));
			}
		}
	}

	void Universe::addGalaxies_(const Vector3& steppedCameraPosition)
	{
		float maxRadius = Galaxy::MAXIMUM_RADIUS * Galaxy::SCALE / SCALE;

		for (int z = 0; z < MAX_GALAXIES_IN_A_ROW; z++) {
			for (int y = 0; y < MAX_GALAXIES_IN_A_ROW; y++) {
				for (int x = 0; x < MAX_GALAXIES_IN_A_ROW; x++) {

					float r = (float)rand() / RAND_MAX;
					float galaxyRadius = maxRadius * (0.25f + 0.75f * r * r);

					children_.push_back(std::make_unique<Galaxy>(this, galaxyRadius));
					Galaxy* galaxy = (Galaxy*)children_.back().get();

					galaxy->transform.setPosition(steppedCameraPosition + Vector3(
						(Coordinate)((x + 1 - MAX_GALAXIES_IN_A_ROW * 0.5f) * PERIOD) + 0.75 * (2 * (float)rand() / RAND_MAX - 1) * PERIOD,
						(Coordinate)((y + 1 - MAX_GALAXIES_IN_A_ROW * 0.5f) * PERIOD) + 0.75 * (2 * (float)rand() / RAND_MAX - 1) * PERIOD,
						(Coordinate)((z + 1 - MAX_GALAXIES_IN_A_ROW * 0.5f) * PERIOD) + 0.75 * (2 * (float)rand() / RAND_MAX - 1) * PERIOD
					));

					r = (float)rand() / RAND_MAX;
					galaxy->transform.rotate(360 * r, glm::sphericalRand(1.0f));
				}
			}
		}
	}

	void Universe::draw(const Camera& camera)
	{
		CoordinateSystem* cs = camera.getCoordinateSystem();

		CoordinateSystem* parentCs = cs->getParent();

		// create and set the projection-view matrix
		float r = 1.0f;
		if (parentCs) {
			r = parentCs->getScale() / cs->getScale();
		}
		glm::mat4 projectionViewMatrix = glm::scale(camera.getProjectionMatrix() * camera.getViewMatrix(true), { r, r, r });
		shaderProgram_->use();
		shaderProgram_->setUniform("projectionViewMatrix", projectionViewMatrix);

		auto hierarchy = camera.getHierarchy();

		// draw the universe
		std::vector<DrawConfiguration> toDrawList;
		drawRecursively_(toDrawList, hierarchy, hierarchy.size() - 1);
		draw_(toDrawList);
	}
}
