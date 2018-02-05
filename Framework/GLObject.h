#pragma once

#include <GL\glew.h>

namespace Framework
{
	class GLObject abstract
	{
	public:
		const GLuint getId() const;

	protected:
		GLuint id_;
	};
}
