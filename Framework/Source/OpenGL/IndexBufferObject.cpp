#include "IndexBufferObject.hpp"

namespace Framework
{
	IndexBufferObject::IndexBufferObject(int numberOfIndices, const GLushort* indices)
	{
		create(numberOfIndices, indices);
	}

	IndexBufferObject::~IndexBufferObject()
	{
		if (sdlCheckValue(SDL_WasInit(SDL_INIT_VIDEO))) {
			glCheck(glDeleteBuffers(1, &id));
		}
	}

	void IndexBufferObject::create(int numberOfIndices, const GLushort* indices)
	{
		if (id) {
			error("Index buffer object has already been created");
			return;
		}

		glCheck(glGenBuffers(1, &id));
		glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id));
		glCheck(glBufferData(GL_ELEMENT_ARRAY_BUFFER, numberOfIndices * sizeof(GLushort), indices, indices == nullptr ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW));
	}
}
