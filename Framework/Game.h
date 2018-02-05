#pragma once

#include "Graphics.h"
#include "Input.h"

#include <chrono>

namespace Framework
{
	class Game abstract
	{
	public:
		Graphics graphics;
		Input input;

		float getFramesPerSecond();
		bool run();

		virtual void start() = 0;
		virtual void update() = 0;
		virtual void draw() = 0;
		virtual void stop() = 0;

	private:
		bool isRunning_ = false;
		unsigned int frames_ = 0;
		std::chrono::time_point<std::chrono::high_resolution_clock> now_ =
			std::chrono::high_resolution_clock::now();

		void renderThread_();
	};
}
