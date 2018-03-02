#include "VertexBufferObject.h"

namespace Framework
{
	VertexBufferObject::VertexBufferObject(const vector<int>& attributes, int numberOfVertices, const void* vertices)
	{
		create(attributes, numberOfVertices, vertices);
	}

	VertexBufferObject::~VertexBufferObject()
	{
		if (sdlCheckValue(SDL_GL_GetCurrentContext())) {
			glCheck(glDeleteBuffers(1, &id));
		}
	}

	void VertexBufferObject::create(const vector<int>& attributes, int numberOfVertices, const void* vertices)
	{
		if (!hasContext("Could not create vertex buffer object")) {
			return;
		}


		if (id) {
			error("Vertex buffer object has already been created");
			return;
		}

		glCheck(glGenBuffers(1, &id));
		glCheck(glBindBuffer(GL_ARRAY_BUFFER, id));

		GLsizei size = 0;
		for (auto const& attribute : attributes) {
			size += attribute * sizeof(GLfloat);
		}

		glCheck(glBufferData(GL_ARRAY_BUFFER, numberOfVertices * size, vertices, vertices ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW));

		int offset = 0;
		for (int i = 0; i < (int)attributes.size(); i++) {
			glCheck(glVertexAttribPointer(i, attributes[i], GL_FLOAT, GL_FALSE, size, (GLvoid*)offset));
			glCheck(glEnableVertexAttribArray(i));
			offset += attributes[i] * sizeof(GLfloat);
		}
	}
}
