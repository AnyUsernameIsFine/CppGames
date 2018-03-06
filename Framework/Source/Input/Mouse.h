#pragma once

#include "SDL_events.h"
#include "System\Globals.h"

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
		Mouse() {}
		void movedEventHandler(const SDL_MouseMotionEvent& event);
		void wheelEventHandler(const SDL_MouseWheelEvent& event);
		void update();

		int myX;
		int myY;
		int myDeltaX = 0;
		int myDeltaY = 0;
		int myWheel = 0;
		int fixedDeltaX;
		int fixedDeltaY;
		int fixedWheel;
		UInt32 buttonState;

		friend class Input;
	};
}
