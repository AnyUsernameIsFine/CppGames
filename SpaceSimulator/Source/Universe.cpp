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
		this->radius = 1;
		this->name = "Universe";
		this->scale = SCALE;

		srand((int)time(nullptr));

		addGalaxies_();
	}

	glm::vec4 Universe::getColor() const
	{
		return COLOR;
	}

	const std::vector<std::unique_ptr<CoordinateSystem>>& Universe::getChildren() const
	{
		return galaxies_;
	}

	void Universe::draw(const Camera& camera)
	{
		// initialize some variables
		CameraHierarchyLevel ch{
			camera.coordinateSystem,
			glm::mat4(1),
			camera.transform.getPosition(),
		};

		std::vector<CameraHierarchyLevel> hierarchy = { ch };

		CoordinateSystem* parentCs = (CoordinateSystem*)ch.coordinateSystem->parent;

		// create and set the projection-view matrix
		float r = 1.0f;
		if (parentCs) {
			r = parentCs->scale / ch.coordinateSystem->scale;
		}
		glm::mat4 projectionViewMatrix = glm::scale(camera.getProjectionMatrix() * camera.getViewMatrix(true), { r, r, r });
		shaderProgram_->use();
		shaderProgram_->setUniform("projectionViewMatrix", projectionViewMatrix);

		// determine camera coordinate system hierarchy
		while (parentCs) {
			glm::quat q = ch.coordinateSystem->transform.getOrientation();
			ch.rotation *= glm::mat4_cast(q);

			// TODO: add more precision?
			ch.position *= q;
			ch.position *= ch.coordinateSystem->scale / parentCs->scale;
			ch.position += ch.coordinateSystem->transform.getPosition();

			ch.coordinateSystem = parentCs;

			hierarchy.push_back(ch);

			parentCs = (CoordinateSystem*)parentCs->parent;
		}

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

						galaxies_.push_back(std::make_unique<Galaxy>(this, galaxyRadius));

						Galaxy* galaxy = (Galaxy*)galaxies_.back().get();

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
