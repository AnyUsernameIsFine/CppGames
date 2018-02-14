#pragma once

#include "Galaxy.h"

namespace Game
{
	class Universe : public CoordinateSystem
	{
	public:
		Universe();

	private:
		void addGalaxies_();
	};
}
