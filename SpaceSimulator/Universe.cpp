#include "Universe.h"

#include <Framework.hpp>

namespace Game
{
	void Universe::create(const std::string& name, float scale)
	{
		this->name = name;
		this->scale = scale;

		std::vector<std::string> names = {
			"Galaxy",
			"Star",
			"Planet",
			"Moon",
		};

		addSubSystems_(this, names, scale);
		linkSubSystems_(this);
	}

	void Universe::addSubSystems_(CoordinateSystem* parent, const std::vector<std::string>& names, float parentScale)
	{
		if (!names.size()) {
			return;
		}

		float subSystemScale = parentScale;

		for (int z = -1; z <= 1; z += 2) {
			float ratio = 3.0f + names.size() / (z + 2.0f);
			//float ratio = 10;
			subSystemScale /= ratio;

			for (int y = 0; y <= 0; y++) {
				for (int x = 0; x <= 0; x++) {
					CoordinateSystem subSystem;

					subSystem.name = names[0];
					subSystem.scale = subSystemScale;
					subSystem.radius = 1;

					float positionOffset = ratio * 2.0f / 3.0f;
					subSystem.transform.setPosition({ x * positionOffset, y * positionOffset, z * positionOffset });
					subSystem.transform.rotate(20.0f * names.size() * z, { 1, 1, 1 });

					addSubSystems_(&subSystem, std::vector<std::string>(names.begin() + 1, names.end()), subSystemScale);

					parent->coordinateSystems.push_back(subSystem);
				}
			}
		}
	}

	void Universe::linkSubSystems_(CoordinateSystem* parent)
	{
		for (auto &subSystem : parent->coordinateSystems) {
			linkSubSystems_(&subSystem);
			subSystem.parent = parent;
		}
	}
}
