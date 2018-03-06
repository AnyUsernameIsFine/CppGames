#include "OpenGLObject.h"

#include <SDL_video.h>

namespace Framework
{
	GLuint OpenGLObject::getId() const
	{
		return id;
	}

	bool OpenGLObject::hasContext() const
	{
		return SDL_GL_GetCurrentContext();
	}

	bool OpenGLObject::hasContext(const std::string& message) const
	{
		if (!hasContext()) {
			error(message + ": No active OpenGL context");
			return false;
		}

		return true;
	}
}
