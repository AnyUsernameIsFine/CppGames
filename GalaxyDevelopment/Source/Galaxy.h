#pragma once

#include <vector>

#include <GLEngine.h>

namespace GalaxyDevelopment
{
	using namespace GLEngine;

	class Galaxy : public GameObject
	{
	public:
		struct Particle
		{
			glm::vec3 position;
			float size;
			glm::vec3 color;
		};

		Galaxy();
		void create(
			float galaxyRadius = 13000,
			float coreRadius = 4000, 
			float angularOffset = 0.0004,
			float coreEccentricity = 0.55,
			float galaxyEccentricity = 0.95,
			int numStars = 1 << 18,
			int numDustClouds = 1 << 14,
			int numH2Regions = 1 << 8
		);
		void singleTimeStep(float time);
		std::vector<Particle> getStars() const;
		std::vector<Particle> getDustClouds() const;
		std::vector<Particle> getH2Regions() const;

	private:
		struct Star
		{
			float angleOnEllipse = 0;
			float ellipseAngularOffset = 0;
			float semiMinorAxis = 0;
			float semiMajorAxis = 0;
			float temperature = 0;
			float brightness = 0;
			float size = 0;
			glm::vec3 position = { 0 };

			const glm::vec3& calculatePosition(int pertN, float pertAmp);
		};

		glm::vec3 colorFromTemperature(float temperature) const;
		void createStars(int numStars);
		void createDustClouds(int numDustClouds);
		void createH2Regions(int numH2Regions);
		float getEccentricity(float radius) const;
		float getAngularOffset(float radius) const;

		float galaxyRadius;
		float coreRadius;
		float farFieldRadius;
		float angularOffset;
		float coreEccentricity;
		float galaxyEccentricity;
		int pertN;
		float pertAmp;

		std::vector<Star> stars;
		std::vector<Star> dustClouds;
		std::vector<Star> h2Regions;

		int numColors;
		float minTemperature;
		float maxTemperature;
		std::vector<glm::vec3> colors;
	};
}
