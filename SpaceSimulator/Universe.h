#pragma once

#include "CoordinateSystem.h"

namespace Game
{
	using namespace Framework;

	class Universe : public CoordinateSystem
	{
	public:
		void create(const std::string& name, float scale);

	private:
		void addSubSystems_(CoordinateSystem* parent, const std::vector<std::string>& names, float subSystemScale);
		void linkSubSystems_(CoordinateSystem* parent);
	};
}
