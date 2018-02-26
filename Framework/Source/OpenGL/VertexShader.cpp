#include "VertexShader.hpp"

namespace Framework
{
	VertexShader::VertexShader(const string& filename, bool fromFile) :
		GLShader(GL_VERTEX_SHADER, filename, fromFile)
	{
		//
	}
}
