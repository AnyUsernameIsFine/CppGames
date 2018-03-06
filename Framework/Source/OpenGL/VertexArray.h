#pragma once

#include <unordered_map>

#include "OpenGLObject.h"

namespace Framework
{
	class VertexArray : public OpenGLObject
	{
	public:
		~VertexArray();
		void setVertexBuffer(const std::vector<int>& attributes, int numVertices, const void* vertices = nullptr);
		void setIndexBuffer(int numIndices, const GLushort indices[] = nullptr);
		void setInstanceBuffer(const std::vector<int>& attributes, int numInstances, const void* instances = nullptr);
		void setExistingVertexBuffer(GLuint id);
		void setExistingIndexBuffer(GLuint id);
		void setExistingInstanceBuffer(GLuint id);
		void updateVertexBuffer(int numVertices, const void* vertices) const;
		void updateIndexBuffer(int numIndices, const GLushort indices[]) const;
		void updateInstanceBuffer(int numInstances, const void* instances) const;
		GLuint getVertexBufferId() const;
		GLuint getIndexBufferId() const;
		GLuint getInstanceBufferId() const;
		void draw(GLenum mode) const;

	private:
		struct VertexBuffer
		{
			GLsizei numVertices;
			std::vector<GLint> attributes;
			GLsizei attributesSize;
			int numReferences = 1;
		};

		struct IndexBuffer
		{
			GLsizei numIndices;
			int numReferences = 1;
		};

		struct InstanceBuffer
		{
			GLsizei numInstances;
			std::vector<GLint> attributes;
			GLsizei attributesSize;
			int numReferences = 1;
		};

		void create();
		void enableVertexAttributes();
		void enableInstanceAttributes();
		void enableAttributes(const std::vector<GLint>& attributes, GLsizei size, GLuint divisor = 0);

		static GLsizei getAttributesSize(const std::vector<GLint>& attributes);

		GLuint vertexBufferId = 0;
		GLuint indexBufferId = 0;
		GLuint instanceBufferId = 0;
		GLuint attributeIndex = 0;

		static std::unordered_map<GLuint, VertexBuffer> vertexBuffers;
		static std::unordered_map<GLuint, IndexBuffer> indexBuffers;
		static std::unordered_map<GLuint, InstanceBuffer> instanceBuffers;
	};
}
