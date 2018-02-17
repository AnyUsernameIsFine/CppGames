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
		glm::vec4 getColor() const;
		const std::vector<std::unique_ptr<CoordinateSystem>>& getChildren() const;
		void draw(const Camera& camera);

	private:
		std::vector<std::unique_ptr<CoordinateSystem>> galaxies_;

		void addGalaxies_();
	};
}
