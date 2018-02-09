#include "Universe.h"

#include <Framework.hpp>

namespace Game
{
	void Universe::create(const std::wstring& name, float scale)
	{
		this->name = name;
		this->scale = scale;

		std::vector<std::wstring> names = {
			L"Galaxy",
			L"Star",
			L"Planet",
			//L"Moon",
		};

		addSubSystems_(this, names, scale);
		linkSubSystems_(this);

		output(&(coordinateSystems[0]));
	}

	void Universe::addSubSystems_(CoordinateSystem* parent, const std::vector<std::wstring>& names, float scalettt)
	{
		if (!names.size()) {
			return;
		}

		for (int z = -1; z <= 1; z += 2) {

			float ratio = 3 + 2 * names.size() / (z + 2.0) / 2;
			scalettt /= ratio;
			output(scalettt);

			for (int y = 0; y <= 0; y++) {
				for (int x = 0; x <= 0; x++) {
					CoordinateSystem subSystem;
					subSystem.name = names[0];
					subSystem.scale = scalettt;
					subSystem.radius = 1;
					subSystem.transform.setPosition({ x * ratio * 2 / 3, y * ratio * 2 / 3, z * ratio * 2 / 3 });
					subSystem.transform.rotate(20 * names.size() * z, { 1, 1, 1 });

					addSubSystems_(&subSystem, std::vector<std::wstring>(names.begin() + 1, names.end()), scalettt);

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
