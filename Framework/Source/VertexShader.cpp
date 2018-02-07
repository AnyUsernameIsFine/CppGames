#include "VertexShader.hpp"
#include "Error.hpp"

namespace Framework
{
	VertexShader::VertexShader(const std::string& filename, bool fromFile) :
		GLShader(GL_VERTEX_SHADER, filename, fromFile)
	{
		//
	}
}
