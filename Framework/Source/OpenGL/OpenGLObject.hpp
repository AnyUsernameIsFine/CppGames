#pragma once

#include "System\Globals.hpp"

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
