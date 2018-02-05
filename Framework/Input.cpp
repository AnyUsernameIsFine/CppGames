#include "Input.h"
#include "Error.h"

namespace Framework
{
	bool Input::isKeyDown(SDL_Keycode key)
	{
		return keyboardState_[SDL_GetScancodeFromKey(key)];
	}

	void Input::processEvent_(SDL_Event event)
	{
		keyboardState_ = SDL_GetKeyboardState(nullptr);
	}
}
