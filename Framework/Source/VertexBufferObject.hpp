#pragma once

#include "OpenGLObject.hpp"

#include <vector>

namespace Framework
{
	class VertexBufferObject : public OpenGLObject
	{
	public:
		VertexBufferObject(const std::vector<int>& attributes, int numberOfVertices, const void* vertices = nullptr);
	};
}
