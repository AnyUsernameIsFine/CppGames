#pragma once

#include "Controller.h"
#include "Keyboard.h"
#include "Mouse.h"

namespace GLEngine
{
	class Input
	{
	public:
		Controller controller;
		Keyboard keyboard;
		Mouse mouse;

	private:
		void processEvent(const SDL_Event& event);
		void update();

		friend class Game;
	};
}
