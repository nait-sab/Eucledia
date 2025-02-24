#include "euclediapch.h"
#include "Eucledia/Core/Application.h"

#include "Eucledia/Renderer/Renderer.h"

#include <GLFW/glfw3.h>

namespace Eucledia
{
	Application* Application::_instance = nullptr;

	Application::Application()
	{
		EUCLEDIA_PROFILE_FUNCTION();
		 
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
		EUCLEDIA_PROFILE_FUNCTION();

		Renderer::shutdown();
	}

	void Application::pushLayer(Layer* layer)
	{
		EUCLEDIA_PROFILE_FUNCTION();

		_layerStack.pushLayer(layer);
		layer->onAttach();
	}

	void Application::pushOverlay(Layer* layer)
	{
		EUCLEDIA_PROFILE_FUNCTION();

		_layerStack.pushOverlay(layer);
		layer->onAttach();
	}

	void Application::onEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.dispatch<WindowCloseEvent>(EUCLEDIA_BIND_EVENT_FN(Application::onWindowClosed));
		dispatcher.dispatch<WindowResizeEvent>(EUCLEDIA_BIND_EVENT_FN(Application::onWindowResized));

		for (auto it = _layerStack.rbegin(); it != _layerStack.rend(); ++it)
		{			
			if (event._handled)
			{
				break;
			}

			(*it)->onEvent(event);
		}
	}

	void Application::run()
	{
		EUCLEDIA_PROFILE_FUNCTION();

		while (_running)
		{
			EUCLEDIA_PROFILE_SCOPE("Application::run loop")

			float time = (float)glfwGetTime();
			Timestep timestep = time - _lasFrameTime;
			_lasFrameTime = time;

			if (!_minimized)
			{
				{
					EUCLEDIA_PROFILE_SCOPE("Application::run layerStack update");

					for (Layer* layer : _layerStack)
					{
						layer->onUpdate(timestep);
					}
				}

				_imGuiLayer->begin();

				{
					EUCLEDIA_PROFILE_SCOPE("Application::run ImGui Render");

					for (Layer* layer : _layerStack)
					{
						layer->onImGuiRender();
					}
				}

				_imGuiLayer->end();
			}

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
		EUCLEDIA_PROFILE_FUNCTION();

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
