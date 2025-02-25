#pragma once

#include "euclediapch.h"

#include "Eucledia/Core/Base.h"
#include "Eucledia/Events/Event.h"

namespace Eucledia
{
	struct WindowProps
	{
		std::string _title;
		uint32_t _width;
		uint32_t _height;

		WindowProps(const std::string& title = "Eucledia Engine", uint32_t width = 1600, uint32_t height = 900)
			: _title(title), _width(width), _height(height) {}
	};

	// Interface of a desktop system based Window
	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() = default;

		virtual void onUpdate() = 0;

		virtual uint32_t getWidth() const = 0;
		virtual uint32_t getHeight() const = 0;

		// Window attributes
		virtual void setEventCallback(const EventCallbackFn& callback) = 0;
		virtual void setVSync(bool enabled) = 0;
		virtual bool isVSync() const = 0;

		virtual void* getNativeWindow() const = 0;

		static scope<Window> create(const WindowProps& props = WindowProps());
	};
}