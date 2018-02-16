#include "Universe.h"

#include <Framework.hpp>

#include <ctime>

namespace Game
{
	namespace
	{
#ifdef USE_REALISTIC_SCALE
		const float SCALE = (int_least64_t)1 << 54; // About 1.9 light-years per unit.
		// Gives a universe with a radius of over 17.5 million million million light-years
		// when using 64 bit integers. One light-year is 9,460,730,472,580,800 meters.
#else
		const float SCALE = (int_least64_t)1 << 14;
#endif
	}

	Universe::Universe()
	{
		this->radius = 1;
		this->name = "Universe";
		this->scale = SCALE;

		srand((int)time(nullptr));

		addGalaxies_();
	}

	void Universe::addGalaxies_()
	{
#ifdef USE_REALISTIC_SCALE
		// Abell 2029, the largest known galaxy (cluster)
		// has a maximum diameter of about 8 million light-years.
		// Let's make that fit in twice for some wiggle room.
		float maxRadius = 9460730472580800 / SCALE * 8000000;
#else
		float maxRadius = (int_least64_t)1 << 55;
#endif

		for (int i = 0; i < 10; i++) {
			float r = (float)rand() / RAND_MAX;
			float galaxyRadius = maxRadius * (0.25f + 0.75f * r * r);

			children.push_back(std::make_unique<Galaxy>(this, galaxyRadius));

			Galaxy* galaxy = (Galaxy*)children.back().get();

			glm::vec3 v = glm::ballRand(maxRadius * 100);
			galaxy->transform.setPosition({ (Coordinate)v.x, (Coordinate)v.y, (Coordinate)v.z });
			r = (float)rand() / RAND_MAX;
			galaxy->transform.rotate(360 * r, glm::sphericalRand(1.0f));
		}
	}

	void Universe::draw(const Camera& camera) const
	{
		// initialize some variables
		CameraHierarchyLevel ch{
			camera.coordinateSystem,
			glm::mat4(1),
			camera.transform.getPosition(),
		};

		std::vector<CameraHierarchyLevel> hierarchy = { ch };

		CoordinateSystem* parentCs = (CoordinateSystem*)ch.coordinateSystem->parent;

		// determine initial scale ratio (inverse of the camera's coordinate system's scale to its parent's)
		// and number of sub levels to draw for galaxies (0 when inside a galaxy, 1 when outside)
		float r = 1.0f;
		int numberOfSubLevelsToDraw = 1;
		if (parentCs) {
			r = parentCs->scale / ch.coordinateSystem->scale;
			numberOfSubLevelsToDraw = 0;
		}
		glm::vec3 s = { r, r, r };

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

		// save projection and view matrices
		glm::mat4 p = camera.getProjectionMatrix();
		glm::mat4 pr = p * camera.getViewMatrix(true);
		glm::mat4 pv = p * camera.getViewMatrix();

		// draw the universe
		drawRecursively_(
			glm::scale(pr, s),
			glm::scale(pv, s),
			hierarchy,
			hierarchy.size() - 1
		);
	}
}
