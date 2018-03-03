#pragma once

#include "Transform.h"

namespace Framework
{
	template<typename T>
	class GameObjectType
	{
	public:
		TransformType<T>& transform();

	protected:
		TransformType<T> myTransform;
	};

	typedef GameObjectType<float> GameObject;
}

namespace Framework
{
	template<typename T>
	TransformType<T>& GameObjectType<T>::transform()
	{
		return myTransform;
	}
}
