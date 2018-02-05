#include "Graphics.h"
#include "Error.h"

namespace Framework
{
	void Graphics::clearScreen(float r, float g, float b, bool depth)
	{
		glCheck(glClearColor(r, g, b, 1.0f));
		glCheck(glClear(GL_COLOR_BUFFER_BIT | (depth ? GL_DEPTH_BUFFER_BIT : 0)));
	}

	bool Graphics::initialize_()
	{
		if (!window.activateOpenGL_()) {
			return false;
		}

		text.initialize_("text.vert", "text.frag", window.getWidth(), window.getHeight());

		return true;
	}
}
