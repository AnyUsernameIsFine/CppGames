#pragma once

#include <Framework.hpp>

#include "Universe.h"
#include "Camera.h"

namespace Game
{
	using namespace Framework;

	class Game : public Framework::Game
	{
	public:
		Universe universe;
		Camera camera;

		Game()
		{
			graphics.window.setTitle("SpaceSimulator");
			graphics.window.setSize(960, 540);
			graphics.window.hideCursor();
			graphics.window.enableVSync();
		}

		void start()
		{
			CoordinateSystem::createMesh();

			universe.create(L"Universe", 10000);

			//camera.coordinateSystem = &universe;
			//camera.coordinateSystem = &universe.coordinateSystems[1];
			camera.coordinateSystem = &universe.coordinateSystems[0].coordinateSystems[0];
			//camera.coordinateSystem = &universe.coordinateSystems[1].coordinateSystems[1].coordinateSystems[1];

			camera.setAspectRatio((float)graphics.window.getWidth() / graphics.window.getHeight());
			camera.setClippingPlanes(0.001f, 10000000.0f);
			camera.setSize(10.0f);

			graphics.text.setFont("Resources/consola.ttf", 16);
			graphics.text.setColor(1, 1, 1);
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
			camera.transform.moveY(cameraSpeed * (input.isKeyDown(SDLK_r) - input.isKeyDown(SDLK_f)));
			camera.transform.moveZ(cameraSpeed * (input.isKeyDown(SDLK_s) - input.isKeyDown(SDLK_w)));

			float rollSensitivity = 90 * graphics.getDeltaSeconds();
			camera.transform.roll(rollSensitivity * (input.isKeyDown(SDLK_e) - input.isKeyDown(SDLK_q)));
		}

		void draw()
		{
			graphics.clearScreen(0, 0, 0);

			//universe.coordinateSystems.back().coordinateSystems.back().transform.yaw(45 * graphics.getDeltaSeconds());
			//universe.coordinateSystems[0].transform.pitch(45 * graphics.getDeltaSeconds());
			//output(camera.coordinateSystem);
			universe.draw(camera);

			int fps = (int)round(graphics.getFps());
			Position<Coordinate> position = camera.transform.getPosition();
			glm::ivec3 o = camera.transform.getOrientation();

			std::wstring fpsString = std::to_wstring(fps) + L"fps";
			std::wstring csString = camera.coordinateSystem->name;
			std::wstring positionString = L"x: " + std::to_wstring(position.x) + L" y: " + std::to_wstring(position.y) + L" z: " + std::to_wstring(position.z);
			std::wstring orientatonString = L"yaw: " + std::to_wstring(o.y) + L"° pitch: " + std::to_wstring(o.x) + L"° roll: " + std::to_wstring(o.z) + L"°";

			graphics.text.draw(0, 0, fpsString + L"\n" + csString + L"\n" + positionString + L"\n" + orientatonString);
		}
	};
}
