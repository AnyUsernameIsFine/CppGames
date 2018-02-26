#pragma once

#include "OpenGLObject.hpp"

namespace Framework
{
	class IndexBufferObject : public OpenGLObject
	{
	public:
		IndexBufferObject(int numberOfIndices, const GLushort* indices = nullptr);
	};
}
