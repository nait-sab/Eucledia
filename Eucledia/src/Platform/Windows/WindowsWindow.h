#pragma once

#include "Eucledia/Window.h"

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
		virtual void init(const WindowProps& props);
		virtual void shutdown();

		GLFWwindow* _window;

		struct WindowData
		{
			std::string _title;
			unsigned int _width, _height;
			bool _vSync;
			EventCallbackFn _eventCallback;
		};

		WindowData _data;
	};
}

