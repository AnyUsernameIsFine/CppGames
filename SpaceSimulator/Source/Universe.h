#pragma once

#include "Galaxy.h"

namespace Game
{
	class Universe : public CoordinateSystem
	{
	public:
		Universe();
		void draw(const Camera& camera);
		glm::vec4 getColor() const { return { 0, 1, 0, 1 }; }

	private:
		void addGalaxies_();
	};
}
