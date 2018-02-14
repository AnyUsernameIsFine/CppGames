#include "VertexShader.hpp"
#include "System\Error.hpp"

namespace Framework
{
	VertexShader::VertexShader(const std::string& filename, bool fromFile) :
		GLShader(GL_VERTEX_SHADER, filename, fromFile)
	{
		//
	}
}
