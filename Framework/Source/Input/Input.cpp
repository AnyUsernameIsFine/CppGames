#include "Input.hpp"

namespace Framework
{
	bool Input::isKeyDown(SDL_Keycode key) const
	{
		return keyboardState[SDL_GetScancodeFromKey(key)];
	}

	bool Input::isKeyUp(SDL_Keycode key) const
	{
		return !keyboardState[SDL_GetScancodeFromKey(key)];
	}

	int Input::getMouseDeltaX() const
	{
		return mouseDeltaX;
	}

	int Input::getMouseDeltaY() const
	{
		return mouseDeltaY;
	}

	int Input::getMouseWheel() const
	{
		return mouseWheel;
	}

	void Input::processEvent(SDL_Event event)
	{
		switch (event.type) {
		case SDL_MOUSEMOTION:
			int x, y;
			SDL_GetRelativeMouseState(&x, &y);
			mouseDeltaX += x;
			mouseDeltaY += y;
			break;

		case SDL_MOUSEWHEEL:
			mouseWheel += event.wheel.y;
			break;
		}
	}

	void Input::clear()
	{
		mouseDeltaX = 0;
		mouseDeltaY = 0;
		mouseWheel = 0;
	}
}
