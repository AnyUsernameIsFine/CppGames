#include "FragmentShader.hpp"
#include "Error.hpp"

namespace Framework
{
	FragmentShader::FragmentShader(const std::string& filename, bool fromFile) :
		GLShader(GL_FRAGMENT_SHADER, filename, fromFile)
	{
		//
	}
}
