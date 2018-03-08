#pragma once

#include "Globals.h"
#include "Universe.h"

namespace SpaceSimulator
{
	class Game : public GLEngine::Game
	{
	public:
		Universe universe;
		Camera camera;
		int seed;

		Game()
		{
			graphics.window.setTitle("Space Simulator");
			graphics.window.setSize(960, 540);
			//graphics.window.enableFullscreen();
			//graphics.window.enableResizing();
			//graphics.window.enableCursor();
			//graphics.window.enableVSync(false);
			graphics.window.enableAntiAliasing();
		}

		void initialize()
		{
			graphics.text.loadFont("Resources/consola.ttf", "Font");
			graphics.text.setFont("Font", graphics.window.getHeight() / 32);
			graphics.text.setColor(0.39f, 0.58f, 0.93f);

			CoordinateSystem::initialize();

			camera.setAspectRatio(graphics.window.getWidth(), graphics.window.getHeight());

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
				Vector3 p = toPutCameraNextTo->transform().getPosition() +
					Vector3(0, 0, static_cast<Coordinate>(5 * toPutCameraNextTo->getRadius()));
				camera.setPosition(p);
			}

			camera.transform().setEulerAngles(0, 0, 0);

			universe.update(camera);
		}

		void update(float deltaSeconds)
		{
			if (input.keyboard.key(SDLK_g)) {
				Random::setRandSeed(static_cast<UInt>(getGameTimeInSeconds() * 1000));
				seed = Random::randInt();
				Random::setHashSeed(seed);

				generateUniverse();
			}

			camera.move(
				static_cast<float>((input.keyboard.key(SDLK_d) - input.keyboard.key(SDLK_a)/* + input.controller.leftXAxis()*/)),
				static_cast<float>((input.keyboard.key(SDLK_r) - input.keyboard.key(SDLK_f)/* + input.controller.rightShoulder() - input.controller.leftShoulder()*/)),
				static_cast<float>((input.keyboard.key(SDLK_s) - input.keyboard.key(SDLK_w)/* + input.controller.leftYAxis()*/))
			);

			float rollSpeed = 90 * deltaSeconds;
			camera.transform().roll(rollSpeed * (
				input.keyboard.key(SDLK_e) - input.keyboard.key(SDLK_q)/* +
				input.controller.rightTrigger() - input.controller.leftTrigger() +
				input.mouse.right() - input.mouse.left()*/)
			);

			float mouseSensitivity = 0.05f;
			float controllerSensitivity = 90 * deltaSeconds;
			camera.transform().yaw(mouseSensitivity * input.mouse.deltaX()/* + controllerSensitivity * input.controller.rightXAxis()*/);
			camera.transform().pitch(mouseSensitivity * input.mouse.deltaY()/* + controllerSensitivity * input.controller.rightYAxis()*/);

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

			graphics.text.draw(0, 0) << static_cast<int>(round(graphics.getFps())) << " fps";
			graphics.text.draw(0, graphics.window.getHeight() - 3 * graphics.text.getFontHeight()) << stream;
		}
	};
}
