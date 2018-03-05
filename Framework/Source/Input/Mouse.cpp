#include "Mouse.h"

namespace Framework
{
	int Mouse::x() const
	{
		return myX;
	}

	int Mouse::y() const
	{
		return myY;
	}

	int Mouse::deltaX() const
	{
		return fixedDeltaX;
	}

	int Mouse::deltaY() const
	{
		return fixedDeltaY;
	}

	int Mouse::wheel() const
	{
		return fixedWheel;
	}

	bool Mouse::left() const
	{
		return buttonState & SDL_BUTTON(SDL_BUTTON_LEFT);
	}

	bool Mouse::middle() const
	{
		return buttonState & SDL_BUTTON(SDL_BUTTON_MIDDLE);
	}

	bool Mouse::right() const
	{
		return buttonState & SDL_BUTTON(SDL_BUTTON_RIGHT);
	}

	void Mouse::movedEventHandler(const SDL_MouseMotionEvent& event)
	{
		myDeltaX += event.xrel;
		myDeltaY += event.yrel;
	}

	void Mouse::wheelEventHandler(const SDL_MouseWheelEvent& event)
	{
		myWheel += event.y;
	}

	void Mouse::update()
	{
		fixedDeltaX = myDeltaX;
		fixedDeltaY = myDeltaY;
		fixedWheel = myWheel;

		myDeltaX = 0;
		myDeltaY = 0;
		myWheel = 0;

		buttonState = checkSDLValue(SDL_GetMouseState(&myX, &myY));
	}
}
