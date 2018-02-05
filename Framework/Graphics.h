#pragma once

#include "Window.h"
#include "Text.h"

namespace Framework
{
	class Graphics
	{
	public:
		Window window;
		Text text;

		void clearScreen(float r = 0, float g = 0, float b = 0, bool depth = true);

	private:
		friend class Game;

		bool initialize_();
	};
}
