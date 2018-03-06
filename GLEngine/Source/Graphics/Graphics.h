#pragma once

#include <SDL_events.h>
#include "System\TimePoint.h"
#include "Text.h"
#include "Window.h"

namespace GLEngine
{
	class Graphics
	{
	public:
		void clearScreen(float r = 0.0f, float g = 0.0f, float b = 0.0f, bool depth = false) const;
		float getFps() const;

		Window window;
		Text text;

	private:
		static constexpr int FPS_BUFFER_SIZE = 60;

		Graphics() {}
		bool createWindow();
		bool initialize();
		void update();
		void windowEventHandler(const SDL_WindowEvent& event);

		float frameLengths[FPS_BUFFER_SIZE];
		float frameLengthsTotal = 0.0f;
		int frameLengthsIndex = 0;
		int numFrameLengths = 0;
		TimePoint frameTimePoint;

		friend class Game;
	};
}
