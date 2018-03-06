#include "Input.h"

namespace GLEngine
{
	void Input::processEvent(const SDL_Event& event)
	{
		switch (event.type) {
			case SDL_CONTROLLERDEVICEADDED:
				controller.addedEventHandler(event.cdevice);
				break;

			case SDL_CONTROLLERDEVICEREMOVED:
				controller.removedEventHandler(event.cdevice);
				break;

			case SDL_MOUSEMOTION:
				mouse.movedEventHandler(event.motion);
				break;

			case SDL_MOUSEWHEEL:
				mouse.wheelEventHandler(event.wheel);
				break;
		}
	}

	void Input::update()
	{
		controller.update();
		keyboard.update();
		mouse.update();
	}
}
