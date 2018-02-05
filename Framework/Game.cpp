#include "Game.h"
#include "Error.h"

#include <thread>

namespace Framework
{
	bool Game::run()
	{
		if (isRunning_) {
			return false;
		}
		isRunning_ = true;

		if (graphics.window.open_()) {
			std::thread thread(&Game::renderThread_, this);

			while (isRunning_) {
				if (graphics.window.pollEvents_(input)) {
					isRunning_ = false;
				}

				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			}

			thread.join();

			graphics.window.close_();

			return true;
		}

		return false;
	}

	void Game::renderThread_()
	{
		if (graphics.initialize_()) {
			start();

			while (isRunning_) {
				update();
				draw();
				graphics.update_();
			}

			stop();
		}
		else {
			isRunning_ = false;
		}
	}
}
