#pragma once

#include "OpenGLObject.hpp"

#include <glm\glm.hpp>

namespace Framework
{
	class Shader : OpenGLObject
	{
	public:
		void createFromFiles(const string& vertexShaderFilename, const string& fragmentShaderFilename);
		void createFromSource(const string& vertexShaderSource, const string& fragmentShaderSource);

		void use() const;
		void setUniform(const string& name, float value) const;
		void setUniform(const string& name, const glm::vec2& value) const;
		void setUniform(const string& name, const glm::vec3& value) const;
		void setUniform(const string& name, const glm::vec4& value) const;
		void setUniform(const string& name, int value) const;
		void setUniform(const string& name, const glm::ivec2& value) const;
		void setUniform(const string& name, const glm::ivec3& value) const;
		void setUniform(const string& name, const glm::ivec4& value) const;
		void setUniform(const string& name, uInt value) const;
		void setUniform(const string& name, const glm::uvec2& value) const;
		void setUniform(const string& name, const glm::uvec3& value) const;
		void setUniform(const string& name, const glm::uvec4& value) const;
		void setUniform(const string& name, const glm::mat2& value, bool transpose = false) const;
		void setUniform(const string& name, const glm::mat3& value, bool transpose = false) const;
		void setUniform(const string& name, const glm::mat4& value, bool transpose = false) const;

	private:
		string shaderSourceFromFile(const string& filename) const;
		GLuint shaderFromSource(GLenum type, const string& source) const;
		GLint getUniformLocation(const string& name) const;
	};
}
