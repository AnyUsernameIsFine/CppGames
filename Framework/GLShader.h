#pragma once

#include "GLObject.h"

#include <string>

namespace Framework
{
	class GLShader : public GLObject
	{
	public:
		GLShader(GLenum type, const std::string& source);
		~GLShader();
	};
}
