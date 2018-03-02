#pragma once

#include "OpenGLObject.h"

namespace Framework
{
	class IndexBufferObject : public OpenGLObject
	{
	public:
		IndexBufferObject() {}
		IndexBufferObject(int numberOfIndices, const GLushort* indices = nullptr);
		~IndexBufferObject();
		void create(int numberOfIndices, const GLushort* indices = nullptr);
	};
}
