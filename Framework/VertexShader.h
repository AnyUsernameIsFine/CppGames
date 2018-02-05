#pragma once

#include "GLShader.h"

#include <string>

namespace Framework
{
	class VertexShader : public GLShader
	{
	public:
		VertexShader(const std::string& source);
	};
}
