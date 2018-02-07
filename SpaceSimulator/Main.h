#pragma once

#include <Framework.hpp>

#include "Camera.h"
#include "Transform.h"

namespace Game
{
	using namespace Framework;

	class Game : public Framework::Game
	{
	public:
		Camera camera;

		Game()
		{
			graphics.window.setTitle("Space Simulator");
			graphics.window.setSize(960, 540);
			graphics.window.hideCursor();
			graphics.window.enableVSync();
		}

		void start()
		{
			graphics.text.setFont("Resources/consola.ttf", 16);
			graphics.text.setColor(1, 1, 1);

			camera.setAspectRatio((float)graphics.window.getWidth() / graphics.window.getHeight());
		}

		void onKeyDown(SDL_Keycode key)
		{
			if (key == SDLK_p) {
				camera.setPerspective(!camera.isPerspective());
			}
		}

		void onMouseMove(int x, int y)
		{
			float sensitivity = 10 * graphics.getDeltaSeconds();

			camera.transform.yaw(sensitivity * x);
			camera.transform.pitch(sensitivity * y);
		}

		void onMouseWheel(int y)
		{
			camera.setFieldOfView(camera.getFieldOfView() - y * 10);
		}

		void update()
		{
			float cameraSpeed = 5 * graphics.getDeltaSeconds();

			camera.transform.moveX(cameraSpeed * (input.isKeyDown(SDLK_d) - input.isKeyDown(SDLK_a)));
			camera.transform.moveZ(cameraSpeed * (input.isKeyDown(SDLK_s) - input.isKeyDown(SDLK_w)));

			float rollSensitivity = 90 * graphics.getDeltaSeconds();
			camera.transform.roll(rollSensitivity * (input.isKeyDown(SDLK_e) - input.isKeyDown(SDLK_q)));
		}

		void draw()
		{
			graphics.clearScreen(0, 0, 0);

			int fps = (int)round(graphics.getFps());
			glm::vec3 p = camera.transform.getPosition();
			glm::ivec3 o = camera.transform.getOrientation();

			std::wstring fpsString = std::to_wstring(fps) + L"fps";
			std::wstring positionString = L"x: " + std::to_wstring(p.x) + L" y: " + std::to_wstring(p.y) + L" z: " + std::to_wstring(p.z);
			std::wstring orientatonString = L"yaw: " + std::to_wstring(o.y) + L"° pitch: " + std::to_wstring(o.x) + L"° roll: " + std::to_wstring(o.z) + L"°";

			graphics.text.draw(0, 0, fpsString + L"\n" + positionString + L"\n" + orientatonString);
		}
	};
}
