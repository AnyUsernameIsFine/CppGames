#include "FragmentShader.hpp"

namespace Framework
{
	FragmentShader::FragmentShader(const string& filename, bool fromFile) :
		GLShader(GL_FRAGMENT_SHADER, filename, fromFile)
	{
		//
	}
}
