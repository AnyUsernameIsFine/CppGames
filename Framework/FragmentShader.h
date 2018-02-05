#pragma once

#include "GLShader.h"

#include <string>

namespace Framework
{
	class FragmentShader : public GLShader
	{
	public:
		FragmentShader(const std::string& source);
	};
}
