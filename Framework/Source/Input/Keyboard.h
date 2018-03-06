#pragma once

#include <SDL_keyboard.h>
#include "System\Globals.h"

namespace Framework
{
	class Keyboard
	{
	public:
		~Keyboard();
		bool key(SDL_Keycode key) const;

	private:
		const UInt8* keyboardState;
		int numKeys;
		UInt8* fixedKeyboardState;

		Keyboard();
		void update();

		friend class Input;
	};
}
