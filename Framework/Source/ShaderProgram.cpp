#include "ShaderProgram.hpp"
#include "VertexShader.hpp"
#include "FragmentShader.hpp"
#include "Error.hpp"

#include <glm\gtc\type_ptr.hpp>

namespace Framework
{
	ShaderProgram::ShaderProgram(const std::string& vsFilename, const std::string& fsFilename, bool fromFile) :
		ShaderProgram({
			&Framework::VertexShader(vsFilename, fromFile),
			&Framework::FragmentShader(fsFilename, fromFile),
			})
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

		id_ = glCheckV(glCreateProgram());

		for (GLShader* shader : shaders) {
			glCheck(glAttachShader(id_, shader->getId()));
		}

		glCheck(glLinkProgram(id_));

		GLint linkStatus;
		glCheck(glGetProgramiv(id_, GL_LINK_STATUS, &linkStatus));
		if (!linkStatus) {
			GLint infoLogLength;
			glCheck(glGetProgramiv(id_, GL_INFO_LOG_LENGTH, &infoLogLength));
			char* infoLog = new char[infoLogLength + 1];
			glCheck(glGetProgramInfoLog(id_, infoLogLength, nullptr, infoLog));
			error("Program info log:\n" + std::string(infoLog));
			delete[] infoLog;
		}

		for (GLShader* shader : shaders) {
			glCheck(glDetachShader(id_, shader->getId()));
		}
	}

	ShaderProgram::~ShaderProgram()
	{
		glCheck(glDeleteProgram(id_));
	}

	void ShaderProgram::use() const
	{
		glCheck(glUseProgram(id_));
	}

	void ShaderProgram::setUniform(const std::string& name, float value) const
	{
		glCheck(glUniform1f(getUniformLocation_(name), value));
	}

	void ShaderProgram::setUniform(const std::string& name, const glm::vec2& value) const
	{
		glCheck(glUniform2f(getUniformLocation_(name), value.x, value.y));
	}

	void ShaderProgram::setUniform(const std::string& name, const glm::vec3& value) const
	{
		glCheck(glUniform3f(getUniformLocation_(name), value.x, value.y, value.z));
	}

	void ShaderProgram::setUniform(const std::string& name, const glm::vec4& value) const
	{
		glCheck(glUniform4f(getUniformLocation_(name), value.x, value.y, value.z, value.w));
	}

	void ShaderProgram::setUniform(const std::string& name, int value) const
	{
		glCheck(glUniform1i(getUniformLocation_(name), value));
	}

	void ShaderProgram::setUniform(const std::string& name, const glm::ivec2& value) const
	{
		glCheck(glUniform2i(getUniformLocation_(name), value.x, value.y));
	}

	void ShaderProgram::setUniform(const std::string& name, const glm::ivec3& value) const
	{
		glCheck(glUniform3i(getUniformLocation_(name), value.x, value.y, value.z));
	}

	void ShaderProgram::setUniform(const std::string& name, const glm::ivec4& value) const
	{
		glCheck(glUniform4i(getUniformLocation_(name), value.x, value.y, value.z, value.w));
	}

	void ShaderProgram::setUniform(const std::string& name, unsigned int value) const
	{
		glCheck(glUniform1ui(getUniformLocation_(name), value));
	}

	void ShaderProgram::setUniform(const std::string& name, const glm::uvec2& value) const
	{
		glCheck(glUniform2ui(getUniformLocation_(name), value.x, value.y));
	}

	void ShaderProgram::setUniform(const std::string& name, const glm::uvec3& value) const
	{
		glCheck(glUniform3ui(getUniformLocation_(name), value.x, value.y, value.z));
	}

	void ShaderProgram::setUniform(const std::string& name, const glm::uvec4& value) const
	{
		glCheck(glUniform4ui(getUniformLocation_(name), value.x, value.y, value.z, value.w));
	}

	void ShaderProgram::setUniform(const std::string& name, const glm::mat2& value, bool transpose) const
	{
		glCheck(glUniformMatrix2fv(getUniformLocation_(name), 1, transpose, glm::value_ptr(value)));
	}

	void ShaderProgram::setUniform(const std::string& name, const glm::mat3& value, bool transpose) const
	{
		glCheck(glUniformMatrix3fv(getUniformLocation_(name), 1, transpose, glm::value_ptr(value)));
	}

	void ShaderProgram::setUniform(const std::string& name, const glm::mat4& value, bool transpose) const
	{
		glCheck(glUniformMatrix4fv(getUniformLocation_(name), 1, transpose, &value[0][0]));
	}

	GLint ShaderProgram::getUniformLocation_(const std::string& name) const
	{
		return glGetUniformLocation(id_, name.c_str());
	}
}
