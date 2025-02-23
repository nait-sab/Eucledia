#pragma once

#include "Eucledia/Core/Core.h"

namespace Eucledia
{
	class Input
	{
	public:
		Input(const Input&) = delete;
		Input& operator=(const Input&) = delete;

		static bool isKeyPressed(int keycode) { return _instance->isKeyPressedImpl(keycode); }

		static bool isMouseButtonPressed(int button) { return _instance->isMouseButtonPressedImpl(button); }
		static std::pair<float, float> getMousePosition() { return _instance->getMousePositionImpl(); }
		static float getMouseX() { return _instance->getMouseXImpl(); }
		static float getMouseY() { return _instance->getMouseYImpl(); }

		static scope<Input> create();

	protected:
		Input() = default;

		virtual bool isKeyPressedImpl(int keycode) = 0;

		virtual bool isMouseButtonPressedImpl(int button) = 0;
		virtual std::pair<float, float> getMousePositionImpl() = 0;
		virtual float getMouseXImpl() = 0;
		virtual float getMouseYImpl() = 0;

	private:
		static scope<Input> _instance;
	};
}