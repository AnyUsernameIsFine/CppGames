#include "Shader.h"

#include <glm\gtc\type_ptr.hpp>

#include <fstream>

namespace Framework
{
	Shader::~Shader()
	{
		if (hasContext()) {
			console("shader " << id << " deleted");
			checkGL(glDeleteShader(id));
		};
	}

	void Shader::createFromFiles(const string& vertexShaderFilename, const string& fragmentShaderFilename)
	{
		if (id) {
			error("Shader has already been created");
			return;
		}

		string vertexShaderSource = shaderSourceFromFile(vertexShaderFilename);
		string fragmentShaderSource = shaderSourceFromFile(fragmentShaderFilename);

		createFromSource(vertexShaderSource, fragmentShaderSource);
	}

	void Shader::createFromSource(const string& vertexShaderSource, const string& fragmentShaderSource)
	{
		if (id) {
			error("Shader has already been created");
			return;
		}

		if (!hasContext("Could not create shader")) {
			return;
		}

		id = checkGLValue(glCreateProgram());

		GLuint vertexShaderId = shaderFromSource(GL_VERTEX_SHADER, vertexShaderSource);
		GLuint fragmentShaderId = shaderFromSource(GL_FRAGMENT_SHADER, fragmentShaderSource);

		checkGL(glAttachShader(id, vertexShaderId));
		checkGL(glAttachShader(id, fragmentShaderId));

		checkGL(glLinkProgram(id));

		GLint linkStatus;
		checkGL(glGetProgramiv(id, GL_LINK_STATUS, &linkStatus));
		if (!linkStatus) {
			GLint infoLogLength;
			checkGL(glGetProgramiv(id, GL_INFO_LOG_LENGTH, &infoLogLength));
			char* infoLog = new char[infoLogLength + 1];
			checkGL(glGetProgramInfoLog(id, infoLogLength, nullptr, infoLog));
			error("Program info log:\n" + string(infoLog));
			delete[] infoLog;
		}

		checkGL(glDetachShader(id, vertexShaderId));
		checkGL(glDetachShader(id, fragmentShaderId));

		checkGL(glDeleteShader(vertexShaderId));
		checkGL(glDeleteShader(fragmentShaderId));
	}

	string Shader::shaderSourceFromFile(const string& filename) const
	{
		std::ifstream ifs(filename);

		if (!ifs) {
			error("File \"" + filename + "\" could not be opened");
			return "";
		}

		return string((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
	}

	GLuint Shader::shaderFromSource(GLenum type, const string& source) const
	{
		GLuint id = checkGLValue(glCreateShader(type));

		const char* sourceCstr = source.c_str();
		checkGL(glShaderSource(id, 1, &sourceCstr, nullptr));
		checkGL(glCompileShader(id));

		GLint compileStatus;
		checkGL(glGetShaderiv(id, GL_COMPILE_STATUS, &compileStatus));
		if (!compileStatus) {
			GLint infoLogLength;
			checkGL(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infoLogLength));
			char* infoLog = new char[infoLogLength + 1];
			checkGL(glGetShaderInfoLog(id, infoLogLength, nullptr, infoLog));
			error("Shader info log:\n" + string(infoLog));
			delete[] infoLog;
		}

		return id;
	}

	void Shader::use() const
	{
		checkGL(glUseProgram(id));
	}

	void Shader::setUniform(const string& name, float value) const
	{
		checkGL(glUniform1f(getUniformLocation(name), value));
	}

	void Shader::setUniform(const string& name, const glm::vec2& value) const
	{
		checkGL(glUniform2f(getUniformLocation(name), value.x, value.y));
	}

	void Shader::setUniform(const string& name, const glm::vec3& value) const
	{
		checkGL(glUniform3f(getUniformLocation(name), value.x, value.y, value.z));
	}

	void Shader::setUniform(const string& name, const glm::vec4& value) const
	{
		checkGL(glUniform4f(getUniformLocation(name), value.x, value.y, value.z, value.w));
	}

	void Shader::setUniform(const string& name, int value) const
	{
		checkGL(glUniform1i(getUniformLocation(name), value));
	}

	void Shader::setUniform(const string& name, const glm::ivec2& value) const
	{
		checkGL(glUniform2i(getUniformLocation(name), value.x, value.y));
	}

	void Shader::setUniform(const string& name, const glm::ivec3& value) const
	{
		checkGL(glUniform3i(getUniformLocation(name), value.x, value.y, value.z));
	}

	void Shader::setUniform(const string& name, const glm::ivec4& value) const
	{
		checkGL(glUniform4i(getUniformLocation(name), value.x, value.y, value.z, value.w));
	}

	void Shader::setUniform(const string& name, uInt value) const
	{
		checkGL(glUniform1ui(getUniformLocation(name), value));
	}

	void Shader::setUniform(const string& name, const glm::uvec2& value) const
	{
		checkGL(glUniform2ui(getUniformLocation(name), value.x, value.y));
	}

	void Shader::setUniform(const string& name, const glm::uvec3& value) const
	{
		checkGL(glUniform3ui(getUniformLocation(name), value.x, value.y, value.z));
	}

	void Shader::setUniform(const string& name, const glm::uvec4& value) const
	{
		checkGL(glUniform4ui(getUniformLocation(name), value.x, value.y, value.z, value.w));
	}

	void Shader::setUniform(const string& name, const glm::mat2& value, bool transpose) const
	{
		checkGL(glUniformMatrix2fv(getUniformLocation(name), 1, transpose, glm::value_ptr(value)));
	}

	void Shader::setUniform(const string& name, const glm::mat3& value, bool transpose) const
	{
		checkGL(glUniformMatrix3fv(getUniformLocation(name), 1, transpose, glm::value_ptr(value)));
	}

	void Shader::setUniform(const string& name, const glm::mat4& value, bool transpose) const
	{
		checkGL(glUniformMatrix4fv(getUniformLocation(name), 1, transpose, &value[0][0]));
	}

	GLint Shader::getUniformLocation(const string& name) const
	{
		return glGetUniformLocation(id, name.c_str());
	}
}
