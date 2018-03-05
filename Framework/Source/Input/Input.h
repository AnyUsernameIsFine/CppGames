#pragma once

#include "Keyboard.h"
#include "Mouse.h"
#include "Controller.h"

namespace Framework
{
	class Input
	{
	public:
		Keyboard keyboard;
		Mouse mouse;
		Controller controller;

	private:
		void processEvent(const SDL_Event& event);
		void update();

		friend class Game;
	};
}
