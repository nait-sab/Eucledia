#include "euclediapch.h"
#include "Application.h"

#include <GLFW/glfw3.h>

namespace Eucledia
{
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1) 

	Application::Application()
	{
		_window = std::unique_ptr<Window>(Window::create());
		_window->setEventCallback(BIND_EVENT_FN(onEvent));
	}

	Application::~Application()
	{
	}

	void Application::onEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.dispatch<WindowCloseEvent>(BIND_EVENT_FN(onWindowClosed));
		EUCLEDIA_CORE_TRACE("{0}", event.toString());
	}

	void Application::run()
	{
		while (_running)
		{
			_window->onUpdate();
		}
	}

	bool Application::onWindowClosed(WindowCloseEvent& event)
	{
		_running = false;
		return true;
	}
}
