#pragma once

#include "System\Globals.hpp"

#include <SDL.h>

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
