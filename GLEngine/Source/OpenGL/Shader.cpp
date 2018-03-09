#include "Shader.h"

#include <fstream>

#include <glm\gtc\type_ptr.hpp>

namespace GLEngine
{
	Shader::~Shader()
	{
		if (hasContext()) {
			checkGL(glDeleteShader(id));
		};
	}

	void Shader::createFromFiles(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename)
	{
		if (!isCreatable()) {
			return;
		}

		std::string vertexShaderSource = shaderSourceFromFile(vertexShaderFilename);
		std::string fragmentShaderSource = shaderSourceFromFile(fragmentShaderFilename);

		createFromSource(vertexShaderSource, fragmentShaderSource);
	}

	void Shader::createComputeFromFile(const std::string& computeShaderFilename)
	{
		if (!isCreatable()) {
			return;
		}

		std::string computeShaderSource = shaderSourceFromFile(computeShaderFilename);

		createComputeFromSource(computeShaderSource);
	}

	void Shader::createFromSource(const std::string& vertexShaderSource, const std::string& fragmentShaderSource)
	{
		if (!isCreatable()) {
			return;
		}

		GLuint vertexShaderId = shaderFromSource(GL_VERTEX_SHADER, vertexShaderSource);
		GLuint fragmentShaderId = shaderFromSource(GL_FRAGMENT_SHADER, fragmentShaderSource);

		create({ vertexShaderId, fragmentShaderId });
	}

	void Shader::createComputeFromSource(const std::string& computeShaderSource)
	{
		if (!isCreatable()) {
			return;
		}

		GLuint computeShaderId = shaderFromSource(GL_COMPUTE_SHADER, computeShaderSource);

		create({ computeShaderId });
	}

	void Shader::use() const
	{
		checkGL(glUseProgram(id));
	}

	void Shader::compute(int x, int y, int z) const
	{
		checkGL(glUseProgram(id));
		checkGL(glDispatchCompute(x, y, z));
	}

	void Shader::setUniform(const std::string& name, float value) const
	{
		checkGL(glUniform1f(getUniformLocation(name), value));
	}

	void Shader::setUniform(const std::string& name, const glm::vec2& value) const
	{
		checkGL(glUniform2f(getUniformLocation(name), value.x, value.y));
	}

	void Shader::setUniform(const std::string& name, const glm::vec3& value) const
	{
		checkGL(glUniform3f(getUniformLocation(name), value.x, value.y, value.z));
	}

	void Shader::setUniform(const std::string& name, const glm::vec4& value) const
	{
		checkGL(glUniform4f(getUniformLocation(name), value.x, value.y, value.z, value.w));
	}

	void Shader::setUniform(const std::string& name, int value) const
	{
		checkGL(glUniform1i(getUniformLocation(name), value));
	}

	void Shader::setUniform(const std::string& name, const glm::ivec2& value) const
	{
		checkGL(glUniform2i(getUniformLocation(name), value.x, value.y));
	}

	void Shader::setUniform(const std::string& name, const glm::ivec3& value) const
	{
		checkGL(glUniform3i(getUniformLocation(name), value.x, value.y, value.z));
	}

	void Shader::setUniform(const std::string& name, const glm::ivec4& value) const
	{
		checkGL(glUniform4i(getUniformLocation(name), value.x, value.y, value.z, value.w));
	}

	void Shader::setUniform(const std::string& name, UInt value) const
	{
		checkGL(glUniform1ui(getUniformLocation(name), value));
	}

	void Shader::setUniform(const std::string& name, const glm::uvec2& value) const
	{
		checkGL(glUniform2ui(getUniformLocation(name), value.x, value.y));
	}

	void Shader::setUniform(const std::string& name, const glm::uvec3& value) const
	{
		checkGL(glUniform3ui(getUniformLocation(name), value.x, value.y, value.z));
	}

	void Shader::setUniform(const std::string& name, const glm::uvec4& value) const
	{
		checkGL(glUniform4ui(getUniformLocation(name), value.x, value.y, value.z, value.w));
	}

	void Shader::setUniform(const std::string& name, const glm::mat2& value, bool transpose) const
	{
		checkGL(glUniformMatrix2fv(getUniformLocation(name), 1, transpose, glm::value_ptr(value)));
	}

	void Shader::setUniform(const std::string& name, const glm::mat3& value, bool transpose) const
	{
		checkGL(glUniformMatrix3fv(getUniformLocation(name), 1, transpose, glm::value_ptr(value)));
	}

	void Shader::setUniform(const std::string& name, const glm::mat4& value, bool transpose) const
	{
		checkGL(glUniformMatrix4fv(getUniformLocation(name), 1, transpose, &value[0][0]));
	}

	GLint Shader::getUniformLocation(const std::string& name) const
	{
		return glGetUniformLocation(id, name.c_str());
	}

	bool Shader::isCreatable() const
	{
		if (id) {
			error("Shader has already been created");
			return false;
		}

		if (!hasContext("Could not create shader")) {
			return false;
		}

		return true;
	}

	std::string Shader::shaderSourceFromFile(const std::string& filename) const
	{
		std::ifstream ifs(filename);

		if (!ifs) {
			error("File \"" + filename + "\" could not be opened");
			return "";
		}

		return std::string(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());
	}

	GLuint Shader::shaderFromSource(GLenum type, const std::string& source) const
	{
		GLuint shaderId = checkGLValue(glCreateShader(type));

		const char* sourceCstr = source.c_str();
		checkGL(glShaderSource(shaderId, 1, &sourceCstr, nullptr));
		checkGL(glCompileShader(shaderId));

		GLint compileStatus;
		checkGL(glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compileStatus));

		if (compileStatus == GL_FALSE) {
			GLint infoLogLength;
			checkGL(glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLogLength));
			infoLogLength -= 2;

			std::vector<GLchar> infoLog(infoLogLength);
			checkGL(glGetShaderInfoLog(shaderId, infoLogLength, nullptr, &infoLog[0]));

			error(&infoLog[0]);
		}

		return shaderId;
	}

	void Shader::create(const std::initializer_list<GLuint>& shaderIds)
	{
		if (!isCreatable()) {
			return;
		}

		id = checkGLValue(glCreateProgram());

		for (const auto& shaderId : shaderIds) {
			checkGL(glAttachShader(id, shaderId));
		}

		checkGL(glLinkProgram(id));

		GLint linkStatus;
		checkGL(glGetProgramiv(id, GL_LINK_STATUS, &linkStatus));

		if (linkStatus == GL_FALSE) {
			GLint infoLogLength;
			checkGL(glGetProgramiv(id, GL_INFO_LOG_LENGTH, &infoLogLength));
			infoLogLength -= 2;

			std::vector<GLchar> infoLog(infoLogLength);
			checkGL(glGetProgramInfoLog(id, infoLogLength, nullptr, &infoLog[0]));

			error(&infoLog[0]);
		}

		for (const auto& shaderId : shaderIds) {
			checkGL(glDetachShader(id, shaderId));
			checkGL(glDeleteShader(shaderId));
		}
	}
}
