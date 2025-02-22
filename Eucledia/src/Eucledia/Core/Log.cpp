#include "euclediapch.h"
#include "Eucledia/Core/Log.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace Eucledia
{
	ref<spdlog::logger> Log::_coreLogger;
	ref<spdlog::logger> Log::_clientLogger;

	void Log::init()
	{
		std::vector<spdlog::sink_ptr> logSinks;
		logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
		logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("Eucledia.log", true));

		logSinks[0]->set_pattern("%^[%T] %n: %v%$");
		logSinks[1]->set_pattern("[%T] [%1] %n: %v");

		_coreLogger = std::make_shared<spdlog::logger>("EUCLEDIA", begin(logSinks), end(logSinks));
		spdlog::register_logger(_coreLogger);
		_coreLogger->set_level(spdlog::level::trace);
		_coreLogger->flush_on(spdlog::level::trace);
		
		_clientLogger = std::make_shared<spdlog::logger>("APP", begin(logSinks), end(logSinks));
		spdlog::register_logger(_clientLogger);
		_clientLogger->set_level(spdlog::level::trace);
		_clientLogger->flush_on(spdlog::level::trace);
	}
}