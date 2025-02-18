#pragma once

namespace Eucledia
{
	class Timestep
	{
	public:
		Timestep(float time) : _time(time)
		{

		}

		operator float() const { return _time; }

		float getSeconds() const { return _time; }
		float getMilliseconds() const { return _time * 1000.f; }

	private:
		float _time;
	};
}
