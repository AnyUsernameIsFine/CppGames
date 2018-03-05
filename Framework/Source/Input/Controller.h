#pragma once

#include "System\Globals.h"

#include "SDL_events.h"

#include <unordered_map>

namespace Framework
{
	class Controller
	{
	public:
		~Controller();
		float leftXAxis() const;
		float leftYAxis() const;
		float rightXAxis() const;
		float rightYAxis() const;
		float leftTrigger() const;
		float rightTrigger() const;
		bool a() const;
		bool b() const;
		bool x() const;
		bool y() const;
		bool back() const;
		bool guide() const;
		bool start() const;
		bool leftStick() const;
		bool rightStick() const;
		bool leftShoulder() const;
		bool rightShoulder() const;
		bool up() const;
		bool down() const;
		bool left() const;
		bool right() const;

	private:
		SDL_GameController* controller = nullptr;
		int16 axisLeftX;
		int16 axisLeftY;
		int16 axisRightX;
		int16 axisRightY;
		float normalizedAxisLeftX;
		float normalizedAxisLeftY;
		float normalizedAxisRightX;
		float normalizedAxisRightY;
		float axisTriggerLeft;
		float axisTriggerRight;
		bool buttonA;
		bool buttonB;
		bool buttonX;
		bool buttonY;
		bool buttonBack;
		bool buttonGuide;
		bool buttonStart;
		bool buttonLeftStick;
		bool buttonRightStick;
		bool buttonLeftShoulder;
		bool buttonRightShoulder;
		bool buttonUp;
		bool buttonDown;
		bool buttonLeft;
		bool buttonRight;

		static const int MAX_AXIS_VALUE = 1 << 15;
		static const int DEAD_ZONE = 1 << 12;

		Controller();
		void addedEventHandler(const SDL_ControllerDeviceEvent& event);
		void removedEventHandler(const SDL_ControllerDeviceEvent& event);
		void update();
		static float getNormalizedAxis(float axis, float otherAxis);
		int16 getAxis(SDL_GameControllerAxis axis) const;
		bool getButton(SDL_GameControllerButton button) const;

		friend class Input;
	};
}
