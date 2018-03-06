#include "VertexArray.h"

#include <algorithm>

namespace GLEngine
{
	VertexArray::~VertexArray()
	{
		if (hasContext()) {
			int numReferences;

			numReferences = --vertexBuffers[vertexBufferId].numReferences;
			if (!numReferences) {
				checkGL(glDeleteBuffers(1, &vertexBufferId));
			}

			numReferences = --indexBuffers[indexBufferId].numReferences;
			if (!numReferences) {
				checkGL(glDeleteBuffers(1, &indexBufferId));
			}

			numReferences = --instanceBuffers[instanceBufferId].numReferences;
			if (!numReferences) {
				checkGL(glDeleteBuffers(1, &instanceBufferId));
			}

			checkGL(glDeleteVertexArrays(1, &id));
		}
	}

	void VertexArray::setVertexBuffer(const std::vector<int>& attributes, int numVertices, const void* vertices)
	{
		if (vertexBufferId) {
			error("Vertex buffer object has already been set");
			return;
		}

		create();

		int attributesSize = getAttributesSize(attributes);

		checkGL(glBindVertexArray(id));
		checkGL(glGenBuffers(1, &vertexBufferId));
		checkGL(glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId));
		checkGL(glBufferData(GL_ARRAY_BUFFER, numVertices * attributesSize, vertices, vertices ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW));
		vertexBuffers[vertexBufferId] = { numVertices, attributes, attributesSize };
		enableVertexAttributes();
		checkGL(glBindVertexArray(0));
	}

	void VertexArray::setIndexBuffer(int numIndices, const GLushort indices[])
	{
		if (indexBufferId) {
			error("Index buffer object has already been set");
			return;
		}

		create();

		checkGL(glBindVertexArray(id));
		checkGL(glGenBuffers(1, &indexBufferId));
		checkGL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId));
		checkGL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(GLushort), indices, indices ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW));
		indexBuffers[indexBufferId] = { numIndices };
		checkGL(glBindVertexArray(0));
	}

	void VertexArray::setIndexBuffer(const std::vector<GLushort>& indices)
	{
		setIndexBuffer(static_cast<int>(indices.size()), indices.data());
	}

	void VertexArray::setInstanceBuffer(const std::vector<int>& attributes, int numInstances, const void* instances)
	{
		if (instanceBufferId) {
			error("Instance buffer object has already been set");
			return;
		}

		create();

		int attributesSize = getAttributesSize(attributes);

		checkGL(glBindVertexArray(id));
		checkGL(glGenBuffers(1, &instanceBufferId));
		checkGL(glBindBuffer(GL_ARRAY_BUFFER, instanceBufferId));
		checkGL(glBufferData(GL_ARRAY_BUFFER, numInstances * attributesSize, instances, instances ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW));
		instanceBuffers[instanceBufferId] = { numInstances, attributes, attributesSize };
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
		vertexBuffers[id].numReferences++;

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
		indexBuffers[id].numReferences++;

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
		instanceBuffers[id].numReferences++;

		checkGL(glBindVertexArray(this->id));
		checkGL(glBindBuffer(GL_ARRAY_BUFFER, id));
		enableInstanceAttributes();
		checkGL(glBindVertexArray(0));
	}

	void VertexArray::updateVertexBuffer(int numVertices, const void* vertices) const
	{
		if (!vertexBufferId) {
			error("Could not update non-existing vertex buffer object");
			return;
		}

		vertexBuffers[vertexBufferId].numVertices = numVertices;
		checkGL(glNamedBufferSubData(vertexBufferId, 0, numVertices * vertexBuffers[vertexBufferId].attributesSize, vertices));
	}

	void VertexArray::updateIndexBuffer(int numIndices, const GLushort indices[]) const
	{
		if (!indexBufferId) {
			error("Could not update non-existing index buffer object");
			return;
		}

		indexBuffers[indexBufferId].numIndices = numIndices;
		checkGL(glNamedBufferSubData(indexBufferId, 0, numIndices * sizeof(GLushort), indices));
	}

	void VertexArray::updateIndexBuffer(const std::vector<GLushort>& indices) const
	{
		updateIndexBuffer(static_cast<int>(indices.size()), indices.data());
	}

	void VertexArray::updateInstanceBuffer(int numInstances, const void* instances) const
	{
		if (!instanceBufferId) {
			error("Could not update non-existing instance buffer object");
			return;
		}

		instanceBuffers[instanceBufferId].numInstances = numInstances;
		checkGL(glNamedBufferSubData(instanceBufferId, 0, numInstances * instanceBuffers[instanceBufferId].attributesSize, instances));
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

		if (instanceBufferId && instanceBuffers[instanceBufferId].numInstances) {
			if (indexBufferId && indexBuffers[indexBufferId].numIndices) {
				checkGL(glDrawElementsInstanced(mode, indexBuffers[indexBufferId].numIndices, GL_UNSIGNED_SHORT, nullptr, instanceBuffers[instanceBufferId].numInstances));
			}
			else if (vertexBufferId && vertexBuffers[vertexBufferId].numVertices) {
				checkGL(glDrawArraysInstanced(mode, 0, vertexBuffers[vertexBufferId].numVertices, instanceBuffers[instanceBufferId].numInstances));
			}
			else {
				error("Insufficient data to draw using vertex array object");
			}
		}
		else if (indexBufferId && indexBuffers[indexBufferId].numIndices) {
			checkGL(glDrawElements(mode, indexBuffers[indexBufferId].numIndices, GL_UNSIGNED_SHORT, nullptr));
		}
		else if (vertexBufferId && vertexBuffers[vertexBufferId].numVertices) {
			checkGL(glDrawArrays(mode, 0, vertexBuffers[vertexBufferId].numVertices));
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

	void VertexArray::enableAttributes(const std::vector<GLint>& attributes, GLsizei size, GLuint divisor)
	{
		int offset = 0;

		for (const auto& attribute : attributes) {
			for (int i = 0; i < attribute; i += 4) {
				int attributeSize = std::min(attribute - i, 4);

				checkGL(glVertexAttribPointer(attributeIndex, attributeSize, GL_FLOAT, GL_FALSE, size, reinterpret_cast<GLvoid*>(offset)));
				checkGL(glVertexAttribDivisor(attributeIndex, divisor));
				checkGL(glEnableVertexAttribArray(attributeIndex));

				offset += attributeSize * sizeof(GLfloat);
				attributeIndex++;
			}
		}
	}

	GLsizei VertexArray::getAttributesSize(const std::vector<GLint>& attributes)
	{
		GLsizei attributesSize = 0;
		for (const auto& attribute : attributes) {
			attributesSize += attribute * sizeof(GLfloat);
		}

		return attributesSize;
	}

	std::unordered_map<GLuint, VertexArray::VertexBuffer> VertexArray::vertexBuffers;
	std::unordered_map<GLuint, VertexArray::IndexBuffer> VertexArray::indexBuffers;
	std::unordered_map<GLuint, VertexArray::InstanceBuffer> VertexArray::instanceBuffers;
}
