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

		inline unsigned int getWidth() const override { return _data._width; }
		inline unsigned int getHeight() const override { return _data._height; }

		// Window attributes
		inline void setEventCallback(const EventCallbackFn& callback) override 
		{
			_data._eventCallback = callback;
		};
		void setVSync(bool enabled) override;
		bool isVSync() const override;
		
		inline virtual void* getNativeWindow() const { return _window; }

	private:
		GLFWwindow* _window;
		scope<GraphicsContext> _context;

		struct WindowData
		{
			std::string _title;
			unsigned int _width, _height;
			bool _vSync;
			EventCallbackFn _eventCallback;
		};

		WindowData _data;

		virtual void init(const WindowProps& props);
		virtual void shutdown();
	};
}

