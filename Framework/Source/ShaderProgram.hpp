#pragma once

#include "Shader.hpp"
#include "Texture2D.hpp"

#include <glm\glm.hpp>

#include <string>
#include <initializer_list>
#include <vector>

namespace Framework
{
	class ShaderProgram : OpenGLObject
	{
	public:
		ShaderProgram(const std::string& vsFilename, const std::string& fsFilename, bool fromFile = true);
		ShaderProgram(const std::initializer_list<GLShader*>& shaders);
		~ShaderProgram();

		void use() const;
		void setUniform(const std::string& name, float value) const;
		void setUniform(const std::string& name, const glm::vec2& value) const;
		void setUniform(const std::string& name, const glm::vec3& value) const;
		void setUniform(const std::string& name, const glm::vec4& value) const;
		void setUniform(const std::string& name, int value) const;
		void setUniform(const std::string& name, const glm::ivec2& value) const;
		void setUniform(const std::string& name, const glm::ivec3& value) const;
		void setUniform(const std::string& name, const glm::ivec4& value) const;
		void setUniform(const std::string& name, unsigned int value) const;
		void setUniform(const std::string& name, const glm::uvec2& value) const;
		void setUniform(const std::string& name, const glm::uvec3& value) const;
		void setUniform(const std::string& name, const glm::uvec4& value) const;
		void setUniform(const std::string& name, const glm::mat2& value, bool transpose = false) const;
		void setUniform(const std::string& name, const glm::mat3& value, bool transpose = false) const;
		void setUniform(const std::string& name, const glm::mat4& value, bool transpose = false) const;

	private:
		GLint getUniformLocation_(const std::string& name) const;
	};
}
