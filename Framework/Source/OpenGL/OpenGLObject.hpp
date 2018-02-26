#pragma once

#include <GL\glew.h>

namespace Framework
{
	class OpenGLObject abstract
	{
	public:
		GLuint getId() const;

	protected:
		GLuint id = 0;
	};
}
