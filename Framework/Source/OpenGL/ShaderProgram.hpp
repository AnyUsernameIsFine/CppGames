#pragma once

#include "Shader.hpp"

#define GLM_FORCE_INLINE 
#include <glm\glm.hpp>

#include <initializer_list>

namespace Framework
{
	class ShaderProgram : OpenGLObject
	{
	public:
		ShaderProgram(const string& vsFilename, const string& fsFilename, bool fromFile = true);
		ShaderProgram(const std::initializer_list<GLShader*>& shaders);
		~ShaderProgram();

		void use() const;
		void setUniform(const string& name, float value) const;
		void setUniform(const string& name, const glm::vec2& value) const;
		void setUniform(const string& name, const glm::vec3& value) const;
		void setUniform(const string& name, const glm::vec4& value) const;
		void setUniform(const string& name, int value) const;
		void setUniform(const string& name, const glm::ivec2& value) const;
		void setUniform(const string& name, const glm::ivec3& value) const;
		void setUniform(const string& name, const glm::ivec4& value) const;
		void setUniform(const string& name, uint value) const;
		void setUniform(const string& name, const glm::uvec2& value) const;
		void setUniform(const string& name, const glm::uvec3& value) const;
		void setUniform(const string& name, const glm::uvec4& value) const;
		void setUniform(const string& name, const glm::mat2& value, bool transpose = false) const;
		void setUniform(const string& name, const glm::mat3& value, bool transpose = false) const;
		void setUniform(const string& name, const glm::mat4& value, bool transpose = false) const;

	private:
		GLint getUniformLocation(const string& name) const;
	};
}
