#include "IndexBufferObject.hpp"
#include "System\Error.hpp"

#include <GL\glew.h>

namespace Framework
{
	IndexBufferObject::IndexBufferObject(int numberOfIndices, const GLushort* indices)
	{
		glCheck(glGenBuffers(1, &id));
		glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id));
		glCheck(glBufferData(GL_ELEMENT_ARRAY_BUFFER, numberOfIndices * sizeof(GLushort), indices, indices == nullptr ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW));
	}
}
