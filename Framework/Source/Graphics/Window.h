#pragma once

#include <SDL_video.h>
#include "System\Globals.h"

namespace Framework
{
	class Window
	{
	public:
		~Window();
		void setTitle(const std::string& title);
		void setSize(int width, int height);
		void enableFullscreen(bool enable = true);
		void enableResizing(bool enable = true);
		void enableCursor(bool enable = true);
		void enableVSync(bool enable = true);
		void enableAntiAliasing(bool enable = true);
		int getWidth() const;
		int getHeight() const;

	private:
		Window();
		bool create();
		bool activateOpenGL();
		void update();
		void resizedEventHandler(int width, int height);
		void applySize();

		SDL_Window* window;
		SDL_DisplayMode closestDisplayMode;
		std::string title = "New window";
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

		friend class Graphics;
	};
}
