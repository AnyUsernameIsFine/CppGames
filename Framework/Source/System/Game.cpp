#include "Game.h"

#include <iostream>
#include <sstream>
#include <thread>
#define NOMINMAX
#include <windows.h>

#include <SDL.h>

namespace
{
	class StringBuffer : public std::stringbuf
	{
	public:
		int sync()
		{
			fputs(str().c_str(), stdout);
			str("");
			return 0;
		}
	};

	static StringBuffer buffer;

	void enableUtf8InConsole()
	{
		SetConsoleOutputCP(CP_UTF8);
		setvbuf(stdout, nullptr, _IONBF, 0);
		std::cout.rdbuf(&buffer);
	}
}

namespace Framework
{
	Game::Game()
	{
		enableUtf8InConsole();
	}

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

			while (checkSDLValue(SDL_PollEvent(&event)) != 0) {
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
				input.update();
				update(gameSeconds - updateTimeInSeconds);
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
			static_cast<Game*>(game)->graphics.windowEventHandler(event->window);
		}

		return 0;
	}

	Game::SDL::SDL()
	{
		checkSDLValue(SDL_Init(0));
	}

	Game::SDL::~SDL()
	{
		SDL_Quit();
	}
}
