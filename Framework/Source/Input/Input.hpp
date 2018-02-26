#pragma once

#include <SDL_events.h>

namespace Framework
{
	class Input
	{
	public:
		bool isKeyDown(SDL_Keycode key) const;
		bool isKeyUp(SDL_Keycode key) const;
		int getMouseDeltaX() const;
		int getMouseDeltaY() const;
		int getMouseWheel() const;

	private:
		const Uint8* keyboardState = SDL_GetKeyboardState(nullptr);
		int mouseDeltaX = 0;
		int mouseDeltaY = 0;
		int mouseWheel = 0;

		Input() {}
		void processEvent(SDL_Event event);
		void clear();

		friend class Game;
	};
}
