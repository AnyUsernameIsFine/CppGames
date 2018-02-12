#pragma once

#include "CoordinateSystem.h"

namespace Game
{
	using namespace Framework;

	class Universe : public CoordinateSystem
	{
	public:
		Universe(float scale);

	private:
		void addDecendants_(CoordinateSystem* parent, const std::vector<std::string>& names, float subSystemScale);
		void linkDecendants_(CoordinateSystem* parent);
	};
}
