#pragma once

#include "System\Globals.h"

#include "SDL_events.h"

namespace Framework
{
	class Mouse
	{
	public:
		int x() const;
		int y() const;
		int deltaX() const;
		int deltaY() const;
		int wheel() const;
		bool left() const;
		bool middle() const;
		bool right() const;

	private:
		int myX;
		int myY;
		int myDeltaX = 0;
		int myDeltaY = 0;
		int myWheel = 0;
		int fixedDeltaX;
		int fixedDeltaY;
		int fixedWheel;
		Uint32 buttonState;

		void movedEventHandler(const SDL_MouseMotionEvent& event);
		void wheelEventHandler(const SDL_MouseWheelEvent& event);
		void update();

		friend class Input;
	};
}
