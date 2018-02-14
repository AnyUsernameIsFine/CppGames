#include "FragmentShader.hpp"
#include "System\Error.hpp"

namespace Framework
{
	FragmentShader::FragmentShader(const std::string& filename, bool fromFile) :
		GLShader(GL_FRAGMENT_SHADER, filename, fromFile)
	{
		//
	}
}
