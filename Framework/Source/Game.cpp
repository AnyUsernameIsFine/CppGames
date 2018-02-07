#include "Game.hpp"
#include "Error.hpp"

#include <thread>

namespace Framework
{
	int Game::run()
	{
		if (isRunning_) {
			return 1;
		}

		if (graphics.openWindow_() != 0) {
			return 1;
		}

		isRunning_ = true;

		std::thread thread(&Game::gameLoop_, this);

		while (isRunning_) {
			SDL_Event event;

			while (sdlCheckV(SDL_PollEvent(&event)) != 0) {
				input.processEvent_(event);

				switch (event.type) {
				case SDL_WINDOWEVENT:
					if (event.window.event == SDL_WINDOWEVENT_MINIMIZED) {
						isActive_ = false;
					}
					else if (event.window.event == SDL_WINDOWEVENT_RESTORED) {
						isActive_ = true;
					}
					break;
				case SDL_KEYDOWN:
					onKeyDown(event.key.keysym.sym);
					break;
				case SDL_MOUSEMOTION:
					onMouseMove(event.motion.xrel, event.motion.yrel);
					break;
				case SDL_MOUSEWHEEL:
					onMouseWheel(event.wheel.y);
					break;
				case SDL_QUIT:
					isRunning_ = false;
				}
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(MILLISECONDS_YIELD_));
		}

		thread.join();

		graphics.closeWindow_();

		return 0;
	}

	void Game::gameLoop_()
	{
		if (graphics.initialize_() != 0) {
			isRunning_ = false;
		}
		else {
			start();

			while (isRunning_) {
				update();

				if (isActive_) {
					draw();
					graphics.update_();
				}
				else {
					std::this_thread::sleep_for(std::chrono::milliseconds(MILLISECONDS_YIELD_));
				}
			}

			stop();
		}
	}
}
