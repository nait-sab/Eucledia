#pragma once

#include "Eucledia/Core/Core.h"

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Eucledia
{
	class Log
	{
	public:
		static void init();

		static ref<spdlog::logger>& getCoreLogger() { return _coreLogger; }
		static ref<spdlog::logger>& getClientLogger() { return _clientLogger; }

	private:
		static ref<spdlog::logger> _coreLogger;
		static ref<spdlog::logger> _clientLogger;
	};
}

// Core Logs macros
#define EUCLEDIA_CORE_TRACE(...)	::Eucledia::Log::getCoreLogger()->trace(__VA_ARGS__)
#define EUCLEDIA_CORE_INFO(...)		::Eucledia::Log::getCoreLogger()->info(__VA_ARGS__)
#define EUCLEDIA_CORE_WARN(...)		::Eucledia::Log::getCoreLogger()->warn(__VA_ARGS__)
#define EUCLEDIA_CORE_ERROR(...)	::Eucledia::Log::getCoreLogger()->error(__VA_ARGS__)
#define EUCLEDIA_CORE_CRITICAL(...)	::Eucledia::Log::getCoreLogger()->critical(__VA_ARGS__)

// App Logs macros
#define EUCLEDIA_TRACE(...)			::Eucledia::Log::getClientLogger()->trace(__VA_ARGS__)
#define EUCLEDIA_INFO(...)			::Eucledia::Log::getClientLogger()->info(__VA_ARGS__)
#define EUCLEDIA_WARN(...)			::Eucledia::Log::getClientLogger()->warn(__VA_ARGS__)
#define EUCLEDIA_ERROR(...)			::Eucledia::Log::getClientLogger()->error(__VA_ARGS__)
#define EUCLEDIA_CRITICAL(...)			::Eucledia::Log::getClientLogger()->critical(__VA_ARGS__)