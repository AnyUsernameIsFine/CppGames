#include "Controller.h"

#include <algorithm>

#include <SDL.h>

namespace GLEngine
{
	Controller::~Controller()
	{
		checkSDL(SDL_QuitSubSystem(SDL_INIT_GAMECONTROLLER));
	}

	float Controller::leftXAxis() const
	{
		return normalizedAxisLeftX;
	}

	float Controller::leftYAxis() const
	{
		return normalizedAxisLeftY;
	}

	float Controller::rightXAxis() const
	{
		return normalizedAxisRightX;
	}

	float Controller::rightYAxis() const
	{
		return normalizedAxisRightY;
	}

	float Controller::leftTrigger() const
	{
		return axisTriggerLeft;
	}

	float Controller::rightTrigger() const
	{
		return axisTriggerRight;
	}

	bool Controller::a() const
	{
		return buttonA;
	}

	bool Controller::b() const
	{
		return buttonB;
	}

	bool Controller::x() const
	{
		return buttonX;
	}

	bool Controller::y() const
	{
		return buttonY;
	}

	bool Controller::back() const
	{
		return buttonBack;
	}

	bool Controller::guide() const
	{
		return buttonGuide;
	}

	bool Controller::start() const
	{
		return buttonStart;
	}

	bool Controller::leftStick() const
	{
		return buttonLeftStick;
	}

	bool Controller::rightStick() const
	{
		return buttonRightStick;
	}

	bool Controller::leftShoulder() const
	{
		return buttonLeftShoulder;
	}

	bool Controller::rightShoulder() const
	{
		return buttonRightShoulder;
	}

	bool Controller::up() const
	{
		return buttonUp;
	}

	bool Controller::down() const
	{
		return buttonDown;
	}

	bool Controller::left() const
	{
		return buttonLeft;
	}

	bool Controller::right() const
	{
		return buttonRight;
	}

	Controller::Controller()
	{
		checkSDL(SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER));
	}

	void Controller::addedEventHandler(const SDL_ControllerDeviceEvent& event)
	{
		if (!controller && checkSDLValue(SDL_IsGameController(event.which))) {
			controller = checkSDLValue(SDL_GameControllerOpen(event.which));
		}
	}

	void Controller::removedEventHandler(const SDL_ControllerDeviceEvent& event)
	{
		SDL_Joystick* joystick = checkSDLValue(SDL_GameControllerGetJoystick(controller));
		if (event.which == checkSDLValue(SDL_JoystickInstanceID(joystick))) {
			checkSDL(SDL_GameControllerClose(controller));
			controller = nullptr;
		}
	}

	void Controller::update()
	{
		// TODO: optimize; these function calls seem to be somewhat expensive
		if (controller) {
			axisLeftX = getAxis(SDL_CONTROLLER_AXIS_LEFTX);
			axisLeftY = getAxis(SDL_CONTROLLER_AXIS_LEFTY);
			axisRightX = getAxis(SDL_CONTROLLER_AXIS_RIGHTX);
			axisRightY = getAxis(SDL_CONTROLLER_AXIS_RIGHTY);

			normalizedAxisLeftX = getNormalizedAxis(axisLeftX, axisLeftY);
			normalizedAxisLeftY = getNormalizedAxis(axisLeftY, axisLeftX);
			normalizedAxisRightX = getNormalizedAxis(axisRightX, axisRightY);
			normalizedAxisRightY = getNormalizedAxis(axisRightY, axisRightX);
			axisTriggerLeft = static_cast<float>(getAxis(SDL_CONTROLLER_AXIS_TRIGGERLEFT)) / MAX_AXIS_VALUE;
			axisTriggerLeft = static_cast<float>(getAxis(SDL_CONTROLLER_AXIS_TRIGGERRIGHT)) / MAX_AXIS_VALUE;
			buttonA = getButton(SDL_CONTROLLER_BUTTON_A);
			buttonB = getButton(SDL_CONTROLLER_BUTTON_B);
			buttonX = getButton(SDL_CONTROLLER_BUTTON_X);
			buttonY = getButton(SDL_CONTROLLER_BUTTON_Y);
			buttonBack = getButton(SDL_CONTROLLER_BUTTON_BACK);
			buttonGuide = getButton(SDL_CONTROLLER_BUTTON_GUIDE);
			buttonStart = getButton(SDL_CONTROLLER_BUTTON_START);
			buttonLeftStick = getButton(SDL_CONTROLLER_BUTTON_LEFTSTICK);
			buttonRightStick = getButton(SDL_CONTROLLER_BUTTON_RIGHTSTICK);
			buttonLeftShoulder = getButton(SDL_CONTROLLER_BUTTON_LEFTSHOULDER);
			buttonRightShoulder = getButton(SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);
			buttonUp = getButton(SDL_CONTROLLER_BUTTON_DPAD_UP);
			buttonDown = getButton(SDL_CONTROLLER_BUTTON_DPAD_DOWN);
			buttonLeft = getButton(SDL_CONTROLLER_BUTTON_DPAD_LEFT);
			buttonRight = getButton(SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
		}
	}

	Int16 Controller::getAxis(SDL_GameControllerAxis axis) const
	{
		return checkSDLValue(SDL_GameControllerGetAxis(controller, axis));
	}

	bool Controller::getButton(SDL_GameControllerButton button) const
	{
		return checkSDLValue(SDL_GameControllerGetButton(controller, button));
	}

	float Controller::getNormalizedAxis(float axis, float otherAxis)
	{
		float distance = std::min(float{ MAX_AXIS_VALUE }, sqrtf(axis * axis + otherAxis * otherAxis));
		return distance < DEAD_ZONE ? 0 : axis * (distance - DEAD_ZONE) / (MAX_AXIS_VALUE - DEAD_ZONE) / distance;
	}
}
