#include "Shader.hpp"
#include "System\Error.hpp"

#include <fstream>

namespace Framework
{
	GLShader::GLShader(GLenum type, const string& source, bool fromFile)
	{
		if (fromFile) {
			std::ifstream ifs(source);

			if (!ifs) {
				error("File \"" + source + "\" could not be opened");
			}
			else {
				string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
				create_(type, content);
			}
		}
		else {
			create_(type, source);
		}
	}

	GLShader::~GLShader()
	{
		glCheck(glDeleteShader(id));
	}

	void GLShader::create_(GLenum type, const string& source)
	{
		id = glCheckV(glCreateShader(type));

		const char* sourceCstr = source.c_str();
		glCheck(glShaderSource(id, 1, &sourceCstr, nullptr));
		glCheck(glCompileShader(id));

		GLint compileStatus;
		glCheck(glGetShaderiv(id, GL_COMPILE_STATUS, &compileStatus));
		if (!compileStatus) {
			GLint infoLogLength;
			glCheck(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infoLogLength));
			char* infoLog = new char[infoLogLength + 1];
			glCheck(glGetShaderInfoLog(id, infoLogLength, nullptr, infoLog));
			error("Shader info log:\n" + string(infoLog));
			delete[] infoLog;
		}
	}
}
