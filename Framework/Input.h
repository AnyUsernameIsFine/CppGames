#pragma once

#include <SDL.h>

namespace Framework
{
	class Input
	{
	public:
		bool isKeyDown(SDL_Keycode key);

	private:
		friend class Window;

		const Uint8* keyboardState_;

		void processEvent_(SDL_Event event);
	};
}
