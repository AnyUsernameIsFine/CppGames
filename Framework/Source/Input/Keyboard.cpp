#include "Keyboard.h"

namespace Framework
{
	bool Keyboard::key(SDL_Keycode key) const
	{
		return fixedKeyboardState[checkSDLValue(SDL_GetScancodeFromKey(key))];
	}

	Keyboard::Keyboard()
	{
		keyboardState = checkSDLValue(SDL_GetKeyboardState(&numberOfKeys));

		fixedKeyboardState = new Uint8[numberOfKeys];
	}

	Keyboard::~Keyboard()
	{
		delete[] fixedKeyboardState;
	}

	void Keyboard::update()
	{
		memcpy(fixedKeyboardState, keyboardState, numberOfKeys * sizeof(keyboardState[0]));
	}
}
