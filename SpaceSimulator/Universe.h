#pragma once

#include "CoordinateSystem.h"

namespace Game
{
	using namespace Framework;

	class Universe : public CoordinateSystem
	{
	public:
		void create(const std::wstring& name, float scale);

	private:
		void addSubSystems_(CoordinateSystem* parent, const std::vector<std::wstring>& names, float scale);
		void linkSubSystems_(CoordinateSystem* parent);
	};
}
