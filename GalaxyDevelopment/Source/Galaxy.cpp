#include "Galaxy.h"

#include <glm\gtc\random.hpp>
#include "specrend.h"
#include "OrbitCalculator.h"

namespace GalaxyDevelopment
{
	Galaxy::Galaxy() :
		numColors(200),
		minTemperature(1000),
		maxTemperature(10000)
	{
		for (int i = 0; i < numColors; ++i) {
			glm::dvec3 color;
			colourSystem *cs = &SMPTEsystem;
			bbTemp = minTemperature + (maxTemperature - minTemperature) / numColors * i;
			double x, y, z;
			spectrum_to_xyz(bb_spectrum, &x, &y, &z);
			xyz_to_rgb(cs, x, y, z, &color.r, &color.g, &color.b);
			norm_rgb(&color.r, &color.g, &color.b);
			colors.emplace_back(color);
		}
	}

	void Galaxy::create(
		float galaxyRadius,
		float coreRadius,
		float angularOffset,
		float coreEccentricity,
		float galaxyEccentricity,
		int numStars,
		int numDustClouds,
		int numH2Regions
	)
	{
		this->galaxyRadius = galaxyRadius;
		this->angularOffset = angularOffset;
		this->coreRadius = coreRadius;
		this->farFieldRadius = galaxyRadius * 2;
		this->coreEccentricity = coreEccentricity;
		this->galaxyEccentricity = galaxyEccentricity;
		this->pertN = 2;
		this->pertAmp = 40;

		createStars(numStars);
		createDustClouds(numDustClouds);
		createH2Regions(numH2Regions);
	}

	void Galaxy::singleTimeStep(float time)
	{
		for (auto& star : stars) {
			star.calculatePosition(pertN, pertAmp);
			if (star.semiMinorAxis < galaxyRadius) {
				star.position += glm::ballRand(abs(Random::randFloatNormal((galaxyRadius - star.semiMinorAxis) / 5)));
			}
		}

		for (auto& dustCloud : dustClouds) {
			dustCloud.calculatePosition(pertN, pertAmp);
			if (dustCloud.semiMinorAxis < galaxyRadius) {
				dustCloud.position += glm::ballRand(abs(Random::randFloatNormal((galaxyRadius - dustCloud.semiMinorAxis) / 5)));
			}
		}

		for (auto& h2Region : h2Regions) {
			h2Region.calculatePosition(pertN, pertAmp);
			if (h2Region.semiMinorAxis < galaxyRadius) {
				h2Region.position += glm::ballRand(abs(Random::randFloatNormal((galaxyRadius - h2Region.semiMinorAxis) / 5)));
			}
		}
	}

	std::vector<Galaxy::Particle> Galaxy::getStars() const
	{
		std::vector<Particle> particles;

		for (const auto& star : stars) {
			Particle particle;
			particle.position = star.position;
			particle.size = star.size;
			particle.color = colorFromTemperature(star.temperature) * star.brightness;
			particle.color.r += 0.2;
			particle.color.g += 0.2;
			particle.color.b += 0.2;

			particles.emplace_back(particle);
		}

		return particles;
	}

	std::vector<Galaxy::Particle> Galaxy::getDustClouds() const
	{
		std::vector<Particle> particles;

		for (const auto& dustCloud : dustClouds) {
			Particle particle;
			particle.position = dustCloud.position;
			particle.size = dustCloud.size;
			particle.color = colorFromTemperature(dustCloud.temperature) * dustCloud.brightness;

			particles.emplace_back(particle);
		}

		return particles;
	}

	std::vector<Galaxy::Particle> Galaxy::getH2Regions() const
	{
		std::vector<Particle> particles;

		for (int i = 0; i < h2Regions.size(); i += 2) {
			const Star& h2Region = h2Regions[i];
			const Star& h2Region2 = h2Regions[i + 1];

			float distance = glm::distance(h2Region.position, h2Region2.position);
			float size = ((1000 - distance) / 10) - 50;
			if (size < 1) {
				continue;
			}

			Particle particle;
			particle.position = h2Region.position;
			particle.size = size * 25;
			particle.color = colorFromTemperature(h2Region.temperature) * h2Region.brightness;
			particle.color.r *= 2;
			particle.color.g *= 0.5;
			particle.color.b *= 0.5;

			particles.emplace_back(particle);

			Particle particle2;
			particle2.position = h2Region.position;
			particle2.size = size * 2;
			particle2.color = { 1, 0.5f, 0.5f };

			particles.emplace_back(particle2);
		}

		return particles;
	}

	glm::vec3 Galaxy::colorFromTemperature(float temperature) const
	{
		int index = (temperature - minTemperature) / (maxTemperature - minTemperature) * numColors;
		index = std::min(numColors - 1, index);
		index = std::max(0, index);

		return colors[index];
	}

