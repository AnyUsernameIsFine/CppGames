#include "Window.hpp"
#include "System\Error.hpp"

#include <GL\glew.h>

#include <ostream>
#include <sstream>

namespace Framework
{
	void Window::setTitle(const std::string& title)
	{
		title_ = title;

		if (window_) {
			sdlCheck(SDL_SetWindowTitle(window_, title.c_str()));
		}
	}

	void Window::setSize(int width, int height)
	{
		width_ = width;
		height_ = height;

		if (window_) {
			sdlCheck(SDL_SetWindowSize(window_, width, height));
		}
	}

	void Window::hideCursor(bool hide)
	{
		hideCursor_ = hide;

		sdlCheck(SDL_SetRelativeMouseMode(hide ? SDL_TRUE : SDL_FALSE));
	}

	void Window::enableVSync(bool enable)
	{
		enableVSync_ = enable;

		if (window_) {
			sdlCheck(SDL_GL_SetSwapInterval(enable ? 1 : 0));
		}
	}

	void Window::setFullscreen(bool set)
	{
		setFullscreen_ = set;

		if (window_) {
			sdlCheck(SDL_SetWindowFullscreen(window_, set ? SDL_WINDOW_FULLSCREEN : 0));
		}
	}

	int Window::getWidth() const
	{
		int width;
		sdlCheck(SDL_GL_GetDrawableSize(window_, &width, nullptr));

		return width;
	}

	int Window::getHeight() const
	{
		int height;
		sdlCheck(SDL_GL_GetDrawableSize(window_, nullptr, &height));

		return height;
	}

	int Window::open_()
	{
		sdlCheck(SDL_Init(SDL_INIT_VIDEO));
		sdlCheck(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3));
		sdlCheck(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3));

		window_ = sdlCheckV(SDL_CreateWindow(
			title_.c_str(),
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			width_,
			height_,
			SDL_WINDOW_HIDDEN | SDL_WINDOW_OPENGL | (setFullscreen_ ? SDL_WINDOW_FULLSCREEN : 0)
		));

		if (window_ == NULL) {
			return 1;
		}

		if (hideCursor_ && sdlCheckV(SDL_SetRelativeMouseMode(SDL_TRUE)) < 0) {
			return 1;
		}

		return 0;
	}

	void Window::close_()
	{
		sdlCheck(SDL_DestroyWindow(window_));
		sdlCheck(SDL_Quit());
	}

	int Window::activateOpenGL_()
	{
		if (sdlCheckV(SDL_GL_CreateContext(window_)) == NULL) {
			return 1;
		}

		if (sdlCheckV(SDL_GL_SetSwapInterval(enableVSync_ ? 1 : 0)) != 0) {
			return 1;
		}

		GLenum e;
		if ((e = glewInit()) != GLEW_OK) {
			error("glewInit failed: " + std::string((const char*)glewGetErrorString(e)));
			return 1;
		}

		return 0;
	}

	void Window::update_()
	{
		if (hidden_) {
			sdlCheck(SDL_ShowWindow(window_));
			sdlCheck(SDL_RaiseWindow(window_));

			hidden_ = false;
		}

		sdlCheck(SDL_GL_SwapWindow(window_));
	}
}
