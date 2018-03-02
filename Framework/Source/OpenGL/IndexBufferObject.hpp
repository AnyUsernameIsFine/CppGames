#pragma once

#include "OpenGLObject.hpp"

namespace Framework
{
	class IndexBufferObject : public OpenGLObject
	{
	public:
		IndexBufferObject() {}
		IndexBufferObject(int numberOfIndices, const GLushort* indices = nullptr);
		void create(int numberOfIndices, const GLushort* indices = nullptr);
	};
}
