#include "Game.h"
#include "Error.h"

#include <thread>

namespace Framework
{
	float Game::getFramesPerSecond()
	{
		auto now = std::chrono::high_resolution_clock::now();
		auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(now - now_).count();
		float framesPerSecond = frames_ * 1000000000.0f / nanoseconds;

		frames_ = 0;
		now_ = now;

		return framesPerSecond;
	}

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

				graphics.window.update_();

				frames_++;
			}

			stop();
		}
		else {
			isRunning_ = false;
		}
	}
}
