#pragma once

#include "Galaxy.h"

namespace SpaceSimulator
{
	class Universe : public CoordinateSystem
	{
	public:
		static const float SCALE;
		static const glm::vec4 COLOR;

		Universe();
		float getScale() const;
		const glm::vec4& getColor() const;
		float getCameraNearPlane() const;
		void create();
		void update(const Camera& camera);
		void draw(const Camera& camera);

	private:
		static const int GALAXIES_PER_SIDE;
		static const float PERIOD;

		void addGalaxies(const Vector3& offset = Vector3(0));
		void addGalaxy(int x, int y, int z);

		Vector3 updateCameraPosition;
		glm::uvec3 updateIndex;
	};
}
