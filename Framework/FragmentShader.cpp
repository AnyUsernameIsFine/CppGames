#include "FragmentShader.h"
#include "Error.h"

namespace Framework
{
	FragmentShader::FragmentShader(const std::string& filename) :
		GLShader(GL_FRAGMENT_SHADER, filename)
	{
		//
	}
}
