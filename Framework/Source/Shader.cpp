#include "Shader.hpp"
#include "Error.hpp"

#include <fstream>
#include <cstdio>

namespace Framework
{
	GLShader::GLShader(GLenum type, const std::string& source, bool fromFile)
	{
		if (fromFile) {
			std::ifstream ifs(source);

			if (!ifs) {
				error("File \"" + source + "\" could not be opened");
			}
			else {
				std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
				create_(type, content);
			}
		}
		else {
			create_(type, source);
		}
	}

	GLShader::~GLShader()
	{
		glCheck(glDeleteShader(id_));
	}

	void GLShader::create_(GLenum type, const std::string& source)
	{
		id_ = glCheckV(glCreateShader(type));

		const char* sourceCstr = source.c_str();
		glCheck(glShaderSource(id_, 1, &sourceCstr, nullptr));
		glCheck(glCompileShader(id_));

		GLint compileStatus;
		glCheck(glGetShaderiv(id_, GL_COMPILE_STATUS, &compileStatus));
		if (!compileStatus) {
			GLint infoLogLength;
			glCheck(glGetShaderiv(id_, GL_INFO_LOG_LENGTH, &infoLogLength));
			char* infoLog = new char[infoLogLength + 1];
			glCheck(glGetShaderInfoLog(id_, infoLogLength, nullptr, infoLog));
			error("Shader info log:\n" + std::string(infoLog));
			delete[] infoLog;
		}
	}
}
