#include "VertexArray.h"

#include <algorithm>

namespace Framework
{
	VertexArray::~VertexArray()
	{
		if (hasContext()) {
			int numberOfReferences;

			numberOfReferences = --vertexBuffers[vertexBufferId].numberOfReferences;
			if (!numberOfReferences) {
				checkGL(glDeleteBuffers(1, &vertexBufferId));
			}

			numberOfReferences = --indexBuffers[indexBufferId].numberOfReferences;
			if (!numberOfReferences) {
				checkGL(glDeleteBuffers(1, &indexBufferId));
			}

			numberOfReferences = --instanceBuffers[instanceBufferId].numberOfReferences;
			if (!numberOfReferences) {
				checkGL(glDeleteBuffers(1, &instanceBufferId));
			}

			checkGL(glDeleteVertexArrays(1, &id));
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

		checkGL(glBindVertexArray(id));
		checkGL(glGenBuffers(1, &vertexBufferId));
		checkGL(glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId));
		checkGL(glBufferData(GL_ARRAY_BUFFER, numberOfVertices * attributesSize, vertices, vertices ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW));
		vertexBuffers[vertexBufferId] = { numberOfVertices, attributes, attributesSize };
		enableVertexAttributes();
		checkGL(glBindVertexArray(0));
	}

	void VertexArray::setIndexBuffer(int numberOfIndices, const GLushort indices[])
	{
		if (indexBufferId) {
			error("Index buffer object has already been set");
			return;
		}

		create();

		checkGL(glBindVertexArray(id));
		checkGL(glGenBuffers(1, &indexBufferId));
		checkGL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId));
		checkGL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, numberOfIndices * sizeof(GLushort), indices, indices ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW));
		indexBuffers[indexBufferId] = { numberOfIndices };
		checkGL(glBindVertexArray(0));
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

		checkGL(glBindVertexArray(id));
		checkGL(glGenBuffers(1, &instanceBufferId));
		checkGL(glBindBuffer(GL_ARRAY_BUFFER, instanceBufferId));
		checkGL(glBufferData(GL_ARRAY_BUFFER, numberOfInstances * attributesSize, instances, instances ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW));
		instanceBuffers[instanceBufferId] = { numberOfInstances, attributes, attributesSize };
		enableInstanceAttributes();
		checkGL(glBindVertexArray(0));
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

		checkGL(glBindVertexArray(this->id));
		checkGL(glBindBuffer(GL_ARRAY_BUFFER, id));
		enableVertexAttributes();
		checkGL(glBindVertexArray(0));
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

		checkGL(glBindVertexArray(this->id));
		checkGL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId));
		checkGL(glBindVertexArray(0));
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

		checkGL(glBindVertexArray(this->id));
		checkGL(glBindBuffer(GL_ARRAY_BUFFER, id));
		enableInstanceAttributes();
		checkGL(glBindVertexArray(0));
	}

	void VertexArray::updateVertexBuffer(int numberOfVertices, const void* vertices) const
	{
		if (!vertexBufferId) {
			error("Could not update non-existing vertex buffer object");
			return;
		}

		vertexBuffers[vertexBufferId].numberOfVertices = numberOfVertices;
		checkGL(glNamedBufferSubData(vertexBufferId, 0, numberOfVertices * vertexBuffers[vertexBufferId].attributesSize, vertices));
	}

	void VertexArray::updateIndexBuffer(int numberOfIndices, const GLushort indices[]) const
	{
		if (!indexBufferId) {
			error("Could not update non-existing index buffer object");
			return;
		}

		indexBuffers[indexBufferId].numberOfIndices = numberOfIndices;
		checkGL(glNamedBufferSubData(indexBufferId, 0, numberOfIndices * sizeof(GLushort), indices));
	}

	void VertexArray::updateInstanceBuffer(int numberOfInstances, const void* instances) const
	{
		if (!instanceBufferId) {
			error("Could not update non-existing instance buffer object");
			return;
		}

		instanceBuffers[instanceBufferId].numberOfInstances = numberOfInstances;
		checkGL(glNamedBufferSubData(instanceBufferId, 0, numberOfInstances * instanceBuffers[instanceBufferId].attributesSize, instances));
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
		checkGL(glBindVertexArray(id));

		if (instanceBufferId && instanceBuffers[instanceBufferId].numberOfInstances) {
			if (indexBufferId && indexBuffers[indexBufferId].numberOfIndices) {
				checkGL(glDrawElementsInstanced(mode, indexBuffers[indexBufferId].numberOfIndices, GL_UNSIGNED_SHORT, nullptr, instanceBuffers[instanceBufferId].numberOfInstances));
			}
			else if (vertexBufferId && vertexBuffers[vertexBufferId].numberOfVertices) {
				checkGL(glDrawArraysInstanced(mode, 0, vertexBuffers[vertexBufferId].numberOfVertices, instanceBuffers[instanceBufferId].numberOfInstances));
			}
			else {
				error("Insufficient data to draw using vertex array object");
			}
		}
		else if (indexBufferId && indexBuffers[indexBufferId].numberOfIndices) {
			checkGL(glDrawElements(mode, indexBuffers[indexBufferId].numberOfIndices, GL_UNSIGNED_SHORT, nullptr));
		}
		else if (vertexBufferId && vertexBuffers[vertexBufferId].numberOfVertices) {
			checkGL(glDrawArrays(mode, 0, vertexBuffers[vertexBufferId].numberOfVertices));
		}
		else {
			error("Insufficient data to draw using vertex array object");
		}

		checkGL(glBindVertexArray(0));
	}

	void VertexArray::create()
	{
		if (id) {
			return;
		}

		if (!hasContext("Could not create vertex array object")) {
			return;
		}

		checkGL(glGenVertexArrays(1, &id));
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

				checkGL(glVertexAttribPointer(attributeIndex, attributeSize, GL_FLOAT, GL_FALSE, size, (GLvoid*)offset));
				checkGL(glVertexAttribDivisor(attributeIndex, divisor));
				checkGL(glEnableVertexAttribArray(attributeIndex));

				offset += attributeSize * sizeof(GLfloat);
				attributeIndex++;
			}
		}
	}

	std::unordered_map<GLuint, VertexArray::VertexBuffer> VertexArray::vertexBuffers;
	std::unordered_map<GLuint, VertexArray::IndexBuffer> VertexArray::indexBuffers;
	std::unordered_map<GLuint, VertexArray::InstanceBuffer> VertexArray::instanceBuffers;
}
