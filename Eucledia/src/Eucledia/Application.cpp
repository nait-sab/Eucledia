#include "euclediapch.h"
#include "Application.h"

#include <glad/glad.h>

namespace Eucledia
{
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1) 

	Application* Application::_instance = nullptr;

	Application::Application()
	{
		EUCLEDIA_CORE_ASSERT(!_instance, "Application already exists")
		_instance = this;

		_window = std::unique_ptr<Window>(Window::create());
		_window->setEventCallback(BIND_EVENT_FN(onEvent));
	}

	Application::~Application()
	{
	}

	void Application::pushLayer(Layer* layer)
	{
		_layerStack.pushLayer(layer);
		layer->onAttach();
	}

	void Application::pushOverlay(Layer* layer)
	{
		_layerStack.pushOverlay(layer);
		layer->onAttach();
	}

	void Application::onEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.dispatch<WindowCloseEvent>(BIND_EVENT_FN(onWindowClosed));

		EUCLEDIA_CORE_TRACE("{0}", event.toString());

		for (auto it = _layerStack.end(); it != _layerStack.begin();)
		{
			(*--it)->onEvent(event);
			
			if (event.getHandled())
			{
				break;
			}
		}
	}

	void Application::run()
	{
		while (_running)
		{
			for (Layer* layer : _layerStack)
			{
				layer->onUpdate();
			}

			_window->onUpdate();
		}
	}

	bool Application::onWindowClosed(WindowCloseEvent& event)
	{
		_running = false;
		return true;
	}
}
