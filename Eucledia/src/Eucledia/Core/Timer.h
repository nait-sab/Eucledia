#pragma once

#include <chrono>

namespace Eucledia
{
	class Timer
	{
	public:
		Timer()
		{
			reset();
		}

		void Timer::reset()
		{
			_start = std::chrono::high_resolution_clock::now();
		}

		float Timer::elapsed()
		{
			return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() -
				_start).count() * 0.001f * 0.001f * 0.001;
		}

		float Timer::elapsedMillis()
		{
			return elapsed() * 1000.f;
		}

	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> _start;
	};
}