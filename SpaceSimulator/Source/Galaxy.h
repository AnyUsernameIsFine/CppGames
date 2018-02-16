#pragma once

#include "Star.h"

namespace Game
{
	class Galaxy : public CoordinateSystem
	{
	public:
		Galaxy(CoordinateSystem* parent, float radius);
		glm::vec4 getColor() const { return { 0, 0, 1, 1 }; }

	private:
		static int counter_;

		void addStars_();
	};
}
