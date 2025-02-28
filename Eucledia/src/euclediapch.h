#pragma once

#ifdef EUCLEDIA_PLATFORM_WNDOWS
	#ifndef NOMINAX
		#define NOMIMAX
	#endif
#endif

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <sstream>
#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "Eucledia/Core/Base.h"

#include "Eucledia/Core/Log.h"

#include "Eucledia/Debug/Instrumentor.h"

#ifdef EUCLEDIA_PLATFORM_WNDOWS
	#include <Windows.h>
#endif