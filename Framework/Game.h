#pragma once

#include "Graphics.h"
#include "Input.h"

namespace Framework
{
	class Game abstract
	{
	public:
		Graphics graphics;
		Input input;

		bool run();

		virtual void start() = 0;
		virtual void update() = 0;
		virtual void draw() = 0;
		virtual void stop() = 0;

	private:
		bool isRunning_ = false;

		void renderThread_();
	};
}
