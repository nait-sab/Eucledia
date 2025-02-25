#pragma once

#include "Eucledia/Core/Window.h"
#include "Eucledia/Renderer/GraphicsContext.h"

#include <GLFW/glfw3.h>

namespace Eucledia
{
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void onUpdate() override;

		uint32_t getWidth() const override { return _data._width; }
		uint32_t getHeight() const override { return _data._height; }

		// Window attributes
		void setEventCallback(const EventCallbackFn& callback) override 
		{
			_data._eventCallback = callback;
		};
		void setVSync(bool enabled) override;
		bool isVSync() const override;
		
		virtual void* getNativeWindow() const { return _window; }

	private:
		GLFWwindow* _window;
		scope<GraphicsContext> _context;

		struct WindowData
		{
			std::string _title;
			uint32_t _width, _height;
			bool _vSync;
			EventCallbackFn _eventCallback;
		};

		WindowData _data;

		virtual void init(const WindowProps& props);
		virtual void shutdown();
	};
}

