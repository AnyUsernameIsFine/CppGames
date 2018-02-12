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
		Universe universe = Universe(100000000);
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
			//camera.coordinateSystem = &universe.descendants[0];
			//camera.coordinateSystem = &universe.descendants[0].descendants[0];
			//camera.coordinateSystem = &universe.descendants[0].descendants[1].descendants[1];
			//camera.coordinateSystem = &universe.descendants[1].descendants[1].descendants[1].descendants[1];
			//camera.coordinateSystem = &universe.descendants[0].descendants[0].descendants[0].descendants[0].descendants[0];

			camera.setAspectRatio((float)graphics.window.getWidth() / graphics.window.getHeight());
			camera.setClippingPlanes(0.001f, 10000000.0f);
			camera.setSize(10.0f);
			camera.transform.moveZ(10.0f);
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
			universe.descendants[0].transform.yaw(20 * graphics.getDeltaSeconds());
			universe.descendants[0].descendants[0].transform.roll(20 * graphics.getDeltaSeconds());
			universe.descendants[0].descendants[0].descendants[0].transform.pitch(20 * graphics.getDeltaSeconds());
			universe.descendants[0].descendants[0].descendants[0].descendants[0].transform.yaw(20 * graphics.getDeltaSeconds());
			universe.descendants[0].descendants[0].descendants[0].descendants[0].descendants[0].transform.roll(20 * graphics.getDeltaSeconds());
			universe.draw(camera);
		}

		void drawInfo()
		{
			int fps = (int)round(graphics.getFps());
			Position<Coordinate> position = camera.transform.getPosition();
			glm::ivec3 o = camera.transform.getOrientation();

			std::string fpsString = std::to_string(fps) + "fps";
			std::string csString = camera.coordinateSystem->name + " (" + std::to_string(camera.coordinateSystem->scale) + "m/u)";
			std::string positionString = "x: " + std::to_string(position.x) + " y: " + std::to_string(position.y) + " z: " + std::to_string(position.z);
			std::string orientatonString = "yaw: " + std::to_string(o.y) + " pitch: " + std::to_string(o.x) + " roll: " + std::to_string(o.z);

			graphics.text.draw(0, 0, fpsString + "\n" + csString + "\n" + positionString + "\n" + orientatonString);
		}
	};
}
