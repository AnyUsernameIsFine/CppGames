#pragma once

#include <glm\glm.hpp>
#include "OpenGLObject.h"

namespace GLEngine
{
	class Shader : OpenGLObject
	{
	public:
		~Shader();
		void createFromFiles(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename);
		void createFromSource(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
		void use() const;
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
		std::string shaderSourceFromFile(const std::string& filename) const;
		GLuint shaderFromSource(GLenum type, const std::string& source) const;
		GLint getUniformLocation(const std::string& name) const;
	};
}
