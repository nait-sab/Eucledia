#pragma once

#include "Core.h"
#include "Window.h"
#include "Eucledia/LayerStack.h"
#include "Events/Event.h"
#include "Eucledia/Events/ApplicationEvent.h"

namespace Eucledia
{
	class EUCLEDIA_API Application
	{
	public:
		Application();
		virtual ~Application();

		void run();

		void onEvent(Event& event);
		void pushLayer(Layer* layer);
		void pushOverlay(Layer* layer);

	private:
		bool onWindowClosed(WindowCloseEvent& event);
		std::unique_ptr<Window> _window;
		bool _running = true;
		LayerStack _layerStack;
	};

	// To be defined in client
	Application* CreateApplication();
}
