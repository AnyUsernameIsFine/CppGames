#pragma once

#include "OpenGLObject.hpp"

namespace Framework
{
	class VertexBufferObject : public OpenGLObject
	{
	public:
		VertexBufferObject() {}
		VertexBufferObject(const vector<int>& attributes, int numberOfVertices, const void* vertices = nullptr);
		void create(const vector<int>& attributes, int numberOfVertices, const void* vertices = nullptr);
	};
}
