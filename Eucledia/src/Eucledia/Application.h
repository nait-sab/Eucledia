#pragma once

#include "Core.h"
#include "Window.h"
#include "Eucledia/LayerStack.h"
#include "Events/Event.h"
#include "Eucledia/Events/ApplicationEvent.h"

#include "Eucledia/ImGui/ImGuiLayer.h"

#include "Eucledia/Renderer/Shader.h"
#include "Eucledia/Renderer/Buffer.h"
#include "Eucledia/Renderer/VertexArray.h"

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

		bool onWindowClosed(WindowCloseEvent& event);
		std::unique_ptr<Window> _window;
		ImGuiLayer* _imGuiLayer;
		bool _running = true;
		LayerStack _layerStack;

		std::shared_ptr<Shader> _shader;
		std::shared_ptr<VertexArray> _vertexArray;

		std::shared_ptr<Shader> _squareShader;
		std::shared_ptr<VertexArray> _squareVA;
	};

	// To be defined in client
	Application* CreateApplication();
}
