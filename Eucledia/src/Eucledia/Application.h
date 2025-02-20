#pragma once

#include "Core.h"
#include "Window.h"
#include "Eucledia/LayerStack.h"
#include "Events/Event.h"
#include "Eucledia/Events/ApplicationEvent.h"

#include "Eucledia/Core/Timestep.h"

#include "Eucledia/ImGui/ImGuiLayer.h"

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

		inline static Application& get() { return *_instance; }
		inline Window& getWindow() { return *_window; }

	private:
		static Application* _instance;

		std::unique_ptr<Window> _window;

		bool onWindowClosed(WindowCloseEvent& event);
		bool onWindowResized(WindowResizeEvent& event);

		LayerStack _layerStack;
		ImGuiLayer* _imGuiLayer;

		bool _running = true;
		bool _minimized = false;
		float _lasFrameTime = 0;
	};

	// To be defined in client
	Application* CreateApplication();
}
