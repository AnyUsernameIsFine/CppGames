#include "IndexBufferObject.hpp"
#include "Error.hpp"

#include <GL\glew.h>

namespace Framework
{
	IndexBufferObject::IndexBufferObject(int numberOfIndices, const int* indices)
	{
		glCheck(glGenBuffers(1, &id_));
		glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_));
		glCheck(glBufferData(GL_ELEMENT_ARRAY_BUFFER, numberOfIndices * sizeof(int), indices, GL_STATIC_DRAW));
	}
}
