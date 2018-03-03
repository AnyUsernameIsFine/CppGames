#pragma once

#include "Transform.h"
#include "Mesh.h"

#include <memory>

namespace Framework
{
	template<typename T>
	class GameObjectType
	{
	public:
		TransformType<T>& transform();
		const Mesh& mesh() const;
		bool hasMesh() const;

		template<typename U>
		void setMesh();

	protected:
		TransformType<T> myTransform;
		std::shared_ptr<Mesh> myMesh = nullptr;
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

	template<typename T>
	template<typename U>
	void GameObjectType<T>::setMesh()
	{
		myMesh = std::make_shared<U>();
	}
}
