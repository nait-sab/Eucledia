#include "euclediapch.h"
#include "Log.h"

namespace Eucledia
{
	ref<spdlog::logger> Log::_coreLogger;
	ref<spdlog::logger> Log::_clientLogger;

	void Log::init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");

		_coreLogger = spdlog::stdout_color_mt("EUCLEDIA");
		_coreLogger->set_level(spdlog::level::trace);

		_clientLogger = spdlog::stdout_color_mt("APP");
		_clientLogger->set_level(spdlog::level::trace);
	}
}