#pragma once

#include "Shader.hpp"

namespace Framework
{
	class VertexShader : public GLShader
	{
	public:
		VertexShader(const string& source, bool fromFile = true);
	};
}
