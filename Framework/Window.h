#pragma once

#include "Input.h"

#include <SDL.h>

#include <string>

namespace Framework
{
	class Window
	{
		const std::string TITLE = "New window";
		const int WIDTH = 640;
		const int HEIGHT = 480;
		const bool SHOW_CURSOR = true;
		const bool ENABLE_VSYNC = false;

	public:
		void setTitle(const std::string& title);
		void setSize(int width, int height);
		void showCursor(bool show = true);
		void enableVSync(bool enable = true);
		const int getWidth() const;
		const int getHeight() const;

	private:
		friend class Graphics;
		friend class Game;

		SDL_Window* window_;
		std::string title_ = TITLE;
		int width_ = WIDTH;
		int height_ = HEIGHT;
		bool showCursor_ = SHOW_CURSOR;
		bool enableVSync_ = ENABLE_VSYNC;

		bool open_();
		void close_();
		bool pollEvents_(Input& input);
		bool activateOpenGL_();
		void update_();
	};
}
