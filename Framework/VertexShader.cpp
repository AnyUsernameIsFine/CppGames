#include "VertexShader.h"
#include "Error.h"

namespace Framework
{
	VertexShader::VertexShader(const std::string& filename) :
		GLShader(GL_VERTEX_SHADER, filename)
	{
		//
	}
}
