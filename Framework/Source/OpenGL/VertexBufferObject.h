#pragma once

#include "OpenGLObject.h"

namespace Framework
{
	class VertexBufferObject : public OpenGLObject
	{
	public:
		VertexBufferObject() {}
		VertexBufferObject(const vector<int>& attributes, int numberOfVertices, const void* vertices = nullptr);
		~VertexBufferObject();
		void create(const vector<int>& attributes, int numberOfVertices, const void* vertices = nullptr);
	};
}
