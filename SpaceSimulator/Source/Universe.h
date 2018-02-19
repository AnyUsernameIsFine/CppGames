#pragma once

#include "Galaxy.h"

namespace Game
{
	class Universe : public CoordinateSystem
	{
	public:
		static const float SCALE;
		static const glm::vec4 COLOR;

		Universe();
		float getScale() const;
		const glm::vec4& getColor() const;
		void create(const Camera& camera);
		void update(const Camera& camera);
		void draw(const Camera& camera);

	private:
		Vector3 updateCameraPosition_;
		glm::ivec3 updateIndex_;

		void addGalaxiesX_(bool positive);
		void addGalaxiesY_(bool positive);
		void addGalaxiesZ_(bool positive);
		void addGalaxy_(const Vector3& offset, int x, int y, int z);
		void addGalaxies_();
		std::shared_ptr<Galaxy> createGalaxy_(const Vector3& offset);

		static const int MAX_GALAXIES_IN_A_ROW_;
		static const float PERIOD_;
	};
}
