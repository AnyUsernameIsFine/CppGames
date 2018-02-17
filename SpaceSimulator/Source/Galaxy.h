#pragma once

#include "Star.h"

namespace Game
{
	class Galaxy : public CoordinateSystem
	{
	public:
		static const float SCALE;
		static const float MAXIMUM_RADIUS;
		static const glm::vec4 COLOR;

		Galaxy(CoordinateSystem* parent, float radius);
		glm::vec4 getColor() const;
		const std::vector<std::unique_ptr<CoordinateSystem>>& getChildren() const;

	private:
		static int counter_;
		std::vector<std::unique_ptr<CoordinateSystem>> stars_;

		void addStars_();
	};
}
