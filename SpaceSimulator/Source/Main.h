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
		int seed;

		Game()
		{
			graphics.window.setTitle("SpaceSimulator");
			graphics.window.setSize(960, 540);
			graphics.window.hideCursor();
			graphics.window.enableVSync();
			graphics.window.enableAntiAliasing();
			//graphics.window.setFullscreen(true);
		}

		void start()
		{
			graphics.text.loadFont("Resources/consola.ttf");
			graphics.text.setFont("Consolas", graphics.window.getHeight() / 32);
			graphics.text.setColor(0.39f, 0.58f, 0.93f);

			CoordinateSystem::initialize();

			camera.setAspectRatio((float)graphics.window.getWidth() / graphics.window.getHeight());

			generateUniverse();
		}

		void generateUniverse()
		{
			Random::setRandSeed((unsigned int)(graphics.getTotalSeconds() * 1000));
			seed = Random::randInt();
			Random::setHashSeed(seed);

			universe.create();

			CoordinateSystem* cs = &universe;
			camera.setCoordinateSystem(cs);

#ifdef UNIVERSE_SCALEZ
			auto galaxies = universe.getChildren();
			auto stars = galaxies[0].get()->getChildren();
			auto planets = stars[0].get()->getChildren();
			cs = planets[0].get();
			camera.setCoordinateSystem(cs);
#endif
			auto children = cs->getChildren();
			if (!children.empty()) {
				CoordinateSystem* toPutCameraNextTo = children[children.size() / 2].get();
				Vector3 p = toPutCameraNextTo->transform.getPosition() +
					Vector3(0, 0, (Coordinate)(5 * toPutCameraNextTo->getRadius()));
				camera.setPosition(p);
			}

			camera.transform.setEulerAngles(0, 0, 0);

			universe.update(camera);
		}

		void onMouseMove(int x, int y)
		{
			float sensitivity = 0.05f;

			camera.transform.yaw(sensitivity * x);
			camera.transform.pitch(sensitivity * y);
		}

		void update()
		{
			if (input.isKeyDown(SDLK_g)) {
				generateUniverse();
			}

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

			universe.draw(camera, graphics.getTotalSeconds());

			int fps = (int)round(graphics.getFps());
			Vector3 position = camera.transform.getPosition();
			glm::ivec3 o = camera.transform.getEulerAngles();

			std::string fpsString = std::to_string(fps) + " fps";
			std::string seedString = "seed     " + std::to_string(seed);
			std::string speedString = "speed    " + camera.getSpeedString();
			std::string csString = "system   " + camera.getCoordinateSystem()->getName();

			graphics.text.draw(2, -5, fpsString);
			graphics.text.draw(2, -5 + graphics.window.getHeight() - 3.0f * graphics.text.getFontHeight(),
				seedString + "\n" + speedString + "\n" + csString + "\n");
		}
	};
}
