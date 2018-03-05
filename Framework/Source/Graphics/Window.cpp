#include "Window.h"

#include <SDL.h>

namespace Framework
{
	Window::Window()
	{
		checkSDL(SDL_InitSubSystem(SDL_INIT_VIDEO));

		checkSDL(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3));
		checkSDL(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3));

		checkSDL(SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE));
	}

	Window::~Window()
	{
		checkSDL(SDL_DestroyWindow(window));
		checkSDL(SDL_QuitSubSystem(SDL_INIT_VIDEO));
	}

	void Window::setTitle(const string& title)
	{
		if (title != this->title) {
			this->title = title;

			if (window) {
				checkSDL(SDL_SetWindowTitle(window, title.c_str()));
			}
		}
	}

	void Window::setSize(int width, int height)
	{
		if (width != this->width || height != this->height) {
			this->width = width;
			this->height = height;

			checkSDL(SDL_GetDesktopDisplayMode(0, &closestDisplayMode));
			closestDisplayMode.w = width;
			closestDisplayMode.h = height;
			checkSDL(SDL_GetClosestDisplayMode(0, &closestDisplayMode, &closestDisplayMode));

			if (window) {
				applySize();
			}
		}
	}

	void Window::enableFullscreen(bool enable)
	{
		if (enable != fullscreen) {
			fullscreen = enable;

			if (window) {
				checkSDL(SDL_SetWindowFullscreen(window, enable ? SDL_WINDOW_FULLSCREEN : 0));
				applySize();
			}
		}
	}

	void Window::enableResizing(bool enable)
	{
		if (enable != resizing) {
			resizing = enable;

			if (window) {
				checkSDL(SDL_SetWindowResizable(window, enable ? SDL_TRUE : SDL_FALSE));
			}
		}
	}

	void Window::enableCursor(bool enable)
	{
		if (enable != cursor) {
			cursor = enable;

			if (window) {
				checkSDL(SDL_SetRelativeMouseMode(enable ? SDL_FALSE : SDL_TRUE));
			}
		}
	}

	void Window::enableVSync(bool enable)
	{
		if (enable != vSync) {
			vSync = enable;

			if (window) {
				checkSDL(SDL_GL_SetSwapInterval(enable ? 1 : 0));
			}
		}
	}

	void Window::enableAntiAliasing(bool enable)
	{
		if (enable != antiAliasing) {
			if (window) {
				error("Can't change anti-aliasing setting after window has been created");
			}
			else {
				antiAliasing = enable;
			}
		}
	}

	int Window::getWidth() const
	{
		return fullscreen ? closestDisplayMode.w : width;
	}

	int Window::getHeight() const
	{
		return fullscreen ? closestDisplayMode.h : height;
	}

	bool Window::create()
	{
		if (antiAliasing) {
			checkSDL(SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1));
			checkSDL(SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4));
		}

		window = checkSDLValue(SDL_CreateWindow(
			title.c_str(),
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			width,
			height,
			SDL_WINDOW_HIDDEN | SDL_WINDOW_OPENGL | (resizing ? SDL_WINDOW_RESIZABLE : 0)
		));

		if (window == nullptr) {
			error("Could not create window");
			return false;
		}

		if (fullscreen && checkSDLValue(SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN)) < 0) {
			error("Could not enable fullscreen mode");
			return false;
		}

		if (!cursor && checkSDLValue(SDL_SetRelativeMouseMode(SDL_TRUE)) < 0) {
			error("Could not set relative mouse mode");
			return false;
		}

		return true;
	}

	bool Window::activateOpenGL()
	{
		if (checkSDLValue(SDL_GL_CreateContext(window)) == nullptr) {
			error("Could not create OpenGL context");
			return false;
		}

		if (checkSDLValue(SDL_GL_SetSwapInterval(vSync ? 1 : 0)) < 0) {
			error("Could not set swap interval");
			return false;
		}

		GLenum e;
		if ((e = glewInit()) != GLEW_OK) {
			error("Could not initialize GLEW: " + string((const char*)glewGetErrorString(e)));
			return false;
		}

		return true;
	}

	void Window::update()
	{
		if (isHidden) {
			isHidden = false;

			checkSDL(SDL_ShowWindow(window));
			checkSDL(SDL_RaiseWindow(window));

			if (antiAliasing) {
				checkGL(glEnable(GL_MULTISAMPLE));
			}
		}

		if (hasResized) {
			hasResized = false;

			if (!fullscreen) {
				width = resizedWidth;
				height = resizedHeight;
			}

			checkGL(glViewport(0, 0, resizedWidth, resizedHeight));
		}

		checkSDL(SDL_GL_SwapWindow(window));
	}

	void Window::resizedEventHandler(int width, int height)
	{
		hasResized = true;
		resizedWidth = width;
		resizedHeight = height;
	}

	void Window::applySize()
	{
		if (fullscreen) {
			checkSDL(SDL_SetWindowDisplayMode(window, &closestDisplayMode));
			checkGL(glViewport(0, 0, closestDisplayMode.w, closestDisplayMode.h));
		}
		else {
			checkSDL(SDL_SetWindowSize(window, width, height));
			checkGL(glViewport(0, 0, width, height));
		}
	}
}
