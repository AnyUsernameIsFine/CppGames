#pragma once

#include <unordered_map>

#include <SDL_events.h>
#include "System\Globals.h"

namespace GLEngine
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
		static constexpr int MAX_AXIS_VALUE = 1 << 15;
		static constexpr int DEAD_ZONE = 1 << 12;

		Controller();
		void addedEventHandler(const SDL_ControllerDeviceEvent& event);
		void removedEventHandler(const SDL_ControllerDeviceEvent& event);
		void update();
		Int16 getAxis(SDL_GameControllerAxis axis) const;
		bool getButton(SDL_GameControllerButton button) const;

		static float getNormalizedAxis(float axis, float otherAxis);

		SDL_GameController* controller = nullptr;
		Int16 axisLeftX;
		Int16 axisLeftY;
		Int16 axisRightX;
		Int16 axisRightY;
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

		friend class Input;
	};
}
