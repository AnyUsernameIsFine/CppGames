#pragma once

#include <SDL.h>

namespace Framework
{
	class Input
	{
	public:
		//Input();
		bool isKeyDown(SDL_Keycode key) const;
		bool isKeyUp(SDL_Keycode key) const;

	private:
		friend class Game;

		const Uint8* keyboardState_ = SDL_GetKeyboardState(nullptr);

		void processEvent_(SDL_Event event);
	};
}
