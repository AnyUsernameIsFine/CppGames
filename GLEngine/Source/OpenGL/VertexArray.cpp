#include "VertexArray.h"

#include <algorithm>

namespace GLEngine
{
	VertexArray::~VertexArray()
	{
		if (hasContext()) {
			if (!--vertexBuffer->numReferences) {
				checkGL(glDeleteBuffers(1, &vertexBuffer->id));
				vertexBuffers.erase(vertexBuffer->id);
			}

			if (!--indexBuffer->numReferences) {
				checkGL(glDeleteBuffers(1, &indexBuffer->id));
				indexBuffers.erase(indexBuffer->id);
			}

			if (!--instanceBuffer->numReferences) {
				checkGL(glDeleteBuffers(1, &instanceBuffer->id));
				instanceBuffers.erase(instanceBuffer->id);
			}

			checkGL(glDeleteVertexArrays(1, &id));
		}
	}

	void VertexArray::setVertexBuffer(const std::vector<int>& attributes, int numVertices, const void* vertices)
	{
		if (vertexBuffer) {
			error("Vertex buffer object has already been set");
			return;
		}

		create();

		int attributesSize = getAttributesSize(attributes);

		GLuint vertexBufferId;
		checkGL(glGenBuffers(1, &vertexBufferId));
		checkGL(glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId));
		checkGL(glBufferData(GL_ARRAY_BUFFER, numVertices * attributesSize, vertices, vertices ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW));
		vertexBuffer = std::make_shared<VertexBuffer>(VertexBuffer{ vertexBufferId, numVertices, attributes, attributesSize });
		vertexBuffers[vertexBufferId] = vertexBuffer;
		enableVertexAttributes();
	}

	void VertexArray::setIndexBuffer(int numIndices, const GLushort indices[])
	{
		if (indexBuffer) {
			error("Index buffer object has already been set");
			return;
		}

		create();

		GLuint indexBufferId;
		checkGL(glGenBuffers(1, &indexBufferId));
		checkGL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId));
		checkGL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(GLushort), indices, indices ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW));
		indexBuffer = std::make_shared<IndexBuffer>(IndexBuffer{ indexBufferId, numIndices });
		indexBuffers[indexBufferId] = indexBuffer;
	}

	void VertexArray::setIndexBuffer(const std::vector<GLushort>& indices)
	{
		setIndexBuffer(static_cast<int>(indices.size()), indices.data());
	}

	void VertexArray::setInstanceBuffer(const std::vector<int>& attributes, int numInstances, const void* instances)
	{
		if (instanceBuffer) {
			error("Instance buffer object has already been set");
			return;
		}

		create();

		int attributesSize = getAttributesSize(attributes);

		GLuint instanceBufferId;
		checkGL(glGenBuffers(1, &instanceBufferId));
		checkGL(glBindBuffer(GL_ARRAY_BUFFER, instanceBufferId));
		checkGL(glBufferData(GL_ARRAY_BUFFER, numInstances * attributesSize, instances, instances ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW));
		instanceBuffer = std::make_shared<InstanceBuffer>(InstanceBuffer{ instanceBufferId, numInstances, attributes, attributesSize });
		instanceBuffers[instanceBufferId] = instanceBuffer;
		enableInstanceAttributes();
	}

	void VertexArray::setExistingVertexBuffer(GLuint id)
	{
		if (vertexBuffer) {
			error("Vertex buffer object has already been set");
			return;
		}

		create();

		vertexBuffer = vertexBuffers[id];
		vertexBuffer->numReferences++;

		checkGL(glBindBuffer(GL_ARRAY_BUFFER, id));
		enableVertexAttributes();
	}

	void VertexArray::setExistingIndexBuffer(GLuint id)
	{
		if (indexBuffer) {
			error("Index buffer object has already been set");
			return;
		}

		create();

		indexBuffer = indexBuffers[id];
		indexBuffer->numReferences++;

		checkGL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id));
	}

	void VertexArray::setExistingInstanceBuffer(GLuint id)
	{
		if (instanceBuffer) {
			error("Instance buffer object has already been set");
			return;
		}

		create();

		instanceBuffer = instanceBuffers[id];
		instanceBuffer->numReferences++;

		checkGL(glBindBuffer(GL_ARRAY_BUFFER, id));
		enableInstanceAttributes();
	}

	void VertexArray::updateVertexBuffer(int numVertices, const void* vertices) const
	{
		if (!vertexBuffer) {
			error("Could not update non-existing vertex buffer object");
			return;
		}

		vertexBuffer->numVertices = numVertices;
		if (numVertices) {
			checkGL(glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer->id));
			checkGL(glBufferSubData(GL_ARRAY_BUFFER, 0, numVertices * vertexBuffer->attributesSize, vertices));
		}
	}

	void VertexArray::updateIndexBuffer(int numIndices, const GLushort indices[]) const
	{
		if (!indexBuffer) {
			error("Could not update non-existing index buffer object");
			return;
		}

		indexBuffer->numIndices = numIndices;
		if (numIndices) {
			checkGL(glBindBuffer(GL_ARRAY_BUFFER, indexBuffer->id));
			checkGL(glBufferSubData(GL_ARRAY_BUFFER, 0, numIndices * sizeof(GLushort), indices));
		}
	}

	void VertexArray::updateIndexBuffer(const std::vector<GLushort>& indices) const
	{
		updateIndexBuffer(static_cast<int>(indices.size()), indices.data());
	}

	void VertexArray::updateInstanceBuffer(int numInstances, const void* instances) const
	{
		if (!instanceBuffer) {
			error("Could not update non-existing instance buffer object");
			return;
		}

		instanceBuffer->numInstances = numInstances;
		if (numInstances) {
			checkGL(glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer->id));
			checkGL(glBufferSubData(GL_ARRAY_BUFFER, 0, numInstances * instanceBuffer->attributesSize, instances));
		}
	}

	GLuint VertexArray::getVertexBufferId() const
	{
		return vertexBuffer->id;
	}

	GLuint VertexArray::getIndexBufferId() const
	{
		return indexBuffer->id;
	}

	GLuint VertexArray::getInstanceBufferId() const
	{
		return instanceBuffer->id;
	}

	void VertexArray::draw(GLenum mode) const
	{
		bind();

		if (instanceBuffer && instanceBuffer->numInstances) {
			if (indexBuffer && indexBuffer->numIndices) {
				checkGL(glDrawElementsInstanced(mode, indexBuffer->numIndices, GL_UNSIGNED_SHORT, nullptr, instanceBuffer->numInstances));
			}
			else if (vertexBuffer && vertexBuffer->numVertices) {
				checkGL(glDrawArraysInstanced(mode, 0, vertexBuffer->numVertices, instanceBuffer->numInstances));
			}
			else {
				error("Insufficient data to draw using vertex array object");
			}
		}
		else if (indexBuffer && indexBuffer->numIndices) {
			checkGL(glDrawElements(mode, indexBuffer->numIndices, GL_UNSIGNED_SHORT, nullptr));
		}
		else if (vertexBuffer && vertexBuffer->numVertices) {
			checkGL(glDrawArrays(mode, 0, vertexBuffer->numVertices));
		}
		else {
			error("Insufficient data to draw using vertex array object");
		}
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
		bind();
	}

	void VertexArray::enableVertexAttributes()
	{
		enableAttributes(vertexBuffer->attributes, vertexBuffer->attributesSize);
	}

	void VertexArray::enableInstanceAttributes()
	{
		enableAttributes(instanceBuffer->attributes, instanceBuffer->attributesSize, 1);
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

	void VertexArray::bind() const
	{
		if (boundId != id) {
			checkGL(glBindVertexArray(id));
			boundId = id;
		}
	}

	GLuint VertexArray::boundId = 0;
	std::unordered_map<GLuint, std::shared_ptr<VertexArray::VertexBuffer>> VertexArray::vertexBuffers;
	std::unordered_map<GLuint, std::shared_ptr<VertexArray::IndexBuffer>> VertexArray::indexBuffers;
	std::unordered_map<GLuint, std::shared_ptr<VertexArray::InstanceBuffer>> VertexArray::instanceBuffers;
}
