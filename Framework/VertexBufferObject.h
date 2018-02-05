#pragma once

#include "GLObject.h"

#include <vector>

namespace Framework
{
	class VertexBufferObject : public GLObject
	{
	public:
		VertexBufferObject(const std::vector<int>& attributes, int numberOfVertices, const void* vertices = nullptr);
	};
}
