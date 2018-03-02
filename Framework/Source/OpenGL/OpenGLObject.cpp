#include "OpenGLObject.h"

namespace Framework
{
	GLuint OpenGLObject::getId() const
	{
		return id;
	}

	bool OpenGLObject::hasContext(const string& message) const
	{
		if (sdlCheckValue(!SDL_GL_GetCurrentContext())) {
			error(message + ": no active OpenGL context");
			return false;
		}

		return true;
	}
}
