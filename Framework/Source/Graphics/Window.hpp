#pragma once

#include "Input\Input.hpp"

#include <SDL.h>

#include <string>

namespace Framework
{
	class Window
	{
		const std::string TITLE = "New window";
		const int WIDTH = 640;
		const int HEIGHT = 480;
		const bool HIDE_CURSOR = false;
		const bool ENABLE_VSYNC = false;
		const bool SET_FULLSCREEN = false;

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
		std::string title_ = TITLE;
		int width_ = WIDTH;
		int height_ = HEIGHT;
		bool hideCursor_ = HIDE_CURSOR;
		bool enableVSync_ = ENABLE_VSYNC;
		bool setFullscreen_ = SET_FULLSCREEN;
		bool hidden_ = true;

		int open_();
		void close_();
		int activateOpenGL_();
		void update_();
	};
}
