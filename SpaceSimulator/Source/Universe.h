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
		glm::uvec3 updateIndex_;

		void addGalaxies_(const Vector3& offset = Vector3(0));

		static const int MAX_GALAXIES_IN_A_ROW_;
		static const float PERIOD_;
	};
}
