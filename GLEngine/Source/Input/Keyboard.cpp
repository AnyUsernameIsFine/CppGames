#include "Keyboard.h"

namespace GLEngine
{
	Keyboard::~Keyboard()
	{
		delete[] fixedKeyboardState;
	}

	bool Keyboard::key(SDL_Keycode key) const
	{
		return fixedKeyboardState[checkSDLValue(SDL_GetScancodeFromKey(key))];
	}

	Keyboard::Keyboard()
	{
		keyboardState = checkSDLValue(SDL_GetKeyboardState(&numKeys));

		fixedKeyboardState = new UInt8[numKeys];
	}

	void Keyboard::update()
	{
		memcpy(fixedKeyboardState, keyboardState, numKeys * sizeof(keyboardState[0]));
	}
}
