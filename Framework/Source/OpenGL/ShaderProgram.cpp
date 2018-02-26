#include "ShaderProgram.hpp"
#include "VertexShader.hpp"
#include "FragmentShader.hpp"
#include "System\Error.hpp"

#define GLM_FORCE_INLINE
#include <glm\gtc\type_ptr.hpp>

namespace Framework
{
	ShaderProgram::ShaderProgram(const string& vsFilename, const string& fsFilename, bool fromFile) :
		ShaderProgram({
			&Framework::VertexShader(vsFilename, fromFile),
			&Framework::FragmentShader(fsFilename, fromFile) })
	{
		//
	}

	ShaderProgram::ShaderProgram(const std::initializer_list<GLShader*>& shaders)
	{
		bool error = false;

		for (GLShader* shader : shaders) {
			error |= !shader->getId();
		}

		if (error) {
			return;
		}

		id = glCheckV(glCreateProgram());

		for (GLShader* shader : shaders) {
			glCheck(glAttachShader(id, shader->getId()));
		}

		glCheck(glLinkProgram(id));

		GLint linkStatus;
		glCheck(glGetProgramiv(id, GL_LINK_STATUS, &linkStatus));
		if (!linkStatus) {
			GLint infoLogLength;
			glCheck(glGetProgramiv(id, GL_INFO_LOG_LENGTH, &infoLogLength));
			char* infoLog = new char[infoLogLength + 1];
			glCheck(glGetProgramInfoLog(id, infoLogLength, nullptr, infoLog));
			error("Program info log:\n" + string(infoLog));
			delete[] infoLog;
		}

		for (GLShader* shader : shaders) {
			glCheck(glDetachShader(id, shader->getId()));
		}
	}

	ShaderProgram::~ShaderProgram()
	{
		glCheck(glDeleteProgram(id));
	}

	void ShaderProgram::use() const
	{
		glCheck(glUseProgram(id));
	}

	void ShaderProgram::setUniform(const string& name, float value) const
	{
		glCheck(glUniform1f(getUniformLocation(name), value));
	}

	void ShaderProgram::setUniform(const string& name, const glm::vec2& value) const
	{
		glCheck(glUniform2f(getUniformLocation(name), value.x, value.y));
	}

	void ShaderProgram::setUniform(const string& name, const glm::vec3& value) const
	{
		glCheck(glUniform3f(getUniformLocation(name), value.x, value.y, value.z));
	}

	void ShaderProgram::setUniform(const string& name, const glm::vec4& value) const
	{
		glCheck(glUniform4f(getUniformLocation(name), value.x, value.y, value.z, value.w));
	}

	void ShaderProgram::setUniform(const string& name, int value) const
	{
		glCheck(glUniform1i(getUniformLocation(name), value));
	}

	void ShaderProgram::setUniform(const string& name, const glm::ivec2& value) const
	{
		glCheck(glUniform2i(getUniformLocation(name), value.x, value.y));
	}

	void ShaderProgram::setUniform(const string& name, const glm::ivec3& value) const
	{
		glCheck(glUniform3i(getUniformLocation(name), value.x, value.y, value.z));
	}

	void ShaderProgram::setUniform(const string& name, const glm::ivec4& value) const
	{
		glCheck(glUniform4i(getUniformLocation(name), value.x, value.y, value.z, value.w));
	}

	void ShaderProgram::setUniform(const string& name, uint value) const
	{
		glCheck(glUniform1ui(getUniformLocation(name), value));
	}

	void ShaderProgram::setUniform(const string& name, const glm::uvec2& value) const
	{
		glCheck(glUniform2ui(getUniformLocation(name), value.x, value.y));
	}

	void ShaderProgram::setUniform(const string& name, const glm::uvec3& value) const
	{
		glCheck(glUniform3ui(getUniformLocation(name), value.x, value.y, value.z));
	}

	void ShaderProgram::setUniform(const string& name, const glm::uvec4& value) const
	{
		glCheck(glUniform4ui(getUniformLocation(name), value.x, value.y, value.z, value.w));
	}

	void ShaderProgram::setUniform(const string& name, const glm::mat2& value, bool transpose) const
	{
		glCheck(glUniformMatrix2fv(getUniformLocation(name), 1, transpose, glm::value_ptr(value)));
	}

	void ShaderProgram::setUniform(const string& name, const glm::mat3& value, bool transpose) const
	{
		glCheck(glUniformMatrix3fv(getUniformLocation(name), 1, transpose, glm::value_ptr(value)));
	}

	void ShaderProgram::setUniform(const string& name, const glm::mat4& value, bool transpose) const
	{
		glCheck(glUniformMatrix4fv(getUniformLocation(name), 1, transpose, &value[0][0]));
	}

	GLint ShaderProgram::getUniformLocation(const string& name) const
	{
		return glGetUniformLocation(id, name.c_str());
	}
}
