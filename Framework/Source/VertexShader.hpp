#pragma once

#include "Shader.hpp"

#include <string>

namespace Framework
{
	class VertexShader : public GLShader
	{
	public:
		VertexShader(const std::string& source, bool fromFile = true);
	};
}
