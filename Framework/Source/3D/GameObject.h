#pragma once

#include <memory>

#include "Mesh.h"
#include "Transform.h"

namespace Framework
{
	template<typename T>
	class GameObjectType;

	using GameObject = GameObjectType<float>;

	template<typename T>
	class GameObjectType
	{
	public:
		template<typename U> void setMesh();
		TransformType<T>& transform();
		const Mesh& mesh() const;
		bool hasMesh() const;

	protected:
		TransformType<T> myTransform;
		std::shared_ptr<Mesh> myMesh = nullptr;
	};

	template<typename T>
	template<typename U>
	void GameObjectType<T>::setMesh()
	{
		myMesh = std::make_shared<U>();
	}

	template<typename T>
	TransformType<T>& GameObjectType<T>::transform()
	{
		return myTransform;
	}

	template<typename T>
	const Mesh& GameObjectType<T>::mesh() const
	{
		return *myMesh.get();
	}

	template<typename T>
	bool GameObjectType<T>::hasMesh() const
	{
		return myMesh != nullptr;
	}
}
