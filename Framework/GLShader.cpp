#include "GLShader.h"
#include "Error.h"

#include <fstream>

namespace Framework
{
	GLShader::GLShader(GLenum type, const std::string& filename)
	{
		std::ifstream ifs(filename);

		if (!ifs) {
			error("File \"" + filename + "\" could not be opened");
		}
		else {
			std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
			const char* source = content.c_str();

			id_ = glCheckV(glCreateShader(type));
			glCheck(glShaderSource(id_, 1, &source, nullptr));
			glCheck(glCompileShader(id_));

			GLint compileStatus;
			glCheck(glGetShaderiv(id_, GL_COMPILE_STATUS, &compileStatus));
			if (!compileStatus) {
				GLint infoLogLength;
				glCheck(glGetShaderiv(id_, GL_INFO_LOG_LENGTH, &infoLogLength));
				char* infoLog = new char[infoLogLength + 1];
				glCheck(glGetShaderInfoLog(id_, infoLogLength, nullptr, infoLog));

				error("Shader info log:\n" + std::string(infoLog));
			}
		}
	}

	GLShader::~GLShader()
	{
		glCheck(glDeleteShader(id_));
	}
}
