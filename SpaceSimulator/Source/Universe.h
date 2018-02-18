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
		void addGalaxies_(const Vector3& steppedCameraPosition);
	};
}
