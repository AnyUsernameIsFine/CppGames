#pragma once

#include <SDL.h>

namespace Framework
{
	class Input
	{
	public:
		const bool isKeyDown(SDL_Keycode key) const;

	private:
		friend class Window;

		const Uint8* keyboardState_;

		void processEvent_(SDL_Event event);
	};
}
