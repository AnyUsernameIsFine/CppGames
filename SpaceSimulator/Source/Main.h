#pragma once

#include <Framework.hpp>

#include "Universe.h"

namespace SpaceSimulator
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
			//graphics.window.enableFullscreen();
			//graphics.window.enableResizing();
			graphics.window.enableCursor(false);
			//graphics.window.enableVSync(false);
			graphics.window.enableAntiAliasing();
		}

		void initialize()
		{
			graphics.text.loadFont("Resources/consola.ttf", "Consolas");
			graphics.text.setFont("Consolas", graphics.window.getHeight() / 32);
			graphics.text.setColor(0.39f, 0.58f, 0.93f);

			CoordinateSystem::initialize();

			camera.setAspectRatio((float)graphics.window.getWidth() / graphics.window.getHeight());

			generateUniverse();
		}

		void generateUniverse()
		{
			universe.create();

#ifdef UNIVERSE_SCALEZ
			auto galaxies = universe.getChildren();
			auto stars = galaxies[0]->getChildren();
			auto planets = stars[0]->getChildren();
			CoordinateSystem* cs = planets[0].get();
#else
			CoordinateSystem* cs = &universe;
#endif
			camera.setCoordinateSystem(cs);

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

		void update(float deltaSeconds)
		{
			if (input.isKeyDown(SDLK_g)) {
				Random::setRandSeed((uInt)(getGameTimeInSeconds() * 1000));
				seed = Random::randInt();
				Random::setHashSeed(seed);

				generateUniverse();
			}

			camera.move(
				(float)(input.isKeyDown(SDLK_d) - input.isKeyDown(SDLK_a)),
				(float)(input.isKeyDown(SDLK_r) - input.isKeyDown(SDLK_f)),
				(float)(input.isKeyDown(SDLK_s) - input.isKeyDown(SDLK_w))
			);

			float rollSensitivity = 90 * deltaSeconds;
			camera.transform.roll(rollSensitivity * (input.isKeyDown(SDLK_e) - input.isKeyDown(SDLK_q)));

			float sensitivity = 0.05f;
			camera.transform.yaw(sensitivity * input.getMouseDeltaX());
			camera.transform.pitch(sensitivity * input.getMouseDeltaY());

			camera.update(deltaSeconds);

			universe.update(camera);
		}

		void draw()
		{
			graphics.clearScreen();

			universe.draw(camera);

			StringStream stream;
			stream << "seed    " << seed << std::endl;
			stream << "speed   " << camera.getSpeedString() << std::endl;
			stream << "system  " << camera.getCoordinateSystem()->getName();

			graphics.text(2, -5) << (int)round(graphics.getFps()) << " fps";
			graphics.text(2, -5 + graphics.window.getHeight() - 3.0f * graphics.text.getFontHeight()) << stream;
		}
	};
}
