#pragma once

#include "VertexBufferObject.h"
#include "IndexBufferObject.h"

namespace Framework
{
	class VertexArrayObject : public OpenGLObject
	{
	public:
		VertexArrayObject() {}
		~VertexArrayObject();
		void create();
		void use() const;
	};
}
