#include "Universe.h"

#include <Framework.hpp>

#include <ctime>

namespace Game
{
	const int Universe::MAX_GALAXIES_IN_A_ROW_ = 15;
	const float Universe::PERIOD_ = 100 * Galaxy::MAX_RADIUS * Galaxy::SCALE / Universe::SCALE;

#ifdef USE_REALISTIC_SCALE
	// About 1.9 light-years per unit.
	// Gives a universe with a radius of over 17.5 million million million light-years
	// when using 64 bit integers. One light-year is 9,460,730,472,580,800 meters.
	const float Universe::SCALE = (int_least64_t)1 << 54;
#else
	const float Universe::SCALE = (int_least64_t)1 << 20;
#endif
	const glm::vec4 Universe::COLOR = { 0, 0, 0, 1 };

	Universe::Universe()
	{
		this->radius_ = 1;
		this->name_ = "Universe";

		this->children_.reserve(MAX_GALAXIES_IN_A_ROW_ * MAX_GALAXIES_IN_A_ROW_ * MAX_GALAXIES_IN_A_ROW_);
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
			(Coordinate)floor(positionInUniverse.x / PERIOD_),
			(Coordinate)floor(positionInUniverse.y / PERIOD_),
			(Coordinate)floor(positionInUniverse.z / PERIOD_)
		);

		addGalaxies_();
	}

	void Universe::update(const Camera& camera)
	{
		Vector3 positionInUniverse = camera.getHierarchy().back().position;

		Vector3 steppedCameraPosition = Vector3(
			(Coordinate)floor(positionInUniverse.x / PERIOD_),
			(Coordinate)floor(positionInUniverse.y / PERIOD_),
			(Coordinate)floor(positionInUniverse.z / PERIOD_)
		);

		if (steppedCameraPosition != updateCameraPosition_) {
			if (steppedCameraPosition.x > updateCameraPosition_.x) {
				addGalaxiesX_(true);
				updateIndex_.x = (updateIndex_.x + 1) % MAX_GALAXIES_IN_A_ROW_;
			}
			else if (steppedCameraPosition.x < updateCameraPosition_.x) {
				updateIndex_.x = (updateIndex_.x - 1 + MAX_GALAXIES_IN_A_ROW_) % MAX_GALAXIES_IN_A_ROW_;
				addGalaxiesX_(false);
			}

			if (steppedCameraPosition.y > updateCameraPosition_.y) {
				addGalaxiesY_(true);
				updateIndex_.y = (updateIndex_.y + 1) % MAX_GALAXIES_IN_A_ROW_;
			}
			else if (steppedCameraPosition.y < updateCameraPosition_.y) {
				updateIndex_.y = (updateIndex_.y - 1 + MAX_GALAXIES_IN_A_ROW_) % MAX_GALAXIES_IN_A_ROW_;
				addGalaxiesY_(false);
			}

			if (steppedCameraPosition.z > updateCameraPosition_.z) {
				addGalaxiesZ_(true);
				updateIndex_.z = (updateIndex_.z + 1) % MAX_GALAXIES_IN_A_ROW_;
			}
			else if (steppedCameraPosition.z < updateCameraPosition_.z) {
				updateIndex_.z = (updateIndex_.z - 1 + MAX_GALAXIES_IN_A_ROW_) % MAX_GALAXIES_IN_A_ROW_;
				addGalaxiesZ_(false);
			}

			updateCameraPosition_ = steppedCameraPosition;
		}
	}

	void Universe::addGalaxiesX_(bool positive)
	{
		Vector3 offset(positive ? MAX_GALAXIES_IN_A_ROW_ : -1, 0, 0);

		for (int z = 0; z < MAX_GALAXIES_IN_A_ROW_; z++) {
			for (int y = 0; y < MAX_GALAXIES_IN_A_ROW_; y++) {
				for (int x = 0; x < 1; x++) {
					addGalaxy_(offset, x, y, z);
				}
			}
		}
	}

	void Universe::addGalaxiesY_(bool positive)
	{
		Vector3 offset(0, positive ? MAX_GALAXIES_IN_A_ROW_ : -1, 0);

		for (int z = 0; z < MAX_GALAXIES_IN_A_ROW_; z++) {
			for (int y = 0; y < 1; y++) {
				for (int x = 0; x < MAX_GALAXIES_IN_A_ROW_; x++) {
					addGalaxy_(offset, x, y, z);
				}
			}
		}
	}

	void Universe::addGalaxiesZ_(bool positive)
	{
		Vector3 offset(0, 0, positive ? MAX_GALAXIES_IN_A_ROW_ : -1);

		for (int z = 0; z < 1; z++) {
			for (int y = 0; y < MAX_GALAXIES_IN_A_ROW_; y++) {
				for (int x = 0; x < MAX_GALAXIES_IN_A_ROW_; x++) {
					addGalaxy_(offset, x, y, z);
				}
			}
		}
	}

	void Universe::addGalaxy_(const Vector3& offset, int x, int y, int z)
	{
		int index =
			((x + updateIndex_.x) % MAX_GALAXIES_IN_A_ROW_) +
			(((y + updateIndex_.y) % MAX_GALAXIES_IN_A_ROW_) +
			((z + updateIndex_.z) % MAX_GALAXIES_IN_A_ROW_) *
				MAX_GALAXIES_IN_A_ROW_) * MAX_GALAXIES_IN_A_ROW_;

		children_.at(index) = createGalaxy_(offset + Vector3(x, y, z));
	}

	void Universe::addGalaxies_()
	{
		for (int z = 0; z < MAX_GALAXIES_IN_A_ROW_; z++) {
			for (int y = 0; y < MAX_GALAXIES_IN_A_ROW_; y++) {
				for (int x = 0; x < MAX_GALAXIES_IN_A_ROW_; x++) {
					children_.push_back(createGalaxy_(Vector3(x, y, z)));
				}
			}
		}
	}

	std::shared_ptr<Galaxy> Universe::createGalaxy_(const Vector3& offset)
	{
		Random::setRandSeed(Random::u32FromByteArray(&(updateCameraPosition_ + offset), 24));

		float r = Random::randFloat();
		float maxRadius = Galaxy::MAX_RADIUS * Galaxy::SCALE / SCALE;
		float galaxyRadius = maxRadius * (0.25f + 0.75f * r * r);

		auto galaxy = std::make_shared<Galaxy>(this, galaxyRadius);

		galaxy->transform.setPosition((updateCameraPosition_ + offset) * PERIOD_ + Vector3(
			(Coordinate)((1 - MAX_GALAXIES_IN_A_ROW_ * 0.5f + Random::randFloat(-0.75, 0.75)) * PERIOD_),
			(Coordinate)((1 - MAX_GALAXIES_IN_A_ROW_ * 0.5f + Random::randFloat(-0.75, 0.75)) * PERIOD_),
			(Coordinate)((1 - MAX_GALAXIES_IN_A_ROW_ * 0.5f + Random::randFloat(-0.75, 0.75)) * PERIOD_)
		));

		r = Random::randFloat();
		galaxy->transform.rotate(360 * r, glm::sphericalRand(1.0f));

		galaxy->create();

		return galaxy;
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

		// draw the universe
		std::vector<DrawConfiguration> toDrawList;
		auto hierarchy = camera.getHierarchy();
		drawRecursively_(toDrawList, hierarchy, hierarchy.size() - 1);
		draw_(toDrawList);
	}
}
