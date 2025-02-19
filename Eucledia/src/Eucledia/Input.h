#pragma once

#include "Eucledia/Core.h"

namespace Eucledia
{
	class EUCLEDIA_API Input
	{
	public:
		Input(const Input&) = delete;
		Input& operator=(const Input&) = delete;

		inline static bool isKeyPressed(int keycode) { return _instance->isKeyPressedImpl(keycode); }

		inline static bool isMouseButtonPressed(int button) { return _instance->isMouseButtonPressedImpl(button); }
		inline static std::pair<float, float> getMousePosition() { return _instance->getMousePositionImpl(); }
		inline static float getMouseX() { return _instance->getMouseXImpl(); }
		inline static float getMouseY() { return _instance->getMouseYImpl(); }

	protected:
		Input() = default;

		virtual bool isKeyPressedImpl(int keycode) = 0;

		virtual bool isMouseButtonPressedImpl(int button) = 0;
		virtual std::pair<float, float> getMousePositionImpl() = 0;
		virtual float getMouseXImpl() = 0;
		virtual float getMouseYImpl() = 0;

	private:
		static Input* _instance;
	};
}