#include "Game.h"
#include "Error.h"

#include <thread>

namespace Framework
{
	int Game::run()
	{
		if (graphics.isActive_) {
			return 1;
		}

		if (graphics.openWindow_() != 0) {
			return 1;
		}

		std::thread thread(&Game::gameLoop_, this);

		graphics.eventLoop_(input);

		thread.join();

		graphics.closeWindow_();

		return 0;
	}

	void Game::gameLoop_()
	{
		if (graphics.initialize_() != 1) {
			start();

			while (graphics.isActive_) {
				update();

				if (!graphics.isWindowMinimized_()) {
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
