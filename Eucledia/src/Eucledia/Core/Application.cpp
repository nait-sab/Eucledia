#include "euclediapch.h"
#include "Eucledia/Core/Application.h"

#include "Eucledia/Renderer/Renderer.h"

#include <GLFW/glfw3.h>

namespace Eucledia
{
	Application* Application::_instance = nullptr;

	Application::Application()
	{
		EUCLEDIA_CORE_ASSERT(!_instance, "Application already exists")
		_instance = this;

		_window = Window::create();
		_window->setEventCallback(EUCLEDIA_BIND_EVENT_FN(Application::onEvent));

		Renderer::init();

		_imGuiLayer = new ImGuiLayer();
		pushOverlay(_imGuiLayer);
	}

	Application::~Application()
	{
		Renderer::shutdown();
	}

	void Application::pushLayer(Layer* layer)
	{
		_layerStack.pushLayer(layer);
	}

	void Application::pushOverlay(Layer* layer)
	{
		_layerStack.pushOverlay(layer);
	}

	void Application::onEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.dispatch<WindowCloseEvent>(EUCLEDIA_BIND_EVENT_FN(Application::onWindowClosed));
		dispatcher.dispatch<WindowResizeEvent>(EUCLEDIA_BIND_EVENT_FN(Application::onWindowResized));

		for (auto it = _layerStack.end(); it != _layerStack.begin();)
		{
			(*--it)->onEvent(event);
			
			if (event._handled)
			{
				break;
			}
		}
	}

	void Application::run()
	{
		while (_running)
		{
			float time = (float)glfwGetTime();
			Timestep timestep = time - _lasFrameTime;
			_lasFrameTime = time;

			if (!_minimized)
			{
				for (Layer* layer : _layerStack)
				{
					layer->onUpdate(timestep);
				}
			}

			_imGuiLayer->begin();

			for (Layer* layer : _layerStack)
			{
				layer->onImGuiRender();
			}

			_imGuiLayer->end();

			_window->onUpdate();
		}
	}

	bool Application::onWindowClosed(WindowCloseEvent& event)
	{
		_running = false;
		return true;
	}

	bool Application::onWindowResized(WindowResizeEvent& event)
	{
		if (event.getWidth() == 0 || event.getHeight() == 0)
		{
			_minimized = true;
			return false;
		}

		_minimized = false;
		Renderer::onWindowRisized(event.getWidth(), event.getHeight());

		return false;
	}
}
