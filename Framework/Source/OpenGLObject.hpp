#pragma once

#include <GL\glew.h>

namespace Framework
{
	class OpenGLObject abstract
	{
	public:
		const GLuint getId() const;

	protected:
		GLuint id_;
	};
}
