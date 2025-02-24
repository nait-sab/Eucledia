#pragma once

#include "euclediapch.h"

#include "Eucledia/Core/Base.h"
#include "Eucledia/Events/Event.h"

namespace Eucledia
{
	struct WindowProps
	{
		std::string _title;
		unsigned int _width;
		unsigned int _height;

		WindowProps(const std::string& title = "Eucledia Engine", unsigned int width = 1600, unsigned int height = 900) 
			: _title(title), _width(width), _height(height) {}
	};

	// Interface of a desktop system based Window
	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() = default;

		virtual void onUpdate() = 0;

		virtual unsigned int getWidth() const = 0;
		virtual unsigned int getHeight() const = 0;

		// Window attributes
		virtual void setEventCallback(const EventCallbackFn& callback) = 0;
		virtual void setVSync(bool enabled) = 0;
		virtual bool isVSync() const = 0;

		virtual void* getNativeWindow() const = 0;

		static scope<Window> create(const WindowProps& props = WindowProps());
	};
}