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
		void update(const Camera& camera);
		void draw(const Camera& camera);

	private:
		void addGalaxies_();
	};
}
