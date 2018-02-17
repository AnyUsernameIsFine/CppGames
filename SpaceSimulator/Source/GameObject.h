#pragma once

#include "Transform.h"

namespace Game
{
	class GameObject
	{
	public:
		GameObject * parent = nullptr;
		Transform transform;
	};
}
