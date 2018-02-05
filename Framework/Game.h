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

		int run();

		virtual void start() = 0;
		virtual void update() = 0;
		virtual void draw() = 0;
		virtual void stop() = 0;

	private:
		static const int MILLISECONDS_YIELD_ = 10;

		void gameLoop_();
	};
}
