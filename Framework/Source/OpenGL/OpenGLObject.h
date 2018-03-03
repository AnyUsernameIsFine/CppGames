#pragma once

#include "System\Globals.h"

#include <SDL_video.h>

namespace Framework
{
	class OpenGLObject abstract
	{
	public:
		GLuint getId() const;

	protected:
		GLuint id = 0;

		bool hasContext() const;
		bool hasContext(const string& message) const;
	};
}
