#pragma once

#include "System\Globals.hpp"
#include "Input\Input.hpp"

#include <SDL.h>

namespace Framework
{
	class Window
	{
	public:
		void setTitle(const string& title);
		void setSize(int width, int height);
		void enableFullscreen(bool enable = true);
		void enableResizing(bool enable = true);
		void enableCursor(bool enable = true);
		void enableVSync(bool enable = true);
		void enableAntiAliasing(bool enable = true);
		int getWidth() const;
		int getHeight() const;

	private:
		SDL_Window* window;
		SDL_DisplayMode closestDisplayMode;
		string title = "New window";
		int width = 640;
		int height = 480;
		bool resizing = false;
		bool vSync = true;
		bool cursor = true;
		bool fullscreen = false;
		bool antiAliasing = false;
		bool isHidden = true;
		bool hasResized = false;
		int resizedWidth;
		int resizedHeight;

		Window();
		int open();
		void close();
		int activateOpenGL();
		void update();
		void onResize(int width, int height);
		void applySize();

		friend class Graphics;
	};
}
