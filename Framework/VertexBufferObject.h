#pragma once

#include "GLObject.h"

#include <vector>

namespace Framework
{
	class VertexBufferObject : public GLObject
	{
	public:
		VertexBufferObject(const std::vector<unsigned int>& attributes, unsigned int numberOfVertices, const void* vertices = nullptr);
	};
}
