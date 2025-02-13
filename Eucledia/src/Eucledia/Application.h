#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Eucledia/Events/ApplicationEvent.h"
#include "Window.h"

namespace Eucledia
{
	class EUCLEDIA_API Application
	{
	public:
		Application();
		virtual ~Application();

		void run();

		void onEvent(Event& event);

	private:
		bool onWindowClosed(WindowCloseEvent& event);
		std::unique_ptr<Window> _window;
		bool _running = true;
	};

	// To be defined in client
	Application* CreateApplication();
}
