#include "Input.hpp"
#include "Error.hpp"

namespace Framework
{
	//Input::Input()
	//{
	//	keyboardState_ = SDL_GetKeyboardState(nullptr);
	//}

	bool Input::isKeyDown(SDL_Keycode key) const
	{
		return keyboardState_[SDL_GetScancodeFromKey(key)];
	}

	bool Input::isKeyUp(SDL_Keycode key) const
	{
		return !keyboardState_[SDL_GetScancodeFromKey(key)];
	}

	void Input::processEvent_(SDL_Event event)
	{
		//switch (event.type) {
		//case SDL_KEYDOWN:
		//case SDL_KEYUP:
		//	//keyboardState_ = SDL_GetKeyboardState(nullptr);
		//	break;
		//}
	}
}
