#pragma once

#include "System\Globals.hpp"
#include "OpenGLObject.hpp"

namespace Framework
{
	class VertexBufferObject : public OpenGLObject
	{
	public:
		VertexBufferObject(const vector<int>& attributes, int numberOfVertices, const void* vertices = nullptr);
	};
}
