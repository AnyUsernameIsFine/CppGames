#pragma once

#include "Galaxy.h"

namespace Game
{
	class Universe : public CoordinateSystem
	{
	public:
		Universe();
		void draw(const Camera& camera) const;

	private:
		void addGalaxies_();
	};
}
