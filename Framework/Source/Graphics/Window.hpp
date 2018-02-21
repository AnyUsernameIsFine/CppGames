#pragma once

#include "Input\Input.hpp"

#include <SDL.h>

#include <string>

namespace Framework
{
	class Window
	{
	public:
		void setTitle(const std::string& title);
		void setSize(int width, int height);
		void hideCursor(bool hide = true);
		void enableVSync(bool enable = true);
		void setFullscreen(bool set = true);
		int getWidth() const;
		int getHeight() const;

	private:
		friend class Graphics;

		SDL_Window* window_;
		std::string title_ = "New window";
		int width_ = 640;
		int height_ = 480;
		bool hideCursor_ = false;
		bool enableVSync_ = false;
		bool setFullscreen_ = false;
		bool hidden_ = true;

		int open_();
		void close_();
		int activateOpenGL_();
		void update_();
	};
}
