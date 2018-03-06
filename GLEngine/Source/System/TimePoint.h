#pragma once

#include <chrono>

#include "System\Globals.h"

namespace GLEngine
{
	class TimePoint
	{
	public:
		float differenceInSeconds(const TimePoint& t) const;
		void setToNow();

	private:
		using Clock = std::chrono::high_resolution_clock;

		std::chrono::time_point<Clock> timePoint = Clock::now();
	};
}
