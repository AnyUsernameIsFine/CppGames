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
		Universe universe { 100000000 };
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
			graphics.text.setFont("Resources/consola.ttf", 16);
			graphics.text.setColor(1, 1, 1);

			CoordinateSystem::createMesh();

			camera.coordinateSystem = &universe;
			//camera.coordinateSystem = &universe.descendants.at(0);
			//camera.coordinateSystem = &universe.descendants.at(0).descendants.at(0);
			//camera.coordinateSystem = &universe.descendants.at(0).descendants.at(1).descendants.at(1);
			//camera.coordinateSystem = &universe.descendants.at(1).descendants.at(1).descendants.at(1).descendants.at(1);
			//camera.coordinateSystem = &universe.descendants.at(0).descendants.at(1).descendants.at(1).descendants.at(1).descendants.at(1);

			camera.setAspectRatio((float)graphics.window.getWidth() / graphics.window.getHeight());
			camera.setClippingPlanes(0.001f, 10000000.0f);
			camera.setSize(10.0f);
			camera.transform.moveZ((Coordinate)10);
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

			camera.transform.moveX((Coordinate)(cameraSpeed * (input.isKeyDown(SDLK_d) - input.isKeyDown(SDLK_a))));
			camera.transform.moveY((Coordinate)(cameraSpeed * (input.isKeyDown(SDLK_r) - input.isKeyDown(SDLK_f))));
			camera.transform.moveZ((Coordinate)(cameraSpeed * (input.isKeyDown(SDLK_s) - input.isKeyDown(SDLK_w))));
			camera.checkForCoordinateSystemSwap();

			float rollSensitivity = 90 * graphics.getDeltaSeconds();
			camera.transform.roll(rollSensitivity * (input.isKeyDown(SDLK_e) - input.isKeyDown(SDLK_q)));
		}

		void draw()
		{
			graphics.clearScreen(0, 0, 0);

			drawUniverse();
			drawInfo();
		}

		void drawUniverse()
		{
			universe.descendants.at(0).transform.yaw(20 * graphics.getDeltaSeconds());
			universe.descendants.at(0).descendants.at(0).transform.roll(20 * graphics.getDeltaSeconds());
			universe.descendants.at(0).descendants.at(0).descendants.at(0).transform.pitch(20 * graphics.getDeltaSeconds());
			universe.descendants.at(0).descendants.at(0).descendants.at(0).descendants.at(0).transform.yaw(20 * graphics.getDeltaSeconds());
			//universe.descendants.at(0).descendants.at(0).descendants.at(0).descendants.at(0).descendants.at(0).transform.roll(20 * graphics.getDeltaSeconds());
			universe.draw(camera);
		}

		void drawInfo()
		{
			int fps = (int)round(graphics.getFps());
			Vector3 position = camera.transform.getPosition();
			glm::ivec3 o = camera.transform.getEulerAngles();

			std::string fpsString = std::to_string(fps) + "fps";
			std::string csString = camera.coordinateSystem->name + " (" + std::to_string(camera.coordinateSystem->scale) + "m/u)";
			std::string positionString = "x: " + std::to_string(position.x) + " y: " + std::to_string(position.y) + " z: " + std::to_string(position.z);
			std::string orientatonString = "yaw: " + std::to_string(o.y) + " pitch: " + std::to_string(o.x) + " roll: " + std::to_string(o.z);

			graphics.text.draw(0, 0, fpsString + "\n" + csString + "\n" + positionString + "\n" + orientatonString);
		}
	};
}
