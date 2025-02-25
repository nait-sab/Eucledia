#pragma once

#include "Eucledia/Core/Base.h"

#include "Eucledia/Core/Window.h"
#include "Eucledia/Core/LayerStack.h"
#include "Eucledia/Events/Event.h"
#include "Eucledia/Events/ApplicationEvent.h"

#include "Eucledia/Core/Timestep.h"
#include "Eucledia/ImGui/ImGuiLayer.h"

namespace Eucledia
{
	class Application
	{
	public:
		Application(const std::string& name = "Eucledia App");
		virtual ~Application();

		void run();

		void onEvent(Event& event);
		void pushLayer(Layer* layer);
		void pushOverlay(Layer* layer);

		Window& getWindow() { return *_window; }

		void close();

		static Application& get() { return *_instance; }

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
	Application* createApplication();
}
