#pragma once

#ifdef EUCLEDIA_PLATFORM_WNDOWS
	#ifdef EUCLEDIA_BUILD_DLL
		#define EUCLEDIA_API __declspec(dllexport)
	#else
		#define EUCLEDIA_API __declspec(dllimport)
	#endif
#else
	#error Eucledia only supports Windows
#endif

#ifdef EUCLEDIA_ENABLE_ASSERTS
	#define EUCLEDIA_ASSERT(x, ...) { if(!(x)) { EUCLEDIA_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define EUCLEDIA_CORE_ASSERT(x, ...) { if(!(x)) { EUCLEDIA_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define EUCLEDIA_ASSERT(x, ...)
	#define EUCLEDIA_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)