#include "Game.h"

#include <thread>

namespace Framework
{
	int Game::run()
	{
		if (isRunning || !graphics.createWindow()) {
			return 1;
		}

		isRunning = true;

		std::thread thread(&Game::gameLoop, this);

		SDL_AddEventWatch(sdlEventHandler, this);

		while (isRunning) {
			SDL_Event event;

			while (sdlCheckValue(SDL_PollEvent(&event)) != 0) {
				input.processEvent(event);

				switch (event.type) {
				case SDL_WINDOWEVENT:
					if (event.window.event == SDL_WINDOWEVENT_MINIMIZED) {
						isWindowActive = false;
					}
					else if (event.window.event == SDL_WINDOWEVENT_RESTORED) {
						isWindowActive = true;
					}
					break;

				case SDL_QUIT:
					isRunning = false;
					break;
				}
			}

			sleep(MILLISECONDS_YIELD);
		}

		thread.join();

		return 0;
	}

	float Game::getGameTimeInSeconds() const
	{
		return TimePoint().differenceInSeconds(startTime);
	}

	void Game::gameLoop()
	{
		if (!graphics.initialize()) {
			isRunning = false;
		}
		else {
			initialize();
			updateTimeInSeconds = getGameTimeInSeconds();

			while (isRunning) {
				float gameSeconds = getGameTimeInSeconds();
				update(gameSeconds - updateTimeInSeconds);
				input.clear();
				updateTimeInSeconds = gameSeconds;

				if (isWindowActive) {
					draw();
					graphics.update();
				}
				else {
					sleep(MILLISECONDS_YIELD);
				}
			}

			finalize();
		}
	}

	void Game::sleep(int milliseconds) const
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
	}

	int Game::sdlEventHandler(void* game, SDL_Event* event)
	{
		if (event->type == SDL_WINDOWEVENT) {
			((Game*)game)->graphics.windowEventHandler(event->window);
		}

		return 0;
	}
}
