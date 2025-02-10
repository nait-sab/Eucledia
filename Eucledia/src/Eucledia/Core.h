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
