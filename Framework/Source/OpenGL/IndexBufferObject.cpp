#include "IndexBufferObject.hpp"
#include "System\Error.hpp"

#include <GL\glew.h>

namespace Framework
{
	IndexBufferObject::IndexBufferObject(int numberOfIndices, const unsigned int* indices)
	{
		glCheck(glGenBuffers(1, &id_));
		glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_));
		glCheck(glBufferData(GL_ELEMENT_ARRAY_BUFFER, numberOfIndices * sizeof(unsigned int), indices, indices == nullptr ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW));
	}
}
