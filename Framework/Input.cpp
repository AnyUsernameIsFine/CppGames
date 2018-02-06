#include "Input.h"
#include "Error.h"

namespace Framework
{
	Input::Input()
	{
		keyboardState_ = SDL_GetKeyboardState(nullptr);
	}

	const bool Input::isKeyDown(SDL_Keycode key) const
	{
		return keyboardState_[SDL_GetScancodeFromKey(key)];
	}

	void Input::processEvent_(SDL_Event event)
	{
		switch (event.type) {
		case SDL_KEYDOWN:
		case SDL_KEYUP:
			keyboardState_ = SDL_GetKeyboardState(nullptr);
			break;
		}
	}
}
