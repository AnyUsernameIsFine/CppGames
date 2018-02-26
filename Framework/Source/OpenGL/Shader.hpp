#pragma once

#include "System\Globals.hpp"
#include "OpenGLObject.hpp"

namespace Framework
{
	class GLShader : public OpenGLObject
	{
	public:
		GLShader(GLenum type, const string& source, bool fromFile = true);
		~GLShader();

	private:
		void create_(GLenum type, const string& source);
	};
}
