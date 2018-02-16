#pragma once

#include "Moon.h"

namespace Game
{
	class Planet : public CoordinateSystem
	{
	public:
		Planet(CoordinateSystem* parent, float radius);
		glm::vec4 getColor() const { return { 1, 1, 0, 1 }; }

	private:
		static int counter_;

		void addMoons_();
	};
}
