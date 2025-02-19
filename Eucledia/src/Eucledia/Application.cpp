#include "euclediapch.h"
#include "Application.h"

#include "Eucledia/Renderer/Renderer.h"

#include <GLFW/glfw3.h>

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

		Renderer::init();

		_imGuiLayer = new ImGuiLayer();
		pushOverlay(_imGuiLayer);
	}

	Application::~Application()
	{
		delete _imGuiLayer;
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
		dispatcher.dispatch<WindowCloseEvent>(BIND_EVENT_FN(onWindowClosed));

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

			for (Layer* layer : _layerStack)
			{
				layer->onUpdate(timestep);
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
}
