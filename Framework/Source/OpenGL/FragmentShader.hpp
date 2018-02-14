#pragma once

#include "Shader.hpp"

#include <string>

namespace Framework
{
	class FragmentShader : public GLShader
	{
	public:
		FragmentShader(const std::string& source, bool fromFile = true);
	};
}
