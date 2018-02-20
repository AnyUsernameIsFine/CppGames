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
			//graphics.window.setFullscreen(true);
			graphics.window.hideCursor();
			graphics.window.enableVSync();
		}

		void start()
		{
			graphics.text.loadFont("Resources/consola.ttf");
			graphics.text.setFont("Consolas", 16);
			graphics.text.setColor(0.39f, 0.58f, 0.93f);

			camera.setAspectRatio((float)graphics.window.getWidth() / graphics.window.getHeight());
			camera.setClippingPlanes(0.001f, 100000.0f);

			CoordinateSystem::createMesh();

			camera.setCoordinateSystem(&universe);

			universe.create(camera);

			//auto children = &camera.getCoordinateSystem()->getChildren();
			//CoordinateSystem* toPutCameraNextTo = children->at(children->size() / 2).get();
			//Vector3 p = toPutCameraNextTo->transform.getPosition() + Vector3(0, 0, 5 * toPutCameraNextTo->getRadius());
			//camera.setPosition(p);
		}

		void onMouseMove(int x, int y)
		{
			float sensitivity = 0.05f;

			camera.transform.yaw(sensitivity * x);
			camera.transform.pitch(sensitivity * y);
		}

		void update()
		{
			camera.move(
				(float)(input.isKeyDown(SDLK_d) - input.isKeyDown(SDLK_a)),
				(float)(input.isKeyDown(SDLK_r) - input.isKeyDown(SDLK_f)),
				(float)(input.isKeyDown(SDLK_s) - input.isKeyDown(SDLK_w))
			);

			float deltaSeconds = graphics.getDeltaSeconds();

			float rollSensitivity = 90 * deltaSeconds;
			camera.transform.roll(rollSensitivity * (input.isKeyDown(SDLK_e) - input.isKeyDown(SDLK_q)));

			camera.update(deltaSeconds);

			universe.update(camera);
		}

		void draw()
		{
			graphics.clearScreen(0, 0, 0);

			universe.draw(camera);

			int fps = (int)round(graphics.getFps());
			Vector3 position = camera.transform.getPosition();
			glm::ivec3 o = camera.transform.getEulerAngles();

			std::string fpsString = std::to_string(fps) + " fps";
			std::string csString = camera.getCoordinateSystem()->getName();
			std::string speedString = camera.getSpeedString();
			std::string positionString = std::to_string(position.x) + ", " + std::to_string(position.y) + ", " + std::to_string(position.z);

			graphics.text.draw(2, -5, fpsString);
			graphics.text.draw(2, -5 + graphics.window.getHeight() - 3.0f * graphics.text.getFontHeight(),
				csString + "\n" + speedString + "\n" + positionString);
		}
	};
}
