#pragma once

#include "GLShader.h"
#include "GLTexture2D.h"

#include <glm\glm.hpp>

#include <string>
#include <initializer_list>
#include <vector>

namespace Framework
{
	class GLProgram : GLObject
	{
	public:
		GLProgram(const std::string& vsFilename, const std::string& fsFilename);
		GLProgram(std::initializer_list<GLShader*> shaders);
		~GLProgram();

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
		const GLint getUniformLocation_(const std::string& name) const;
	};
}
