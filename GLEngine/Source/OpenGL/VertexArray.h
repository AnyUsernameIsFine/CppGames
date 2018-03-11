#pragma once

#include <memory>
#include <unordered_map>

#include "OpenGLObject.h"

namespace GLEngine
{
	class VertexArray : public OpenGLObject
	{
	public:
		VertexArray() {}
		~VertexArray();
		void setVertexBuffer(const std::vector<int>& attributes, int numVertices, const void* vertices = nullptr);
		template<typename T> void setVertexBuffer(const std::vector<int>& attributes, const std::vector<T>& vertices);
		void setIndexBuffer(int numIndices, const GLushort indices[] = nullptr);
		void setIndexBuffer(const std::vector<GLushort>& indices);
		void setInstanceBuffer(const std::vector<int>& attributes, int numInstances, const void* instances = nullptr);
		template<typename T> void setInstanceBuffer(const std::vector<int>& attributes, const std::vector<T>& instances);
		void setExistingVertexBuffer(GLuint id);
		void setExistingIndexBuffer(GLuint id);
		void setExistingInstanceBuffer(GLuint id);
		void updateVertexBuffer(int numVertices, const void* vertices) const;
		template<typename T> void updateVertexBuffer(const std::vector<T>& vertices) const;
		void updateIndexBuffer(int numIndices, const GLushort indices[]) const;
		void updateIndexBuffer(const std::vector<GLushort>& indices) const;
		void updateInstanceBuffer(int numInstances, const void* instances) const;
		template<typename T> void updateInstanceBuffer(const std::vector<T>& instances) const;
		GLuint getVertexBufferId() const;
		GLuint getIndexBufferId() const;
		GLuint getInstanceBufferId() const;
		void draw(GLenum mode) const;
		VertexArray(const VertexArray&) = delete;
		VertexArray(VertexArray&& other) = delete;
		VertexArray &operator=(const VertexArray&) = delete;
		VertexArray &operator=(VertexArray&& other) = delete;

	private:
		struct VertexBuffer
		{
			GLuint id;
			GLsizei numVertices;
			std::vector<GLint> attributes;
			GLsizei attributesSize;
			int numReferences = 1;
		};

		struct IndexBuffer
		{
			GLuint id;
			GLsizei numIndices;
			int numReferences = 1;
		};

		struct InstanceBuffer
		{
			GLuint id;
			GLsizei numInstances;
			std::vector<GLint> attributes;
			GLsizei attributesSize;
			int numReferences = 1;
		};

		void create();
		void enableVertexAttributes();
		void enableInstanceAttributes();
		void enableAttributes(const std::vector<GLint>& attributes, GLsizei size, GLuint divisor = 0);
		void bind() const;

		static GLsizei getAttributesSize(const std::vector<GLint>& attributes);

		std::shared_ptr<VertexBuffer> vertexBuffer = nullptr;
		std::shared_ptr<IndexBuffer> indexBuffer = nullptr;
		std::shared_ptr<InstanceBuffer> instanceBuffer = nullptr;
		GLuint attributeIndex = 0;

		static GLuint boundId;
		static std::unordered_map<GLuint, std::shared_ptr<VertexBuffer>> vertexBuffers;
		static std::unordered_map<GLuint, std::shared_ptr<IndexBuffer>> indexBuffers;
		static std::unordered_map<GLuint, std::shared_ptr<InstanceBuffer>> instanceBuffers;
	};

	template<typename T>
	void VertexArray::setVertexBuffer(const std::vector<int>& attributes, const std::vector<T>& vertices)
	{
		setVertexBuffer(attributes, static_cast<int>(vertices.size()), vertices.data());
	}

	template<typename T>
	void VertexArray::setInstanceBuffer(const std::vector<int>& attributes, const std::vector<T>& instances)
	{
		setInstanceBuffer(attributes, static_cast<int>(instances.size()), instances.data());
	}

	template<typename T>
	void VertexArray::updateVertexBuffer(const std::vector<T>& vertices) const
	{
		updateVertexBuffer(static_cast<int>(vertices.size()), vertices.data());
	}

	template<typename T>
	void VertexArray::updateInstanceBuffer(const std::vector<T>& instances) const
	{
		updateInstanceBuffer(static_cast<int>(instances.size()), instances.data());
	}
}
