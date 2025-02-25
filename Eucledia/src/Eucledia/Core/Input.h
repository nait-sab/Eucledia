#pragma once

#include "Eucledia/Core/Base.h"

namespace Eucledia
{
	class Input
	{
	public:
		static bool isKeyPressed(int keycode);

		static bool isMouseButtonPressed(int button);
		static std::pair<float, float> getMousePosition();
		static float getMouseX();
		static float getMouseY();
	};
}