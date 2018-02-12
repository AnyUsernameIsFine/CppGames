#include "Universe.h"

#include <Framework.hpp>

namespace Game
{
	Universe::Universe(float scale)
	{
		this->name = "Universe";
		this->scale = scale;

		std::vector<std::string> names = {
			"Galaxy",
			"Star",
			"Planet",
			"Moon",
			"Asteroid",
		};

		addDecendants_(this, names, scale);
		linkDecendants_(this);
	}

	void Universe::addDecendants_(CoordinateSystem* parent, const std::vector<std::string>& names, float parentScale)
	{
		if (!names.size()) {
			return;
		}

		float csScale = parentScale;

		for (int z = 0; z <= 0; z += 2) {
			for (int y = 0; y <= 0; y += 2) {
				for (int x = -1; x <= 1; x += 2) {

					float ratio = 3.0f + names.size() / (x + 2.0f);
					//float ratio = 10;
					csScale /= ratio;

					CoordinateSystem cs;

					cs.name = names[0];
					cs.scale = csScale;
					cs.radius = 1;

					float positionOffset = ratio * 2.0f / 3.0f;
					cs.transform.setPosition({ x * positionOffset, y * positionOffset, z * positionOffset });

					// test long distances
					if (parentScale == 100000000) {
						//cs.transform.moveX(100000000.0f);
					}

					cs.transform.rotate(20.0f * names.size() * x, { 1, 1, 1 });

					addDecendants_(&cs, std::vector<std::string>(names.begin() + 1, names.end()), csScale);

					parent->descendants.push_back(cs);
				}
			}
		}
	}

	void Universe::linkDecendants_(CoordinateSystem* parent)
	{
		for (auto &cs : parent->descendants) {
			linkDecendants_(&cs);
			cs.parent = parent;
		}
	}
}
