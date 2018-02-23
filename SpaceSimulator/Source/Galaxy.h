#pragma once

#include "Star.h"

namespace Game
{
	class Galaxy : public CoordinateSystem
	{
	public:
		static const float SCALE;
		static const float MAX_RADIUS;
		static const glm::vec4 COLOR;

		Galaxy(CoordinateSystem* parent, float radius);
		float getScale() const;
		const glm::vec4& getColor() const;
		float getCameraNearPlane() const;
		void create();

	private:
		void addStars_();
	};
}
