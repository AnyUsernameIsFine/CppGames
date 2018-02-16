#pragma once

#include <Framework.hpp>

#include "Universe.h"

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
			graphics.text.setFont("Resources/consola.ttf", 16);
			graphics.text.setColor(1, 1, 1);

			CoordinateSystem::createMesh();

			camera.setAspectRatio((float)graphics.window.getWidth() / graphics.window.getHeight());
			camera.setClippingPlanes(0.001f, 100000.0f);
			camera.setSize(10.0f);

			camera.coordinateSystem = &universe;
			CoordinateSystem* toPutCameraNextTo = camera.coordinateSystem->children[1].get();
			camera.transform.setPosition(toPutCameraNextTo->transform.getPosition());
			camera.transform.moveZ(5 * toPutCameraNextTo->radius);
		}

		void onKeyDown(SDL_Keycode key)
		{
			if (key == SDLK_p) {
				camera.setPerspective(!camera.isPerspective());
			}
		}

		void onMouseMove(int x, int y)
		{
			float sensitivity = 0.05f;

			camera.transform.yaw(sensitivity * x);
			camera.transform.pitch(sensitivity * y);
		}

		void onMouseWheel(int y)
		{
			camera.setFieldOfView(camera.getFieldOfView() - y * 10);
		}

		void update()
		{
			float deltaSeconds = graphics.getDeltaSeconds();

			camera.move(
				(float)(input.isKeyDown(SDLK_d) - input.isKeyDown(SDLK_a)),
				(float)(input.isKeyDown(SDLK_r) - input.isKeyDown(SDLK_f)),
				(float)(input.isKeyDown(SDLK_s) - input.isKeyDown(SDLK_w))
			);

			float rollSensitivity = 90 * deltaSeconds;
			camera.transform.roll(rollSensitivity * (input.isKeyDown(SDLK_e) - input.isKeyDown(SDLK_q)));

			camera.update(deltaSeconds);
		}

		void draw()
		{
			graphics.clearScreen(0, 0, 0);

			drawUniverse();
			drawInfo();
		}

		void drawUniverse()
		{
			universe.draw(camera);
		}

		void drawInfo()
		{
			int fps = (int)round(graphics.getFps());
			Vector3 position = camera.transform.getPosition();
			glm::ivec3 o = camera.transform.getEulerAngles();

			std::string fpsString = std::to_string(fps) + " fps";
			std::string csString = camera.coordinateSystem->name;
			std::string speedString = camera.getSpeedString();
			//std::string positionString = "x: " + std::to_string(position.x) + " y: " + std::to_string(position.y) + " z: " + std::to_string(position.z);
			//std::string orientatonString = "yaw: " + std::to_string(o.y) + " pitch: " + std::to_string(o.x) + " roll: " + std::to_string(o.z);

			graphics.text.draw(0, 0, fpsString + "\n" + csString + "\n" + speedString);
			//graphics.text.draw(0, 0, fpsString + "\n" + csString + "\n" + positionString + "\n" + orientatonString);
		}
	};
}
