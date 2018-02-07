#include "VertexBufferObject.hpp"
#include "Error.hpp"

#include <GL\glew.h>

namespace Framework
{
	VertexBufferObject::VertexBufferObject(const std::vector<int>& attributes, int numberOfVertices, const void* vertices)
	{
		glCheck(glGenBuffers(1, &id_));
		glCheck(glBindBuffer(GL_ARRAY_BUFFER, id_));

		GLsizei size = 0;
		for (auto const& attribute : attributes) {
			size += attribute * sizeof(GLfloat);
		}

		glCheck(glBufferData(GL_ARRAY_BUFFER, size * numberOfVertices, vertices, vertices == nullptr ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW));

		int offset = 0;
		for (int i = 0; i < (int)attributes.size(); i++) {
			glCheck(glVertexAttribPointer(i, attributes[i], GL_FLOAT, GL_FALSE, size, (GLvoid*)offset));
			glCheck(glEnableVertexAttribArray(i));
			offset += attributes[i] * sizeof(GLfloat);
		}
	}
}
