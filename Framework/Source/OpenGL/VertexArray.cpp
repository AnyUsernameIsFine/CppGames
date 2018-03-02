#include "VertexArray.h"

#include <algorithm>

namespace Framework
{
	VertexArray::~VertexArray()
	{
		if (SDL_GL_GetCurrentContext()) {
			int numberOfReferences;

			numberOfReferences = --vertexBuffers[vertexBufferId].numberOfReferences;
			if (!numberOfReferences) {
				glCheck(glDeleteBuffers(1, &vertexBufferId));
			}

			numberOfReferences = --indexBuffers[indexBufferId].numberOfReferences;
			if (!numberOfReferences) {
				glCheck(glDeleteBuffers(1, &indexBufferId));
			}

			numberOfReferences = --instanceBuffers[instanceBufferId].numberOfReferences;
			if (!numberOfReferences) {
				glCheck(glDeleteBuffers(1, &instanceBufferId));
			}

			glCheck(glDeleteVertexArrays(1, &id));
		}
	}

	void VertexArray::setVertexBuffer(const vector<int>& attributes, int numberOfVertices, const void* vertices)
	{
		if (vertexBufferId) {
			error("Vertex buffer object has already been set");
			return;
		}

		create();

		GLsizei attributesSize = 0;
		for (auto& attribute : attributes) {
			attributesSize += attribute * sizeof(GLfloat);
		}

		glCheck(glBindVertexArray(id));
		glCheck(glGenBuffers(1, &vertexBufferId));
		glCheck(glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId));
		glCheck(glBufferData(GL_ARRAY_BUFFER, numberOfVertices * attributesSize, vertices, vertices ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW));
		vertexBuffers[vertexBufferId] = { numberOfVertices, attributes, attributesSize };
		enableVertexAttributes();
		glCheck(glBindVertexArray(0));
	}

	void VertexArray::setIndexBuffer(int numberOfIndices, const GLushort indices[])
	{
		if (indexBufferId) {
			error("Index buffer object has already been set");
			return;
		}

		create();

		glCheck(glBindVertexArray(id));
		glCheck(glGenBuffers(1, &indexBufferId));
		glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId));
		glCheck(glBufferData(GL_ELEMENT_ARRAY_BUFFER, numberOfIndices * sizeof(GLushort), indices, indices ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW));
		indexBuffers[indexBufferId] = { numberOfIndices };
		glCheck(glBindVertexArray(0));
	}

	void VertexArray::setInstanceBuffer(const vector<int>& attributes, int numberOfInstances, const void* instances)
	{
		if (instanceBufferId) {
			error("Instance buffer object has already been set");
			return;
		}

		create();

		GLsizei attributesSize = 0;
		for (auto& attribute : attributes) {
			attributesSize += attribute * sizeof(GLfloat);
		}

		glCheck(glBindVertexArray(id));
		glCheck(glGenBuffers(1, &instanceBufferId));
		glCheck(glBindBuffer(GL_ARRAY_BUFFER, instanceBufferId));
		glCheck(glBufferData(GL_ARRAY_BUFFER, numberOfInstances * attributesSize, instances, instances ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW));
		instanceBuffers[instanceBufferId] = { numberOfInstances, attributes, attributesSize };
		enableInstanceAttributes();
		glCheck(glBindVertexArray(0));
	}

	void VertexArray::setExistingVertexBuffer(GLuint id)
	{
		if (vertexBufferId) {
			error("Vertex buffer object has already been set");
			return;
		}

		create();

		vertexBufferId = id;
		vertexBuffers[id].numberOfReferences++;

		glCheck(glBindVertexArray(this->id));
		glCheck(glBindBuffer(GL_ARRAY_BUFFER, id));
		enableVertexAttributes();
		glCheck(glBindVertexArray(0));
	}

	void VertexArray::setExistingIndexBuffer(GLuint id)
	{
		if (indexBufferId) {
			error("Index buffer object has already been set");
			return;
		}

		create();

		indexBufferId = id;
		indexBuffers[id].numberOfReferences++;

		glCheck(glBindVertexArray(this->id));
		glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId));
		glCheck(glBindVertexArray(0));
	}

	void VertexArray::setExistingInstanceBuffer(GLuint id)
	{
		if (instanceBufferId) {
			error("Instance buffer object has already been set");
			return;
		}

		create();

		instanceBufferId = id;
		instanceBuffers[id].numberOfReferences++;

		glCheck(glBindVertexArray(this->id));
		glCheck(glBindBuffer(GL_ARRAY_BUFFER, id));
		enableInstanceAttributes();
		glCheck(glBindVertexArray(0));
	}

	void VertexArray::updateVertexBuffer(int numberOfVertices, const void* vertices) const
	{
		if (!vertexBufferId) {
			error("Could not update non-existing vertex buffer object");
			return;
		}

		vertexBuffers[vertexBufferId].numberOfVertices = numberOfVertices;
		glCheck(glNamedBufferSubData(vertexBufferId, 0, numberOfVertices * vertexBuffers[vertexBufferId].attributesSize, vertices));
	}

	void VertexArray::updateIndexBuffer(int numberOfIndices, const GLushort indices[]) const
	{
		if (!indexBufferId) {
			error("Could not update non-existing index buffer object");
			return;
		}

		indexBuffers[indexBufferId].numberOfIndices = numberOfIndices;
		glCheck(glNamedBufferSubData(indexBufferId, 0, numberOfIndices * sizeof(GLushort), indices));
	}

	void VertexArray::updateInstanceBuffer(int numberOfInstances, const void* instances) const
	{
		if (!instanceBufferId) {
			error("Could not update non-existing instance buffer object");
			return;
		}

		instanceBuffers[instanceBufferId].numberOfInstances = numberOfInstances;
		glCheck(glNamedBufferSubData(instanceBufferId, 0, numberOfInstances * instanceBuffers[instanceBufferId].attributesSize, instances));
	}

	GLuint VertexArray::getVertexBufferId() const
	{
		return vertexBufferId;
	}

	GLuint VertexArray::getIndexBufferId() const
	{
		return indexBufferId;
	}

	GLuint VertexArray::getInstanceBufferId() const
	{
		return instanceBufferId;
	}

	void VertexArray::draw(GLenum mode) const
	{
		glCheck(glBindVertexArray(id));

		if (instanceBufferId && instanceBuffers[instanceBufferId].numberOfInstances) {
			if (indexBufferId && indexBuffers[indexBufferId].numberOfIndices) {
				glCheck(glDrawElementsInstanced(mode, indexBuffers[indexBufferId].numberOfIndices, GL_UNSIGNED_SHORT, nullptr, instanceBuffers[instanceBufferId].numberOfInstances));
			}
			else if (vertexBufferId && vertexBuffers[vertexBufferId].numberOfVertices) {
				glCheck(glDrawArraysInstanced(mode, 0, vertexBuffers[vertexBufferId].numberOfVertices, instanceBuffers[instanceBufferId].numberOfInstances));
			}
			else {
				error("Insufficient data to draw using vertex array object");
			}
		}
		else if (indexBufferId && indexBuffers[indexBufferId].numberOfIndices) {
			glCheck(glDrawElements(mode, indexBuffers[indexBufferId].numberOfIndices, GL_UNSIGNED_SHORT, nullptr));
		}
		else if (vertexBufferId && vertexBuffers[vertexBufferId].numberOfVertices) {
			glCheck(glDrawArrays(mode, 0, vertexBuffers[vertexBufferId].numberOfVertices));
		}
		else {
			error("Insufficient data to draw using vertex array object");
		}

		glCheck(glBindVertexArray(0));
	}

	void VertexArray::create()
	{
		if (id) {
			return;
		}

		if (!hasContext("Could not create vertex array object")) {
			return;
		}

		glCheck(glGenVertexArrays(1, &id));
	}

	void VertexArray::enableVertexAttributes()
	{
		enableAttributes(vertexBuffers[vertexBufferId].attributes, vertexBuffers[vertexBufferId].attributesSize);
	}

	void VertexArray::enableInstanceAttributes()
	{
		enableAttributes(instanceBuffers[instanceBufferId].attributes, instanceBuffers[instanceBufferId].attributesSize, 1);
	}

	void VertexArray::enableAttributes(const vector<GLint>& attributes, GLsizei size, GLuint divisor)
	{
		int offset = 0;

		for (const auto& attribute : attributes) {
			for (int i = 0; i < attribute; i += 4) {
				int attributeSize = std::min(attribute - i, 4);

				glCheck(glVertexAttribPointer(attributeIndex, attributeSize, GL_FLOAT, GL_FALSE, size, (GLvoid*)offset));
				glCheck(glVertexAttribDivisor(attributeIndex, divisor));
				glCheck(glEnableVertexAttribArray(attributeIndex));

				offset += attributeSize * sizeof(GLfloat);
				attributeIndex++;
			}
		}
	}

	std::unordered_map<GLuint, VertexArray::VertexBuffer> VertexArray::vertexBuffers;
	std::unordered_map<GLuint, VertexArray::IndexBuffer> VertexArray::indexBuffers;
	std::unordered_map<GLuint, VertexArray::InstanceBuffer> VertexArray::instanceBuffers;
}
