#pragma once

#include "Window.h"
#include "Text.h"

#include <chrono>
#include <vector>

namespace Framework
{
	class Graphics
	{
	public:
		Window window;
		Text text;

		void clearScreen(float r = 0, float g = 0, float b = 0, bool depth = true);
		float getFps();
		float getDeltaSeconds();
		float getTotalSeconds();

	private:
		friend class Game;

		static const int FPS_BUFFER_SIZE_ = 60;
		const double NANOSECONDS_PER_SECOND_ = 1000000000.0;
		const std::string TEXT_VS_ = "text.vert";
		const std::string TEXT_FS_ = "text.frag";

		long long nanosecondsSinceStart_;
		long long frameLengths_[FPS_BUFFER_SIZE_];
		long long frameLengthsTotal_ = 0;
		int frameLengthsIndex_ = 0;
		int numberOfFrameLengths_ = 0;
		std::chrono::time_point<std::chrono::high_resolution_clock> frameTimePoint_;

		bool initialize_();
		void update_();
	};
}
