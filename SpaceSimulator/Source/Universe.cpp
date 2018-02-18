#include "Universe.h"

#include <Framework.hpp>

#include <ctime>

namespace Game
{
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
	}

	float Universe::getScale() const
	{
		return SCALE;
	}

	const glm::vec4& Universe::getColor() const
	{
		return COLOR;
	}

	void Universe::update(const Camera& camera)
	{
		addGalaxies_();
	}

	void Universe::draw(const Camera& camera)
	{
		CoordinateSystem* cs = camera.coordinateSystem;

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

	void Universe::addGalaxies_()
	{
		float maxRadius = Galaxy::MAXIMUM_RADIUS * Galaxy::SCALE / SCALE;

		int foo = 2;

		for (int z = -foo; z <= foo; z++) {
			for (int y = -foo; y <= foo; y++) {
				for (int x = -foo; x <= foo; x++) {
					if ((float)rand() / RAND_MAX < 0.5f) {
						float r = (float)rand() / RAND_MAX;
						float galaxyRadius = maxRadius * (0.25f + 0.75f * r * r);

						children_.push_back(std::make_unique<Galaxy>(this, galaxyRadius));

						Galaxy* galaxy = (Galaxy*)children_.back().get();

						galaxy->transform.setPosition(
							(Coordinate)((x + 0.75 * ((float)rand() / RAND_MAX - 0.5)) * maxRadius * 100),
							(Coordinate)((y + 0.75 * ((float)rand() / RAND_MAX - 0.5)) * maxRadius * 100),
							(Coordinate)((z + 0.75 * ((float)rand() / RAND_MAX - 0.5)) * maxRadius * 100)
						);

						r = (float)rand() / RAND_MAX;
						galaxy->transform.rotate(360 * r, glm::sphericalRand(1.0f));
					}
				}
			}
		}
	}
}
