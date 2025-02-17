#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/fmt/ostr.h"

namespace Eucledia
{
	class EUCLEDIA_API Log
	{
	public:
		static void init();

		inline static std::shared_ptr<spdlog::logger>& getCoreLogger() { return _coreLogger; }
		inline static std::shared_ptr<spdlog::logger>& getClientLogger() { return _clientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> _coreLogger;
		static std::shared_ptr<spdlog::logger> _clientLogger;
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