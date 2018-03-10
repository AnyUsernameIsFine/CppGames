#pragma once

#include <glm\glm.hpp>
#include "OpenGLObject.h"

namespace GLEngine
{
	class Shader : OpenGLObject
	{
	public:
		~Shader();
		void createVertexFragmentFromFiles(const std::string& vertex, const std::string& fragment);
		void createVertexFragmentFromSource(const std::string& vertex, const std::string& fragment);
		void createVertexGeometryFragmentFromFiles(const std::string& vertex, const std::string& geometry, const std::string& fragment);
		void createVertexGeometryFragmentFromSource(const std::string& vertex, const std::string& geometry, const std::string& fragment);
		void createComputeFromFile(const std::string& compute);
		void createComputeFromSource(const std::string& compute);
		void use() const;
		void compute(int x, int y, int z) const;
		void setUniform(const std::string& name, float value) const;
		void setUniform(const std::string& name, const glm::vec2& value) const;
		void setUniform(const std::string& name, const glm::vec3& value) const;
		void setUniform(const std::string& name, const glm::vec4& value) const;
		void setUniform(const std::string& name, int value) const;
		void setUniform(const std::string& name, const glm::ivec2& value) const;
		void setUniform(const std::string& name, const glm::ivec3& value) const;
		void setUniform(const std::string& name, const glm::ivec4& value) const;
		void setUniform(const std::string& name, UInt value) const;
		void setUniform(const std::string& name, const glm::uvec2& value) const;
		void setUniform(const std::string& name, const glm::uvec3& value) const;
		void setUniform(const std::string& name, const glm::uvec4& value) const;
		void setUniform(const std::string& name, const glm::mat2& value, bool transpose = false) const;
		void setUniform(const std::string& name, const glm::mat3& value, bool transpose = false) const;
		void setUniform(const std::string& name, const glm::mat4& value, bool transpose = false) const;

	private:
		bool isCreatable() const;
		std::string shaderSourceFromFile(const std::string& filename) const;
		GLuint shaderFromSource(GLenum type, const std::string& source) const;
		void create(const std::initializer_list<GLuint>& shaderIds);
		GLint getUniformLocation(const std::string& name) const;
	};
}
