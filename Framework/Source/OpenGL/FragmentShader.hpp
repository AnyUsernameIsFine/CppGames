#pragma once

#include "Shader.hpp"

namespace Framework
{
	class FragmentShader : public GLShader
	{
	public:
		FragmentShader(const string& source, bool fromFile = true);
	};
}
