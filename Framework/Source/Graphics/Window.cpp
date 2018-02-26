#include "Window.hpp"
#include "System\Error.hpp"

#include <GL\glew.h>

namespace Framework
{
	Window::Window()
	{
		sdlCheck(SDL_Init(SDL_INIT_VIDEO));

		sdlCheck(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3));
		sdlCheck(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3));
	}

	void Window::setTitle(const string& title)
	{
		if (title != this->title) {
			this->title = title;

			if (window) {
				sdlCheck(SDL_SetWindowTitle(window, title.c_str()));
			}
		}
	}

	void Window::setSize(int width, int height)
	{
		if (width != this->width || height != this->height) {
			this->width = width;
			this->height = height;

			sdlCheck(SDL_GetDesktopDisplayMode(0, &closestDisplayMode));
			closestDisplayMode.w = width;
			closestDisplayMode.h = height;
			sdlCheck(SDL_GetClosestDisplayMode(0, &closestDisplayMode, &closestDisplayMode));

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
				sdlCheck(SDL_SetWindowFullscreen(window, enable ? SDL_WINDOW_FULLSCREEN : 0));
				applySize();
			}
		}
	}

	void Window::enableResizing(bool enable)
	{
		if (enable != resizing) {
			resizing = enable;

			if (window) {
				sdlCheck(SDL_SetWindowResizable(window, enable ? SDL_TRUE : SDL_FALSE));
			}
		}
	}

	void Window::enableCursor(bool enable)
	{
		if (enable != cursor) {
			cursor = enable;

			if (window) {
				sdlCheck(SDL_SetRelativeMouseMode(enable ? SDL_FALSE : SDL_TRUE));
			}
		}
	}

	void Window::enableVSync(bool enable)
	{
		if (enable != vSync) {
			vSync = enable;

			if (window) {
				sdlCheck(SDL_GL_SetSwapInterval(enable ? 1 : 0));
			}
		}
	}

	void Window::enableAntiAliasing(bool enable)
	{
		if (enable != antiAliasing) {
			if (!window) {
				antiAliasing = enable;
			}
			else {
				error("Can't change anti-aliasing setting after window has been created");
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

	int Window::open()
	{
		if (antiAliasing) {
			sdlCheck(SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1));
			sdlCheck(SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4));
		}

		window = sdlCheckV(SDL_CreateWindow(
			title.c_str(),
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			width,
			height,
			SDL_WINDOW_HIDDEN | SDL_WINDOW_OPENGL | (resizing ? SDL_WINDOW_RESIZABLE : 0)
		));

		if (window == NULL) {
			return 1;
		}

		if (fullscreen) {
			SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
		}

		if (!cursor && sdlCheckV(SDL_SetRelativeMouseMode(SDL_TRUE)) < 0) {
			return 1;
		}

		return 0;
	}

	void Window::close()
	{
		sdlCheck(SDL_DestroyWindow(window));
		sdlCheck(SDL_Quit());
	}

	int Window::activateOpenGL()
	{
		if (sdlCheckV(SDL_GL_CreateContext(window)) == NULL) {
			return 1;
		}

		if (sdlCheckV(SDL_GL_SetSwapInterval(vSync ? 1 : 0)) != 0) {
			return 1;
		}

		GLenum e;
		if ((e = glewInit()) != GLEW_OK) {
			error("glewInit failed: " + string((const char*)glewGetErrorString(e)));
			return 1;
		}

		return 0;
	}

	void Window::update()
	{
		if (isHidden) {
			isHidden = false;

			sdlCheck(SDL_ShowWindow(window));
			sdlCheck(SDL_RaiseWindow(window));

			if (antiAliasing) {
				glCheck(glEnable(GL_MULTISAMPLE));
			}
		}

		if (hasResized) {
			hasResized = false;

			if (!fullscreen) {
				width = resizedWidth;
				height = resizedHeight;
			}

			glCheck(glViewport(0, 0, resizedWidth, resizedHeight));
		}

		sdlCheck(SDL_GL_SwapWindow(window));
	}

	void Window::onResize(int width, int height)
	{
		hasResized = true;

		resizedWidth = width;
		resizedHeight = height;
	}

	void Window::applySize()
	{
		if (fullscreen) {
			sdlCheck(SDL_SetWindowDisplayMode(window, &closestDisplayMode));
			glCheck(glViewport(0, 0, closestDisplayMode.w, closestDisplayMode.h));
		}
		else {
			sdlCheck(SDL_SetWindowSize(window, width, height));
			glCheck(glViewport(0, 0, width, height));
		}
	}
}
