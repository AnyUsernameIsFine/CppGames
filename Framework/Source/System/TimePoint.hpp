#pragma once

#include "System\Globals.hpp"

#include <chrono>

namespace Framework
{
	class TimePoint
	{
	public:
		float differenceInSeconds(const TimePoint& t) const;
		void setToNow();

	private:
		typedef	std::chrono::high_resolution_clock Clock;

		std::chrono::time_point<Clock> timePoint = Clock::now();
	};
}