	void Galaxy::createStars(int numStars)
	{
		Star centerBlackHole;
		centerBlackHole.temperature = 6000;
		stars.emplace_back(centerBlackHole);

		for (int i = 1; i < numStars; i++) {
			float radius = abs(Random::randFloatNormal(galaxyRadius / 3));
			//radius = Random::randFloat(galaxyRadius);

			Star star;
			star.semiMinorAxis = radius;
			star.semiMajorAxis = radius * getEccentricity(radius);
			star.ellipseAngularOffset = getAngularOffset(radius);
			star.angleOnEllipse = Random::randFloat(360);
			star.temperature = Random::randFloat(4000, 8000);
			star.brightness = Random::randFloat(0.3, 0.5);
			star.size = Random::randFloat(0.0001, 0.0004) * galaxyRadius;

			stars.emplace_back(star);
		}
	}

	void Galaxy::createDustClouds(int numDustClouds)
	{
		for (int i = 0; i < numDustClouds; i++) {
			float radius;
			if (i % 4 == 0) {
				radius = abs(Random::randFloatNormal(galaxyRadius / 3));
			}
			else {
				float x = Random::randFloat(-galaxyRadius, galaxyRadius);
				float y = Random::randFloat(-galaxyRadius, galaxyRadius);
				radius = sqrt(x * x + y * y);
			}
			//radius = Random::randFloat(galaxyRadius);

			Star dustCloud;
			dustCloud.semiMinorAxis = radius;
			dustCloud.semiMajorAxis = radius * getEccentricity(radius);
			dustCloud.ellipseAngularOffset = getAngularOffset(radius);
			dustCloud.angleOnEllipse = Random::randFloat(360);
			dustCloud.temperature = 5000 + radius / 4.5;
			dustCloud.brightness = Random::randFloat(0.01, 0.015);
			dustCloud.size = Random::randFloat(0.08, 0.12) * galaxyRadius;

			dustClouds.emplace_back(dustCloud);
		}
	}

	void Galaxy::createH2Regions(int numH2Regions)
	{
		for (int i = 0; i < numH2Regions; i++) {
			float x = Random::randFloat(-galaxyRadius, galaxyRadius);
			float y = Random::randFloat(-galaxyRadius, galaxyRadius);
			float radius = sqrt(x*x + y * y);

			Star h2Region;
			h2Region.semiMinorAxis = radius;
			h2Region.semiMajorAxis = radius * getEccentricity(radius);
			h2Region.ellipseAngularOffset = getAngularOffset(radius);
			h2Region.angleOnEllipse = Random::randFloat(360);
			h2Region.temperature = Random::randFloat(3000, 9000);
			h2Region.brightness = Random::randFloat(0.1, 0.15);
			h2Region.size = Random::randFloat(0.05, 0.1) * galaxyRadius;

			h2Regions.emplace_back(h2Region);

			Star h2Region2;
			h2Region2.semiMinorAxis = radius + 1000;
			h2Region2.semiMajorAxis = h2Region.semiMajorAxis;
			h2Region2.ellipseAngularOffset = h2Region.ellipseAngularOffset;
			h2Region2.angleOnEllipse = h2Region.angleOnEllipse;
			h2Region2.temperature = h2Region.temperature;
			h2Region2.brightness = h2Region.brightness;
			h2Region2.size = h2Region.size;

			h2Regions.emplace_back(h2Region2);
		}
	}

	float Galaxy::getEccentricity(float radius) const
	{
		if (radius < coreRadius) {
			return 1 + (radius / coreRadius) * (coreEccentricity - 1);
		}
		else if (radius > coreRadius && radius <= galaxyRadius) {
			return coreEccentricity + (radius - coreRadius) / (galaxyRadius - coreRadius) * (galaxyEccentricity - coreEccentricity);
		}
		else if (radius > galaxyRadius && radius < farFieldRadius) {
			return galaxyEccentricity + (radius - galaxyRadius) / (farFieldRadius - galaxyRadius) * (1 - galaxyEccentricity);
		}
		else {
			return 1;
		}
	}

	float Galaxy::getAngularOffset(float radius) const
	{
		return radius * angularOffset;
	}

	const glm::vec3& Galaxy::Star::calculatePosition(int pertN, float pertAmp)
	{
		auto position2d = OrbitCalculator::compute(
			ellipseAngularOffset,
			semiMinorAxis,
			semiMajorAxis,
			angleOnEllipse,
			glm::vec2(0),
			pertN,
			pertAmp
		);

		position = { position2d.x, 0, position2d.y };

		return position;
	}
}
