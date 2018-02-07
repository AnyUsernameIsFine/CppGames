#pragma once

#include "OpenGLObject.hpp"

#include <string>

namespace Framework
{
	class GLShader : public OpenGLObject
	{
	public:
		GLShader(GLenum type, const std::string& source, bool fromFile = true);
		~GLShader();

	private:
		void create_(GLenum type, const std::string& source);
	};
}
