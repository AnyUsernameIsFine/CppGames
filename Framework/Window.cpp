#include "Window.h"
#include "Error.h"

#include <GL\glew.h>

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

	void Window::showCursor(bool show)
	{
		showCursor_ = show;
		sdlCheck(SDL_ShowCursor(show ? SDL_ENABLE : SDL_DISABLE));
	}

	void Window::enableVSync(bool enable)
	{
		enableVSync_ = enable;

		if (window_) {
			sdlCheck(SDL_GL_SetSwapInterval(enable ? 1 : 0));
		}
	}

	const int Window::getWidth() const
	{
		int width;
		sdlCheck(SDL_GL_GetDrawableSize(window_, &width, nullptr));
		return width;
	}

	const int Window::getHeight() const
	{
		int height;
		sdlCheck(SDL_GL_GetDrawableSize(window_, nullptr, &height));
		return height;
	}

	bool Window::open_()
	{
		sdlCheck(SDL_Init(SDL_INIT_VIDEO));
		sdlCheck(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3));
		sdlCheck(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3));

		window_ = sdlCheckV(SDL_CreateWindow(title_.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width_, height_, SDL_WINDOW_OPENGL));
		if (window_ == NULL) {
			return false;
		}

		if (!showCursor_) {
			sdlCheck(SDL_ShowCursor(SDL_DISABLE));
		}

		return true;
	}

	void Window::close_()
	{
		sdlCheck(SDL_DestroyWindow(window_));
		sdlCheck(SDL_Quit());
	}

	bool Window::pollEvents_(Input& input)
	{
		bool close = false;

		SDL_Event event;
		while (sdlCheckV(SDL_PollEvent(&event))) {
			input.processEvent_(event);
				
			if (event.type == SDL_QUIT) {
				close = true;
			}
		}

		return close;
	}

	bool Window::activateOpenGL_()
	{
		if (sdlCheckV(SDL_GL_CreateContext(window_)) == NULL) {
			return false;
		}

		if (sdlCheckV(SDL_GL_SetSwapInterval(enableVSync_ ? 1 : 0)) < 0) {
			return false;
		}

		glewInit();

		return true;
	}

	void Window::update_()
	{
		sdlCheck(SDL_GL_SwapWindow(window_));
	}
}
