#pragma once

#include "System\TimePoint.h"
#include "Window.h"
#include "Text.h"

#include <SDL_events.h>

namespace Framework
{
	class Graphics
	{
	public:
		Window window;
		Text text;

		void clearScreen(float r = 0, float g = 0, float b = 0, bool depth = false) const;
		float getFps() const;

	private:
		static const int FPS_BUFFER_SIZE = 60;

		float frameLengths[FPS_BUFFER_SIZE];
		float frameLengthsTotal = 0;
		int frameLengthsIndex = 0;
		int numberOfFrameLengths = 0;
		TimePoint frameTimePoint;

		Graphics() {}
		bool createWindow();
		bool initialize();
		void update();
		void windowEventHandler(SDL_WindowEvent event);

		friend class Game;
	};
}
