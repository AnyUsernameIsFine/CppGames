#pragma once

#include "OpenGLObject.h"

#include <unordered_map>

namespace Framework
{
	class VertexArray : public OpenGLObject
	{
	public:
		~VertexArray();
		void setVertexBuffer(const vector<int>& attributes, int numberOfVertices, const void* vertices = nullptr);
		void setIndexBuffer(int numberOfIndices, const GLushort indices[] = nullptr);
		void setInstanceBuffer(const vector<int>& attributes, int numberOfInstances, const void* instances = nullptr);
		void setExistingVertexBuffer(GLuint id);
		void setExistingIndexBuffer(GLuint id);
		void setExistingInstanceBuffer(GLuint id);
		void updateVertexBuffer(int numberOfVertices, const void* vertices) const;
		void updateIndexBuffer(int numberOfIndices, const GLushort indices[]) const;
		void updateInstanceBuffer(int numberOfInstances, const void* instances) const;
		GLuint getVertexBufferId() const;
		GLuint getIndexBufferId() const;
		GLuint getInstanceBufferId() const;
		void draw(GLenum mode) const;

	private:
		struct VertexBuffer
		{
			GLsizei numberOfVertices;
			vector<GLint> attributes;
			GLsizei attributesSize;
			int numberOfReferences = 1;
		};

		struct IndexBuffer
		{
			GLsizei numberOfIndices;
			int numberOfReferences = 1;
		};

		struct InstanceBuffer
		{
			GLsizei numberOfInstances;
			vector<GLint> attributes;
			GLsizei attributesSize;
			int numberOfReferences = 1;
		};

		GLuint vertexBufferId = 0;
		GLuint indexBufferId = 0;
		GLuint instanceBufferId = 0;
		GLuint attributeIndex = 0;

		void create();
		void enableVertexAttributes();
		void enableInstanceAttributes();
		void enableAttributes(const vector<GLint>& attributes, GLsizei size, GLuint divisor = 0);

		static std::unordered_map<GLuint, VertexBuffer> vertexBuffers;
		static std::unordered_map<GLuint, IndexBuffer> indexBuffers;
		static std::unordered_map<GLuint, InstanceBuffer> instanceBuffers;
	};
}
