#pragma once

#include "System\Globals.h"

#include <SDL_keyboard.h>

namespace Framework
{
	class Keyboard
	{
	public:
		~Keyboard();
		bool key(SDL_Keycode key) const;

	private:
		const Uint8* keyboardState;
		int numberOfKeys;
		Uint8* fixedKeyboardState;

		Keyboard();
		void update();

		friend class Input;
	};
}
