#pragma once

#include "Moon.h"

namespace Game
{
	class Planet : public CoordinateSystem
	{
	public:
		static const float SCALE;
		static const float MAX_RADIUS;
		static const glm::vec4 COLOR;

		Planet(CoordinateSystem* parent, float radius);
		float getScale() const;
		const glm::vec4& getColor() const;
		void create();

	private:
		void addMoons_();
	};
}
