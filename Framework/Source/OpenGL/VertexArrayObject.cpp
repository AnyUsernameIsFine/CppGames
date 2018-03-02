#include "VertexArrayObject.h"

namespace Framework
{
	VertexArrayObject::~VertexArrayObject()
	{
		if (sdlCheckValue(SDL_GL_GetCurrentContext())) {
			glCheck(glDeleteVertexArrays(1, &id));
		}
	}

	void VertexArrayObject::create()
	{
		if (!hasContext("Could not create vertex array object")) {
			return;
		}

		if (id) {
			error("Vertex array object has already been created");
			return;
		}

		glCheck(glGenVertexArrays(1, &id));
		glCheck(glBindVertexArray(id));
	}

	void VertexArrayObject::use() const
	{
		if (!id) {
			error("Vertex array object has not been created");
			return;
		}

		glBindVertexArray(id);
	}
}
