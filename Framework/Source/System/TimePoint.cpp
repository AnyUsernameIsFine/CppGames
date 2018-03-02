#include "TimePoint.h"

namespace Framework
{
	float TimePoint::differenceInSeconds(const TimePoint& t) const
	{
		return std::chrono::duration_cast<std::chrono::nanoseconds>
			(timePoint - t.timePoint).count() / 1000000000.0f;
	}

	void TimePoint::setToNow()
	{
		timePoint = Clock::now();
	}
}
